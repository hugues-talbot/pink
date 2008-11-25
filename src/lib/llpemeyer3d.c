/* $Id: llpemeyer3d.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
OBSOLETE - VOIR llpemeyer.c

/* operateur de calcul de la ligne de partage des eaux en 3D (6-voisinage)*/
/* utilise une File d'Attente Hierarchique */
/*
   d'apres "Un algorithme optimal de ligne de partage des eaux"
           F. Meyer - actes du 8eme congres AFCET - Lyon-Villeurbanne
           1991 
   variante de la section VI (ligne d'epaisseur 1 pixel)
*/
/* Michel Couprie - juillet 1997 */

#define PARANO                 /* even paranoid people have ennemies */
#define VERBOSE

/*
#define TRACEPUSHINIT
*/
#define TRACEPUSH
#define TRACEPOP
#define TRACEMARQUE

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfah.h>
#include <mcindic.h>
#include <mcchrono.h>
#include <llpemeyer3d.h>

#define EN_FAH   0

/* ==================================== */
int32_t NotIn(int32_t e, int32_t *list, int32_t n)                       
/* ==================================== */
{
/* renvoie 1 si e n'est pas dans list, 0 sinon */
/* e : l'element a rechercher */
/* list : la liste (tableau d'entiers) */
/* n : le nombre d'elements dans la liste */
  while (n > 0)
    if (list[--n] == e) return 0;
  return 1;
} /* NotIn() */

/* ==================================== */
int32_t llpemeyer3d(
        struct xvimage *image,
        struct xvimage *marqueurs,
        int32_t trace)
/* ==================================== */
{
  register int32_t i;                       /* index muet */
  register int32_t x;                       /* index muet de pixel */
  register int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t w;                       /* index muet (generalement un voisin de x) */
  register int32_t k;                       /* index muet */
  int32_t j;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint8_t *B = UCHARDATA(marqueurs);       /* l'image de marqueurs */
  uint32_t *M;             /* l'image d'etiquettes */
  int32_t nlabels;                 /* nombre de labels differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  chrono chrono1;
  int32_t tracedate = 0;
  int32_t etiqcc[6];
  int32_t ncc;  

  if ((rowsize(marqueurs) != rs) || (colsize(marqueurs) != cs))
  {
    fprintf(stderr, "llpemeyer3d: incompatible image sizes\n");
    return 0;
  }

  /* pour l'analyse de performances */
  start_chrono(&chrono1);

  IndicsInit(N);
  FAH = CreeFahVide(N+1);
  if (FAH == NULL)
  {   fprintf(stderr, "llpemeyer3d() : CreeFah failed\n");
      return(0);
  }

  /* ================================================ */
  /* CREATION DES LABELS INITIAUX                     */
  /* ================================================ */

  M = (uint32_t *)calloc(N, sizeof(int32_t));
  if (M == NULL)
  {   fprintf(stderr, "llpemeyer3d() : calloc failed\n");
      return(0);
  }
  nlabels = 0;

  for (x = 0; x < N; x++)
  {
    if (B[x] && (M[x] == 0))
    {
      nlabels += 1;
      M[x] = nlabels;
      FahPush(FAH, x, 0);
      while (! FahVide(FAH))
      {
        w = FahPop(FAH);
        for (k = 0; k <= 10; k += 2)
        {
          y = voisin6(w, k, rs, n, N);
          if ((y != -1) && (M[y] == 0) && (B[y] == B[w]))
          {
            M[y] = nlabels;
            FahPush(FAH, y, 0);
          } /* if y ... */
        } /* for k ... */
      } /* while (! FahVide(FAH)) */
    } /* if (M[x] == 0) */
  } /* for (x = 0; x < N; x++) */

  /* ================================================ */
  /* INITIALISATION DE LA FAH                         */
  /* ================================================ */

  FahFlush(FAH);
  FahPush(FAH, -1, 0);   /* force la creation du niveau 0 dans la Fah. */
                         /* NECESSAIRE pour eviter la creation prematuree */
                         /* de la file d'urgence */ 

  for (x = 0; x < N; x++)
  {
    if (B[x])            /* on va empiler les voisins des regions marquees */
    {
      for (k = 0; k <= 10; k += 2)
      {
        y = voisin6(x, k, rs, n, N);
        if ((y != -1) && !B[y] && !IsSet(y, EN_FAH))
        {            
          FahPush(FAH, y, SOURCE[y]);
          Set(y, EN_FAH);
#ifdef TRACEPUSHINIT
          if (trace) printf("%d: empile point %d (%d,%d) au niveau %d\n", 
                             tracedate++, y, y%rs, y/rs, SOURCE[y]);
#endif

        }
      } /* for (k = 0; k < 8; k += 2) */
    } /* if (B[x]) */
  } /* for (x = 0; x < N; x++) */

  x = FahPop(FAH);
#ifdef PARANO
  if (x != -1)
  {   
     fprintf(stderr,"lpemeyer3d() : ORDRE FIFO NON RESPECTE PAR LA FAH !!!\n");
     return(0);
  }
#endif

  /* ================================================ */
  /* INONDATION                                       */
  /* ================================================ */

  nlabels += 1;          /* cree le label pour les points de la LPE */
  while (! FahVide(FAH))
  {
    x = FahPop(FAH);
    UnSet(x, EN_FAH);

#ifdef TRACEPOP
    if (trace) 
    {
      printf("%d: POP point %d (%d,%d) ; niveau %d\n", 
              tracedate++, x, x%rs, x/rs, SOURCE[x]);
    }
#endif

    ncc = 0;
    for (k = 0; k <= 10; k += 2)
    {
      y = voisin6(x, k, rs, n, N);
      if ((y != -1) && (M[y] != 0) && (M[y] != nlabels) && NotIn(M[y], etiqcc, ncc)) 
      {
        etiqcc[ncc] = M[y];        
        ncc += 1;
      }
    } /* for k */

    if (ncc == 1)
    {
      M[x] = etiqcc[0];
#ifdef TRACEMARQUE
          if (trace) printf("%d: marque point %d (%d,%d) label %d\n", 
                             tracedate++, x, x%rs, x/rs, M[x]);
#endif
      for (k = 0; k <= 10; k += 2)
      {
        y = voisin6(x, k, rs, n, N);     
        if ((y != -1) && (M[y] == 0) && (! IsSet(y, EN_FAH)))
        {
          FahPush(FAH, y, SOURCE[y]);
          Set(y, EN_FAH);
#ifdef TRACEPUSH
          if (trace) printf("%d: empile point %d (%d,%d) au niveau %d\n", 
                             tracedate++, y, y%rs, y/rs, SOURCE[y]);
#endif
        } /* if ((y != -1) && (! IsSet(y, EN_FAH))) */
      } /* for k */
    } 
    else 
    if (ncc > 1)
    {
      M[x] = nlabels;
#ifdef TRACEMARQUE
          if (trace) printf("%d: marque point %d (%d,%d) label LPE\n", 
                             tracedate++, x, x%rs, x/rs);
#endif
    }
  } /* while (! FahVide(FAH)) */
  /* FIN PROPAGATION */

  for (x = 0; x < N; x++)
  {
    if ((M[x] == nlabels) || (M[x] == 0)) SOURCE[x] = 255; else SOURCE[x] = 0;
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahTermine(FAH);
  free(M);

  /* pour l'analyse de performances */
  save_time(N, read_chrono(&chrono1), "llpemeyer3d", image->name);    
  return(1);
}

