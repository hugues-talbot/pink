/* $Id: lattribarea.c,v 1.3 2009-03-13 14:46:14 mcouprie Exp $ */
/* 
   Operateurs connexes bases sur l'attribut surface
   ================================================
   (algorithme de P. Salembier)

   Operateurs : 
      lsegmentarea
      lareaopening
      lareaselnb   (selection d'un nombre donne de composantes)

   Michel Couprie - 1999-2002

   update MC 2009 : fix bug param > surface image
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcfahsalembier.h>
#include <mcutil.h>
#include <mcindic.h>
#include <lattribarea.h>

#define PARANO
/*
#define VERBOSE
#define DEBUG
#define DEBUGRECONS
#define DEBUGFLOOD
*/

#define ATTR_SURF

#include "lattrib.c"

/* ======================================================================== */
/* ======================================================================== */
/* OPERATEURS BASES SUR L'ARBRE DES COMPOSANTES */
/* ======================================================================== */
/* ======================================================================== */

/* ==================================== */
int32_t lsegmentarea(struct xvimage *image, int32_t connex, int32_t param, int32_t maximise)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsegmentarea"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fahs * FAHS;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  if (param >= N) 
  { 
    memset(F, NDG_MAX, N);
    return 1;
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAHS = CreeFahsVide(N);

  STATUS = (uint32_t *)calloc(1,N * sizeof(int32_t));
  if (STATUS == NULL)
  {   fprintf(stderr, "%s() : malloc failed for STATUS\n", F_NAME);
      return(0);
  }

  number_nodes = (uint32_t *)calloc(256, sizeof(int32_t));
  if (number_nodes == NULL)
  {   fprintf(stderr, "%s() : calloc failed for number_nodes\n", F_NAME);
      return(0);
  }

  node_at_level = (uint8_t *)calloc(256, sizeof(char));
  if (node_at_level == NULL)
  {   fprintf(stderr, "%s() : calloc failed for node_at_level\n", F_NAME);
      return(0);
  }
  
  TREE = InitCompTree(N);
  if (TREE == NULL)
  {   fprintf(stderr, "%s() : InitCompTree failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /* INITIALISATIONS                                  */
  /* ================================================ */

  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 1; i < N; i++) if (F[i] < F[k]) k = i;
  FahsPush(FAHS, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */


  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

#ifdef VERBOSE
  fprintf(stderr, "flood terminee\n");
#endif
#ifdef DEBUG
  AfficheCompTree(TREE);
#endif

  CTREE = CompTree2CompactTree(TREE, number_nodes);

#ifdef VERBOSE
  fprintf(stderr, "CompTree2CompactTree terminee\n");
#endif

  CalculeAttributs(CTREE);

#ifdef VERBOSE
  fprintf(stderr, "CalculeAttributs terminee\n");
#endif
#ifdef DEBUG
  AfficheCompactTree(CTREE);
#endif

  (void)FiltreSurfRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreSurfRec terminee\n");
#endif

  if ( maximise )
    (void)MaximiseSegmentation(CTREE, 0);

#ifdef VERBOSE
  fprintf(stderr, "MaximiseSegmentation terminee\n");
#endif

#ifdef DEBUG
  AfficheCompactTree(CTREE);
  WriteCompactTree(CTREE, "ctree.graph");
#endif

  RecupereSegmentation(CTREE, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereSegmentation terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahsTermine(FAHS);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lsegmentarea() */

/* ==================================== */
int32_t lareaopening(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lareaopening"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fahs * FAHS;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  if (param >= N) 
  { 
    memset(F, 0, N);
    return 1;
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAHS = CreeFahsVide(N);

  STATUS = (uint32_t *)calloc(1,N * sizeof(int32_t));
  if (STATUS == NULL)
  {   fprintf(stderr, "%s() : malloc failed for STATUS\n", F_NAME);
      return(0);
  }

  number_nodes = (uint32_t *)calloc(256, sizeof(int32_t));
  if (number_nodes == NULL)
  {   fprintf(stderr, "%s() : calloc failed for number_nodes\n", F_NAME);
      return(0);
  }

  node_at_level = (uint8_t *)calloc(256, sizeof(char));
  if (node_at_level == NULL)
  {   fprintf(stderr, "%s() : calloc failed for node_at_level\n", F_NAME);
      return(0);
  }
  
  TREE = InitCompTree(N);
  if (TREE == NULL)
  {   fprintf(stderr, "%s() : InitCompTree failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /* INITIALISATIONS                                  */
  /* ================================================ */

  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 1; i < N; i++) if (F[i] < F[k]) k = i;
  FahsPush(FAHS, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */
  
  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F); 

#ifdef VERBOSE
  fprintf(stderr, "flood terminee\n");
#endif
#ifdef DEBUG
  AfficheCompTree(TREE);
#endif

  CTREE = CompTree2CompactTree(TREE, number_nodes);

#ifdef VERBOSE
  fprintf(stderr, "CompTree2CompactTree terminee\n");
#endif
#ifdef DEBUG
  AfficheCompactTree(CTREE);
#endif

  CalculeAttributs(CTREE);

#ifdef VERBOSE
  fprintf(stderr, "CalculeAttributs terminee\n");
#endif

  (void)FiltreSurfRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreSurfRec terminee\n");
#endif

#ifdef DEBUG
  AfficheCompactTree(CTREE);
  WriteCompactTree(CTREE, "ctree.graph");
#endif

  RecupereImageFiltree(CTREE, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereImageFiltree terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahsTermine(FAHS);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lareaopening() */

/* ==================================== */
int32_t lareaclosing(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lareaclosing"
{
  register int32_t i;
  int32_t ret;
  int32_t N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *F = UCHARDATA(image);

  if (param >= N) 
  { 
    memset(F, NDG_MAX, N);
    return 1;
  }

  for (i = 0; i < N; i++) F[i] = 255 - F[i];
  ret = lareaopening(image, connex, param);
  for (i = 0; i < N; i++) F[i] = 255 - F[i];
  return(ret);
} /* lareaclosing() */

/* ==================================== */
int32_t lareaselnb(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lareaselnb"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fahs * FAHS;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * cpct;          /* resultat : l'arbre des composantes compacte' */
  int32_t nbcomp, nbfeuilles;
  int32_t *A;                       /* tableau pour le tri des composantes par hauteurs croissantes */

  if (param >= N) 
  { 
    memset(F, NDG_MAX, N);
    return 1;
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAHS = CreeFahsVide(N);

  STATUS = (uint32_t *)calloc(1,N * sizeof(int32_t));
  if (STATUS == NULL)
  {   fprintf(stderr, "%s() : malloc failed for STATUS\n", F_NAME);
      return(0);
  }

  number_nodes = (uint32_t *)calloc(256, sizeof(int32_t));
  if (number_nodes == NULL)
  {   fprintf(stderr, "%s() : calloc failed for number_nodes\n", F_NAME);
      return(0);
  }

  node_at_level = (uint8_t *)calloc(256, sizeof(char));
  if (node_at_level == NULL)
  {   fprintf(stderr, "%s() : calloc failed for node_at_level\n", F_NAME);
      return(0);
  }
  
  TREE = InitCompTree(N);
  if (TREE == NULL)
  {   fprintf(stderr, "%s() : InitCompTree failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /* INITIALISATIONS                                  */
  /* ================================================ */

  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 1; i < N; i++) if (F[i] < F[k]) k = i;
  FahsPush(FAHS, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAHS, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

#ifdef VERBOSE
  fprintf(stderr, "flood terminee\n");
#endif
#ifdef DEBUG
  AfficheCompTree(TREE);
#endif

  cpct = CompTree2CompactTree(TREE, number_nodes);

#ifdef VERBOSE
  fprintf(stderr, "CompTree2CompactTree terminee\n");
#endif
#ifdef DEBUG
  AfficheCompactTree(cpct);
#endif

  CalculeAttributs(cpct);

#ifdef VERBOSE
  fprintf(stderr, "CalculeAttributs terminee\n");
#endif
#ifdef DEBUG
  AfficheCompactTree(cpct);
#endif

  nbfeuilles = LeafMark(cpct);
  nbcomp = cpct->nbcomp;
  A = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (A == NULL)
  {   fprintf(stderr, "%s() : malloc failed\n", F_NAME);
      return(0);
  }
  for (i = 0; i < nbcomp; i++) A[i] = i;
  i_TriRapideStochastique (A, cpct->surf, 0, nbcomp-1);
  i = 0;
  while ((nbfeuilles > param) && (i < nbcomp))
  {
    //printf("i = %d, A[i] = %d, cpct->surf[A[i]] = %d, nbf = %d\n", 
    //       i, A[i], cpct->surf[A[i]], nbfeuilles);
    if (NbFilsNonFiltres(cpct, A[i]) == 0)
    {
      cpct->flags[A[i]] |= FILTERED_OUT;
      cpct->flags[A[i]] &= ~LEAF;
      k = cpct->pere[A[i]];
      if (NbFilsNonFiltres(cpct, k) != 0) 
        nbfeuilles--;
      else
        cpct->flags[k] |= LEAF;
    }
#ifdef PARANO
    else
      printf("Erreur imprevue : Composante non feuille : %d\n", A[i]);
#endif
    i++;
  } // while ((nbfeuilles > param) && (i < nbcomp))

#ifdef DEBUG
  AfficheCompactTree(cpct);
  WriteCompactTree(cpct, "ctree.graph");
#endif

  RecupereSegmentation(cpct, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereSegmentation terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahsTermine(FAHS);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  free(A);
  return(1);
} /* lareaselnb() */
