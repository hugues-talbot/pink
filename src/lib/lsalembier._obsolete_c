/* $Id: lsalembier.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */

OBSOLETE - voir ldynamique, lattribXXXX

/* 
   Operateurs utilisant l'arbre des composantes.
   =============================================
   (algorithme de P. Salembier)

   Operateurs : 
      lsegreconsheight (avec reconstruction basee sur la signature de l'attribut contrast)
      lsegmentheight (sans reconstruction)
      lsegmentarea
      lsegmentvol

      lareaopening
      lareaclosing

      lheightmaxima
      lvolmaxima

      lheightselnb   (selection d'un nombre donne de composantes)
      lareaselnb   (selection d'un nombre donne de composantes)
      lvolselnb   (selection d'un nombre donne de composantes)

   Michel Couprie - octobre 1999

   Decembre 2000 : l*selnb, lvolmaxima, lsegmentvol, legmentheight, lsegmentarea  
                   versions 3d (sauf lsegreconsheight)

   Juin 2001 : lheightselnb : ajout des options "min" et "max" 
               (recherche la valeur min ou max telle que le critere est satisfait)

   Decembre 2001 : ldynamique (d'apres Grimaud et L. Najman)
   Fevrier 2002 : lwshedtopo (d'apres MC, GB)
   Fevrier-juin 2002 : lwshedval (d'apres LN, MC, GB)
   Mai 2002 : nouvelles versions de l*selnb
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcfahsalembier.h>
#include <mcutil.h>
#include <mcindic.h>
#include <lsalembier.h>

#define PARANO
/*
#define VERBOSE
#define DEBUG
#define DEBUGRECONS
#define DEBUGFLOOD
*/

/*         CES FLAGS SONT POSITIONNES PAR LE MAKEFILE: 
#define ATTR_SURF
#define ATTR_HEIGHT
#define ATTR_PERIM
#define ATTR_HBORD
#define ATTR_CONTRAST
#define ATTR_VOL
#define ATTR_DYN
#define RECONS
*/

#define CONDITION_VIOLATION 0x01

#define FILTERED_OUT 0x01
#define LEAFMIN      0x02
#define LEAFMAX      0x04
#define LEAF         (LEAFMIN|LEAFMAX)
#define LCA1         0x08
#define LCA2         0x10
#define LCA          (LCA1|LCA2)

#define NOT_ANALYZED 2000000000
#define IN_THE_QUEUE 2000000001

/*
   principe de l'encodage des composantes: 
   le niveau est code dans les bits 24 a 31
   il reste donc 24 bits pour coder le numero de la composante, 
         24
   soit 2   = 16 megacomposantes par niveau.
*/

#define ENCODE(y,h) (y|(h<<24))
#define DECODENUM(y) (y&0x00ffffff)
#define DECODENIV(y) (y>>24)

/* 
   macros pour l'acces aux donnees de la structure CompactTree
*/

#define NBCOMP(h) ((h==0)?(cpct->hc[0]):(cpct->hc[h]-cpct->hc[h-1]))
#define INDEXCOMP(h,j) ((h==0)?(j):(cpct->hc[h-1]+j))
#define NBFILS(c) ((c==0)?(cpct->dfils[0]):(cpct->dfils[c]-cpct->dfils[c-1]))
#define INDEXFILS(c,j) ((c==0)?(j):(cpct->dfils[c-1]+j))

/* ============================================================================== */
/* 
  Structure de donnees pour la construction de l'arbre des composantes.

  Les sommets de cet arbre sont les composantes des coupes >=, 
  a l'exception de celles qui sont egales a une composante d'un niveau inferieur.
  Il y a donc moins de N sommets (N = nombre de pixels) et de N-1 arcs.

  Une composante (sommet) est representee par deux donnees : 
  son niveau (sur 8 bits) et son numero dans le niveau (sur 24 bits), 
  le tout encode dans un entier 32 bits.

  L'arbre est represente par
  une liste d'arcs (tableaux tete et queue), fleches de la racine vers les feuilles.
*/
/* ============================================================================== */
typedef struct {
  uint32_t nbmaxarcs;          /* nombre maximum d'arcs */
  uint32_t nbarcs;             /* nombre effectif d'arcs */
  uint32_t racine;             /* racine de l'arbre */
  uint32_t *tete;              /* sommets initiaux des arcs de l'arbre */
  uint32_t *queue;             /* sommets terminaux des arcs de l'arbre */  
#ifdef ATTR_SURF
  int32_t surf_racine;                 /* attribut surface pour la racine */
  int32_t *surf;                       /* attributs des sommets terminaux des arcs */
#endif
#ifdef ATTR_PERIM
  int32_t *perim;
#endif 
#ifdef ATTR_HBORD
  int32_t *hbord;
#endif
#ifdef ATTR_VOL
  int32_t *vol;
#endif
} CompTree;
/* ============================================================================== */

/* ============================================================================== */
/* 
  Structure de donnees compacte pour l'arbre des composantes.

  L'arbre est represente par cinq tableaux:
  - le tableau 'hc' qui au niveau de gris h, associe le nombre de composantes de 
    niveau <= h (histogramme cumule). On a donc:
      nombre de composantes de niveau 0 = hc[0]
      nombre de composantes de niveau h > 0 = hc[h] - hc[h-1]
      index derniere composante de niveau h s'il en existe = hc[h] - 1
  - le tableau 'comp' qui, a l'index i d'une composante, associe le code de la 
    composante (represente dans 32 bits comme dans la structure CompTree)
  - le tableau 'pere' qui, a l'index i d'une composante, associe l'index du pere de i
    (sauf pour la racine qui a pour index 0 et a laquelle est associee 0)
  - le tableau 'dfils' qui, a l'index i d'une composante, associe l'index(+1) 
    du dernier fils de i (sauf pour les feuilles)
  - le tableau 'fils' qui contient les listes de fils
*/
/* ============================================================================== */
#define CPCT_ROOT 0
typedef struct {
  uint32_t nbcomp;  /* nombre de composantes */
  uint32_t *comp;   /* tableau des composantes */
  uint32_t *pere;   /* tableau representant la relation 'pere' */  
  uint32_t *dfils;  /* tableau donnant l'index+1 du dernier fils dans le tableau 'fils' */
  uint32_t *fils;   /* tableau representant, avec le precedent, la relation 'fils' */  
  uint32_t *hc;     /* histogramme cumule des composantes */
#ifdef ATTR_SURF
  int32_t *surf;
#endif
#ifdef ATTR_HEIGHT
  int32_t *height;
#endif
#ifdef ATTR_PERIM
  int32_t *perim; 
#endif
#ifdef ATTR_HBORD
  int32_t *hbord;
#endif
#ifdef ATTR_CONTRAST
  double *contrast;           /* attribut flottant */
#endif
#ifdef ATTR_VOL
  int32_t *vol;
#endif
#ifdef ATTR_DYN
  int32_t *dyn;
#endif
  char *flags;          /* 8 booleens pour des usages divers */
} CompactTree;
/* ============================================================================== */


/* ======================================================================== */
/* ======================================================================== */
/* FONCTIONS POUR LE TRI */
/* ======================================================================== */
/* ======================================================================== */

// cle DOUBLE

/* =============================================================== */
static int32_t d_Partitionner(int32_t *A, double *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements q tq T[A[q]] <= T[A[p]] et les autres.
*/
{
  int32_t t;
  double x = T[A[p]];
  int32_t i = p - 1;
  int32_t j = r + 1;
  while (1)
  {
    do j--; while (T[A[j]] > x);
    do i++; while (T[A[i]] < x);
    if (i < j) { t = A[i]; A[i] = A[j]; A[j] = t; }
    else return j;
  } /* while (1) */   
} /* d_Partitionner() */

/* =============================================================== */
static int32_t d_PartitionStochastique(int32_t *A, double *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements k tels que T[A[k]] <= T[A[q]] et les autres, 
  avec q tire au hasard dans [p,r].
*/
{
  int32_t t, q;

  q = p + (rand() % (r - p + 1));
  t = A[p];         /* echange A[p] et A[q] */
  A[p] = A[q]; 
  A[q] = t;
  return d_Partitionner(A, T, p, r);
} /* d_PartitionStochastique() */

/* =============================================================== */
static void d_TriRapideStochastique (int32_t * A, double *T, int32_t p, int32_t r)
/* =============================================================== */
/* 
  trie les valeurs du tableau A de l'indice p (compris) a l'indice r (compris) 
  par ordre croissant 
*/
{
  int32_t q; 
  if (p < r)
  {
    q = d_PartitionStochastique(A, T, p, r);
    d_TriRapideStochastique (A, T, p, q) ;
    d_TriRapideStochastique (A, T, q+1, r) ;
  }
} /* d_TriRapideStochastique() */

// cle INT

/* =============================================================== */
static int32_t i_Partitionner(int32_t *A, int32_t *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements q tq T[A[q]] <= T[A[p]] et les autres.
*/
{
  int32_t t;
  int32_t x = T[A[p]];
  int32_t i = p - 1;
  int32_t j = r + 1;
  while (1)
  {
    do j--; while (T[A[j]] > x);
    do i++; while (T[A[i]] < x);
    if (i < j) { t = A[i]; A[i] = A[j]; A[j] = t; }
    else return j;
  } /* while (1) */   
} /* i_Partitionner() */

/* =============================================================== */
static int32_t i_PartitionStochastique(int32_t *A, int32_t *T, int32_t p, int32_t r)
/* =============================================================== */
/*
  partitionne les elements de A entre l'indice p (compris) et l'indice r (compris)
  en deux groupes : les elements k tels que T[A[k]] <= T[A[q]] et les autres, 
  avec q tire au hasard dans [p,r].
*/
{
  int32_t t, q;

  q = p + (rand() % (r - p + 1));
  t = A[p];         /* echange A[p] et A[q] */
  A[p] = A[q]; 
  A[q] = t;
  return i_Partitionner(A, T, p, r);
} /* i_PartitionStochastique() */

/* =============================================================== */
static void i_TriRapideStochastique (int32_t * A, int32_t *T, int32_t p, int32_t r)
/* =============================================================== */
/* 
  trie les valeurs du tableau A de l'indice p (compris) a l'indice r (compris) 
  par ordre croissant 
*/
{
  int32_t q; 
  if (p < r)
  {
    q = i_PartitionStochastique(A, T, p, r);
    i_TriRapideStochastique (A, T, p, q) ;
    i_TriRapideStochastique (A, T, q+1, r) ;
  }
} /* i_TriRapideStochastique() */

/* ======================================================================== */
/* ======================================================================== */
/* CONSTRUCTION ET GESTION DE L'ARBRE DES COMPOSANTES */
/* ======================================================================== */
/* ======================================================================== */

/* ==================================== */
CompTree * InitCompTree(int32_t nbmaxarcs)
/* ==================================== */
{
  CompTree *ct;
  ct = (CompTree *)calloc(1,sizeof(CompTree));
  if (ct == NULL) return NULL;
  ct->tete = (uint32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->tete == NULL) return NULL;
  ct->queue = (uint32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->queue == NULL) return NULL;
#ifdef ATTR_SURF
  ct->surf = (int32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->surf == NULL) return NULL;
#endif
#ifdef ATTR_PERIM
  ct->perim = (int32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->perim == NULL) return NULL;
#endif
#ifdef ATTR_HBORD
  ct->hbord = (int32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->hbord == NULL) return NULL;
#endif
#ifdef ATTR_VOL
  ct->vol = (int32_t *)calloc(1,nbmaxarcs * sizeof(int32_t));
  if (ct->vol == NULL) return NULL;
#endif
  ct->nbmaxarcs = nbmaxarcs;
  ct->nbarcs = 0;
  ct->racine = CPCT_ROOT;
  return ct;
} /* InitCompTree() */

/* ==================================== */
void TermineCompTree(CompTree *ct)
/* ==================================== */
{
  free(ct->tete);
  free(ct->queue);
#ifdef ATTR_SURF
  free(ct->surf);
#endif
#ifdef ATTR_PERIM
  free(ct->perim);
#endif
#ifdef ATTR_HBORD
  free(ct->hbord);
#endif
#ifdef ATTR_VOL
  free(ct->vol);
#endif
  free(ct);
} /* TermineCompTree() */

/* ==================================== */
CompactTree * CompTree2CompactTree(CompTree *ct, uint32_t *number_nodes)
/* ==================================== */
/* ATTENTION EFFET DE BORD : DETRUIT LA RELATION number_nodes */
{
  CompactTree *cpct;
  uint32_t i, n, h, t, th, tn, q, qh, qn;
  uint32_t nbcomp = ct->nbarcs + 1;
  uint32_t *nfils;

  cpct = (CompactTree *)calloc(1,sizeof(CompactTree));
  if (cpct == NULL) return NULL;
  cpct->nbcomp = nbcomp;
  cpct->comp = (uint32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->comp == NULL) return NULL;
  cpct->pere = (uint32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->pere == NULL) return NULL;
  cpct->dfils = (uint32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->dfils == NULL) return NULL;
  cpct->fils = (uint32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->fils == NULL) return NULL;
  cpct->hc = (uint32_t *)calloc(1,256 * sizeof(int32_t));
  if (cpct->hc == NULL) return NULL;
#ifdef ATTR_SURF
  cpct->surf = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->surf == NULL) return NULL;
#endif
#ifdef ATTR_HEIGHT
  cpct->height = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->height == NULL) return NULL;
#endif
#ifdef ATTR_PERIM
  cpct->perim = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->perim == NULL) return NULL;
#endif
#ifdef ATTR_HBORD
  cpct->hbord = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->hbord == NULL) return NULL;
#endif
#ifdef ATTR_CONTRAST
  cpct->contrast = (double *)calloc(1,nbcomp * sizeof(double));
  if (cpct->contrast == NULL) return NULL;
#endif
#ifdef ATTR_VOL
  cpct->vol = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->vol == NULL) return NULL;
#endif
#ifdef ATTR_DYN
  cpct->dyn = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  if (cpct->dyn == NULL) return NULL;
#endif
  cpct->flags = (char *)calloc(nbcomp, sizeof(char));
  if (cpct->flags == NULL) return NULL;

  /* calcule l'histogramme cumule hc */
  n = cpct->hc[0] = number_nodes[0]; 
  for (i = 1; i < 256; i++) { n += number_nodes[i]; cpct->hc[i] = n; }

  /* construit le tableau des composantes comp */
  n = 0; h = 0; while (!number_nodes[h]) h++; /* ATTENTION CODE FRAGILE */
  for (i = 0; i < nbcomp; i++)                /* SUPPOSE CORRECTES LES DONNEES D'ENTREE */
  {                                           /* PAS DE VERIFICATION */
    cpct->comp[i] = ENCODE(n,h); 
    number_nodes[h]--; n++;
    if (!number_nodes[h]) { n = 0; while (!number_nodes[h]) h++; }
  } /* for i */
  
  /* construit la relation pere */
  for (i = 0; i < nbcomp-1; i++)
  {
    t = ct->tete[i]; q = ct->queue[i];
    th = DECODENIV(t); tn = DECODENUM(t);
    qh = DECODENIV(q); qn = DECODENUM(q);
    cpct->pere[INDEXCOMP(qh,qn)] = INDEXCOMP(th,tn);
  }
  q = ct->racine; qh = DECODENIV(q); qn = DECODENUM(q);
  cpct->pere[INDEXCOMP(qh,qn)] = INDEXCOMP(qh,qn);

  /* construit la relation dfils et fils */
  nfils = (uint32_t *)calloc(nbcomp, sizeof(int32_t));
  if (nfils == NULL) return NULL;
  for (i = 1; i < nbcomp; i++) nfils[cpct->pere[i]] += 1;
    /* exception : la racine (0) est fille d'elle-meme, cette relation n'est pas comptee */ 
  cpct->dfils[CPCT_ROOT] = nfils[CPCT_ROOT];
  for (i = 1; i < nbcomp; i++) cpct->dfils[i] = cpct->dfils[i - 1] + nfils[i];
  for (i = 1; i < nbcomp; i++)
  {
    t = cpct->pere[i]; /* i est fils de t */
    nfils[t] -= 1;
    cpct->fils[INDEXFILS(t,nfils[t])] = i;
  }
  free(nfils);

  /* transfere les attributs (cas particulier pour la racine) */
  for (i = 0; i < nbcomp-1; i++)
  {
    q = ct->queue[i]; qh = DECODENIV(q); qn = DECODENUM(q);
#ifdef ATTR_SURF
    cpct->surf[INDEXCOMP(qh,qn)] = ct->surf[i];
#endif
#ifdef ATTR_PERIM
    cpct->perim[INDEXCOMP(qh,qn)] = ct->perim[i];
#endif
#ifdef ATTR_HBORD
    cpct->hbord[INDEXCOMP(qh,qn)] = ct->hbord[i];
#endif
#ifdef ATTR_VOL
    cpct->vol[INDEXCOMP(qh,qn)] = ct->vol[i];
#endif
  }
#ifdef ATTR_SURF
  cpct->surf[CPCT_ROOT] = ct->surf_racine;
#endif
#ifdef ATTR_PERIM
  cpct->perim[CPCT_ROOT] = 0;
#endif
#ifdef ATTR_HBORD
  cpct->hbord[CPCT_ROOT] = 0;
#endif
#ifdef ATTR_VOL
  cpct->vol[CPCT_ROOT] = 0;
#endif  
  return cpct;
} /* CompTree2CompactTree() */

/* ==================================== */
void ReInitFlags(CompactTree * cpct)
/* ==================================== */
{
  uint32_t nbcomp = cpct->nbcomp;
  memset(cpct->flags,0,nbcomp);
} /* ReInitFlags() */

#ifdef ATTR_SURF
/* ==================================== */
int32_t surfrec(CompactTree * cpct, uint32_t som, int32_t *na1)
/* ==================================== */
/*
   Calcule la surface de chacune des composantes, a partir de 
   l'information stockee dans cpct->surf[i], qui correspond a la 
   difference de surface entre la composante i et ses filles.
   Le resultat est provisoirement stocke dans le tableau na1 (indexe par
   le numero de composante i), pour etre ensuite recopie dans cpct->surf[i].
*/ 
{
  int32_t i, n, j;
  n = NBFILS(som);
  if (n == 0) return na1[som] = cpct->surf[som];
  na1[som] = cpct->surf[som];
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    na1[som] += surfrec(cpct, j, na1);
  }
  return na1[som];
} /* surfrec() */
#endif

#ifdef ATTR_VOL
/* ==================================== */
int32_t volrec(CompactTree * cpct, uint32_t som, int32_t *na1)
/* ==================================== */
/*
   Attention: pour utiliser cette fonction, il faut avoir 
   prealablement calcule la surface des composantes (cf. surfrec())
   et avoir stocke cette information dans cpct->surf[].

   Calcule le volume de chacune des composantes, a partir de 
   l'information stockee dans cpct->surf[i], qui correspond a la 
   surface de la composante.
   Le resultat est provisoirement stocke dans le tableau na1 (indexe par
   le numero de composante i), pour etre ensuite recopie dans cpct->vol[i].
*/ 
{
  int32_t i, n, j;
  n = NBFILS(som);
  if (n == 0) return na1[som] = cpct->surf[som];
  na1[som] = cpct->surf[som];
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    na1[som] += volrec(cpct, j, na1);
  }
  return na1[som];
} /* volrec() */
#endif

#ifdef ATTR_HEIGHT
/* ==================================== */
int32_t heightrec(CompactTree * cpct, uint32_t som, int32_t *na1)
/* ==================================== */
/* retourne le niveau max des descendants de som (ou le niveau de som pour une feuille) */ 
{
  int32_t i, n, j, h;
  n = NBFILS(som);
  if (n == 0) return na1[som] = DECODENIV(cpct->comp[som]);
  na1[som] = 0;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    h = heightrec(cpct, j, na1);
    na1[som] = max(na1[som], h);
  }
  return na1[som];
} /* heightrec() */
#endif

#ifdef ATTR_PERIM
/* ==================================== */
int32_t perimrec(CompactTree * cpct, uint32_t som, int32_t *nperim)
/* ==================================== */
{
  int32_t i, n, j;
  n = NBFILS(som);
  if (n == 0) return nperim[som] = cpct->perim[som];
  nperim[som] = cpct->perim[som];
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    nperim[som] += perimrec(cpct, j, nperim);
  }
  return nperim[som];
} /* perimrec() */
#endif

#ifdef ATTR_HBORD
/* ==================================== */
int32_t hbordrec(CompactTree * cpct, uint32_t som, int32_t *nhbord)
/* ==================================== */
{
  int32_t i, n, j;
  n = NBFILS(som);
  if (n == 0) return nhbord[som] = cpct->hbord[som];
  nhbord[som] = cpct->hbord[som];
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    nhbord[som] += hbordrec(cpct, j, nhbord);
  }
  return nhbord[som];
} /* hbordrec() */
#endif

/* ==================================== */
void CalculeAttributs(CompactTree * cpct)
/* ==================================== */
{
  int32_t *na1, *na2, i;
  uint32_t nbcomp = cpct->nbcomp;

  na1 = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
  na2 = (int32_t *)calloc(1,nbcomp * sizeof(int32_t));
#ifdef ATTR_SURF
  (void)surfrec(cpct, 0, na1);
  for (i = 0; i < nbcomp; i++) cpct->surf[i] = na1[i];
#endif
#ifdef ATTR_VOL
  if (cpct->surf == NULL)
  {
    fprintf(stderr, "CalculeAttributs: VOL ne peut etre calcule dans SURF\n");
    exit(0);
  }
  (void)volrec(cpct, 0, na1);
  for (i = 0; i < nbcomp; i++) cpct->vol[i] = na1[i];
#endif
#ifdef ATTR_HEIGHT

  (void)heightrec(cpct, 0, na1);
  /* pour la mesure de la hauteur, il faut rajouter la difference de niveau avec le pere */
  for (i = 1; i < nbcomp; i++) cpct->height[i] = na1[i] 
	     /* - DECODENIV(cpct->comp[i]) + DECODENIV(cpct->comp[i]) */ /* inutile */
                                               - DECODENIV(cpct->comp[cpct->pere[i]]) - 1;

  cpct->height[0] = NDG_MAX - NDG_MIN;
#endif
#ifdef ATTR_PERIM
  (void)perimrec(cpct, 0, na1);
  for (i = 0; i < nbcomp; i++) cpct->perim[i] = na1[i]; 
#endif
#ifdef ATTR_HBORD
  (void)hbordrec(cpct, 0, na2);
  for (i = 0; i < nbcomp; i++) cpct->hbord[i] = na2[i];  
#endif
#ifdef ATTR_CONTRAST
  for (i = 0; i < nbcomp; i++) cpct->contrast[i] = ((double)(na2[i]))/na1[i];  
#endif
  free(na1);
  free(na2);
} /* CalculeAttributs() */

#ifdef ATTR_HEIGHT
/* ==================================== */
int32_t FiltreHeightRec(CompactTree * cpct, int32_t som, int32_t h)
/* ==================================== */
/*
  Filtre les sommets de l'arbre selon un critere de hauteur : 

     height(som) >= h

  Un sommet 'som' ne satisfaisant pas le critere est marque FILTERED_OUT.
  La fonction traite recursivement les fils, et retourne le nombre NNM de sommets non marques dans
    la descendance (inclus le sommet lui-meme).
  Un sommet non filtre et dont le NNM de la descendance vaut 0 est marque LEAFMIN. 
*/
{
  int32_t i, n, j, NNM = 0;
  n = NBFILS(som);
  if (cpct->height[som] < h) cpct->flags[som] |= FILTERED_OUT;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    NNM += FiltreHeightRec(cpct, j, h);
  }
  if (cpct->height[som] >= h)           /* sommet non filtre */
  {
    if (NNM == 0) cpct->flags[som] |= LEAFMIN;
    NNM++;
  }
  return NNM;
} /* FiltreHeightRec() */
#endif

#ifdef ATTR_SURF
/* ==================================== */
int32_t FiltreSurfRec(CompactTree * cpct, int32_t som, int32_t h)
/* ==================================== */
/*
  Filtre les sommets de l'arbre selon un critere de surface : 

     surf(som) >= h

  Un sommet 'som' ne satisfaisant pas le critere est marque FILTERED_OUT.
  La fonction traite recursivement les fils, et retourne le nombre NNM de sommets non marques dans
    la descendance (inclus le sommet lui-meme).
  Un sommet non filtre et dont le NNM de la descendance vaut 0 est marque LEAFMIN. 
*/
{
  int32_t i, n, j, NNM = 0;
  n = NBFILS(som);
  if (cpct->surf[som] < h) cpct->flags[som] |= FILTERED_OUT;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    NNM += FiltreSurfRec(cpct, j, h);
  }
  if (cpct->surf[som] >= h)           /* sommet non filtre */
  {
    if (NNM == 0) cpct->flags[som] |= LEAFMIN;
    NNM++;
  }
  return NNM;
} /* FiltreSurfRec() */
#endif

#ifdef ATTR_VOL
/* ==================================== */
int32_t FiltreVolRec(CompactTree * cpct, int32_t som, int32_t h)
/* ==================================== */
/*
  Filtre les sommets de l'arbre selon un critere de volume : 

     vol(som) >= h

  Un sommet 'som' ne satisfaisant pas le critere est marque FILTERED_OUT.
  La fonction traite recursivement les fils, et retourne le nombre NNM de sommets non marques dans
    la descendance (inclus le sommet lui-meme).
  Un sommet non filtre et dont le NNM de la descendance vaut 0 est marque LEAFMIN. 
*/
{
  int32_t i, n, j, NNM = 0;
  n = NBFILS(som);
  if (cpct->vol[som] < h) cpct->flags[som] |= FILTERED_OUT;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    NNM += FiltreVolRec(cpct, j, h);
  }
  if (cpct->vol[som] >= h)           /* sommet non filtre */
  {
    if (NNM == 0) cpct->flags[som] |= LEAFMIN;
    NNM++;
  }
  return NNM;
} /* FiltreVolRec() */
#endif

/* ==================================== */
int32_t MaximiseSegmentation(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  
*/
{
  int32_t i, n, j, f, nf, NF = 0;
  if (cpct->flags[som] & FILTERED_OUT) return 0;
  n = NBFILS(som);
  if (n == 0) return 1;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    if (nf = MaximiseSegmentation(cpct, j)) { f = j; NF += nf; }
  }
  if (NF == 0) return 1;
  if (NF == 1)
  {
    cpct->flags[f] |= FILTERED_OUT;
    cpct->flags[som] |= LEAFMAX;
    return 1;
  }
  return 1 + NF;
} /*  MaximiseSegmentation() */

#ifdef ATTR_CONTRAST
/* ==================================== */
void Reconstruction(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  Recherche a partir de la racine, les sommets marques LEAF.
  A partir de chacun de ces sommets :
    - remonte en suivant les etiquettes LEAFMAX jusqu'a trouver un sommet marque LEAFMIN,
    - stocke dans 'branche' le chemin (liste de sommets), dans 'contrast' l'attribut contrast associe a chaque sommet,
      et dans 'index' les index des sommets (initialises a 0,1,2,3,...)
    - trie le tableau index sur la cle contraste,
    - selectionne un sommet M maximum (si plusieurs, ...),
    - a partir de M, on "redescend" en demarquant les sommets
*/
{
  int32_t i, n, m, j, k, M;
  double contrast[256];
  int32_t branche[256];
  int32_t index[256];

  if (!(cpct->flags[som] & LEAF)) /* remonte l'arbre pour trouver une LEAF */ 
  {
    n = NBFILS(som);
    for (i = 0; i < n; i++) 
    {
      j = INDEXFILS(som, i);
      j = cpct->fils[j];
      Reconstruction(cpct, j);
    }
  }
  else /* on a trouve une LEAF */
  {
    m = 0;  /* m indexe le tableau contrast */
    k = som;
    while (!(cpct->flags[k] & LEAFMIN))
    {
      contrast[m] = cpct->contrast[k];
      branche[m] = k;
      m++;
      n = NBFILS(k); /* on va chercher le fils qui est marque LEAF */
      for (i = 0; i < n; i++) 
      {
        j = INDEXFILS(k, i);
        j = cpct->fils[j];
        if (cpct->flags[j] & LEAF) break;
      }
      k = j;
#ifdef PARANO
      if (i >= n) fprintf(stderr, "Reconstruction : ERREUR INATTENDUE\n");
#endif
    }
    contrast[m] = cpct->contrast[k];
    branche[m] = k;
    m++;    

#ifdef DEBUGRECONS
    printf("Reconstruction Sommet %d\n", som);
    for (i = 0; i < m; i++) printf("%d %d %g\n", i, branche[i], contrast[i]);
#endif

    /* trie le tableau index sur la cle contraste */
    for (i = 0; i < m; i++) index[i] = i;
    d_TriRapideStochastique (index, contrast, 0, m-1);

#ifdef DEBUGRECONS
    printf("Apres tri : \n");
    for (i = 0; i < m; i++) printf("%d %d %g\n", index[i], branche[index[i]], contrast[index[i]]);
#endif

    /* selectionne un sommet M maximum (SI PLUSIEURS, ... CHOIX ARBITRAIRE), */
    M = branche[index[m-1]];

    /* a partir de M, on "redescend" en demarquant les sommets */
    cpct->flags[M] &= ~FILTERED_OUT;
    cpct->flags[M] |= LEAFMAX;
    k = cpct->pere[M];
    while (cpct->flags[k] & LEAFMAX)
    {
      cpct->flags[k] &= ~LEAFMAX;
      cpct->flags[k] &= ~FILTERED_OUT;
      k = cpct->pere[k];
    }
  } /* else (on a trouve une LEAF) */
} /*  Reconstruction() */
#endif

/* ==================================== */
int32_t NbLeafs(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  Recherche a partir du sommet som, les sommets marques LEAF.
  Retourne le nombre de ces sommets.
*/
{
  int32_t i, j, k, n;

  if (!(cpct->flags[som] & LEAF)) /* remonte l'arbre pour trouver une LEAF */ 
  {
    n = NBFILS(som);
    k = 0; 
    for (i = 0; i < n; i++) 
    {
      j = INDEXFILS(som, i);
      j = cpct->fils[j];
      k += NbLeafs(cpct, j);
    }
    return k;
  }
  else /* (on a trouve une LEAF) */
    return 1;
} /* NbLeafs() */

/* ==================================== */
void RecupereImageFiltree(CompactTree * cpct,           
       uint32_t *STATUS,
       int32_t rs, int32_t N, 
       uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t i, h;
  uint32_t c, comp;
  for (i = 0; i < N; i++) 
  {
    h = ORI[i];
    c = STATUS[i];
    comp = INDEXCOMP(h,c);
    while (cpct->flags[comp] == FILTERED_OUT) comp = cpct->pere[comp];
    ORI[i] = DECODENIV(cpct->comp[comp]);
  }  
} /* RecupereImageFiltree() */

/* ==================================== */
void RecupereSegmentation(CompactTree * cpct,           
       uint32_t *STATUS,
       int32_t rs, int32_t N, 
       uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t i, h;
  uint32_t c, comp;
  for (i = 0; i < N; i++) 
  {
    h = ORI[i];
    c = STATUS[i];
    comp = INDEXCOMP(h,c);
    while (cpct->flags[comp] & FILTERED_OUT) 
    {
#ifdef PARANO
      if (comp == cpct->pere[comp])       
        fprintf(stderr, "RecupereSegmentation: la racine a ete eliminee\n");
#endif
      comp = cpct->pere[comp];
    }
    if (cpct->flags[comp] & LEAF) ORI[i] = NDG_MAX; else ORI[i] = NDG_MIN;
  }  
} /* RecupereSegmentation() */

/* ==================================== */
void TermineCompactTree(CompactTree *cpct)
/* ==================================== */
{
  free(cpct->comp);
  free(cpct->pere);
  free(cpct->dfils);
  free(cpct->fils);
  free(cpct->hc);
#ifdef ATTR_SURF
  free(cpct->surf);
#endif
#ifdef ATTR_HEIGHT
  free(cpct->height);
#endif
#ifdef ATTR_PERIM
  free(cpct->perim);
#endif
#ifdef ATTR_HBORD
  free(cpct->hbord);
#endif
#ifdef ATTR_CONTRAST
  free(cpct->contrast);
#endif
#ifdef ATTR_VOL
  free(cpct->vol);
#endif
#ifdef ATTR_DYN
  free(cpct->dyn);
#endif

  free(cpct->flags);
  free(cpct);
} /* TermineCompactTree() */

/* ==================================== */
void AfficheCompTree(CompTree *ct)
/* ==================================== */
{
  int32_t i;
  printf("===========================\n");
  printf("nombre max arcs = %d\n", ct->nbmaxarcs);
  printf("nombre arcs = %d\n", ct->nbarcs);
  printf("racine = C%d,%d\n", DECODENIV(ct->racine), DECODENUM(ct->racine));
  printf("===========================\n");
  for (i = 0; i < ct->nbarcs; i++)
    printf("C%d,%d --> C%d,%d\n", 
           DECODENIV(ct->tete[i]), DECODENUM(ct->tete[i]), 
           DECODENIV(ct->queue[i]), DECODENUM(ct->queue[i]));
  printf("===========================\n");
} /* AfficheCompTree() */

/* ==================================== */
void AfficheCompactTree(CompactTree *cpct)
/* ==================================== */
{
  uint32_t i, j, n, f;
  printf("===========================\n");
  printf("nombre composantes = %d\n", cpct->nbcomp);
  printf("===========================\n");
  for (i = 0; i < cpct->nbcomp; i++)
  {
    printf("comp[%d] = C%d,%d ", i, DECODENIV(cpct->comp[i]), DECODENUM(cpct->comp[i]));
#ifdef ATTR_SURF
    printf("surf = %d  ", cpct->surf[i]);
#endif
#ifdef ATTR_HEIGHT
    printf("height = %d  ", cpct->height[i]);
#endif
#ifdef ATTR_VOL
    printf("vol = %d  ", cpct->vol[i]);
#endif
#ifdef ATTR_PERIM
    printf("perim = %d  ", cpct->perim[i]);
#endif
#ifdef ATTR_HBORD
    printf("hbord = %d  ", cpct->hbord[i]);
#endif
#ifdef ATTR_CONTRAST
    printf("contrast = %g ",cpct->contrast[i]);
#endif
#ifdef ATTR_DYN
    printf("dynamique = %d ",cpct->dyn[i]);
#endif
    printf("fils = [ ");
    n = NBFILS(i);
    for (j = 0; j < n; j++)
    {
      f = INDEXFILS(i,j);
      f = cpct->fils[f];
      printf("C%d,%d ", DECODENIV(cpct->comp[f]), DECODENUM(cpct->comp[f]), f);
    }
    printf("] ");
    if (cpct->flags[i] & FILTERED_OUT) printf(" - OUT");
    if (cpct->flags[i] & LEAFMIN) printf(" - LEAFMIN");
    if (cpct->flags[i] & LEAFMAX) printf(" - LEAFMAX");
    printf("\n");
  }
  printf("===========================\n");
  for (i = 0; i < cpct->nbcomp; i++)
    printf("pere[%d] = %d\n", i, cpct->pere[i]);
  printf("===========================\n");
} /* AfficheCompactTree() */

/* ==================================== */
void AfficheImaComp(CompactTree * cpct,           
       uint32_t *STATUS,
       int32_t rs, int32_t N, 
       uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t i, h;
  uint32_t c, comp;
  for (i = 0; i < N; i++) 
  {
    h = ORI[i];
    c = STATUS[i];
    printf("C%d,%d\t", h, c);
    if ((i % rs) == (rs - 1)) printf("\n");
  }  
} /* AfficheImaComp() */

/* ==================================== */
void WriteCompactTree(CompactTree *cpct, char * filename)
/* ==================================== */
{
  uint32_t i, j, n, f;
  FILE * fd = NULL;
  char buf[256];

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "WriteCompactTree: cannot open file: %s\n", filename);
    exit(0);
  }

  fprintf(fd, "%d %d\n", cpct->nbcomp, cpct->nbcomp-1);
  fprintf(fd, "noms sommets\n");
  for (i = 0; i < cpct->nbcomp; i++)
  {
    sprintf(buf, "C%d,%d", DECODENIV(cpct->comp[i]), DECODENUM(cpct->comp[i]));
    fprintf(fd, "%d %s\n", i, buf);
  }
  fprintf(fd, "arcs\n");
  for (i = 1; i < cpct->nbcomp; i++)
    fprintf(fd, "%d %d\n", cpct->pere[i], i);
  fclose(fd);
} /* WriteCompactTree() */

/* ==================================== */
int32_t LeafCount(CompactTree *cpct)
/* ==================================== */
{
  uint32_t i, f = 0;
  for (i = 0; i < cpct->nbcomp; i++)
    if ((NBFILS(i)) == 0) f++;
  return f;
} /* LeafCount() */

/* ==================================== */
int32_t LeafMark(CompactTree *cpct)
/* ==================================== */
{
  uint32_t i, f = 0;
  for (i = 0; i < cpct->nbcomp; i++)
    if ((NBFILS(i)) == 0) { f++; cpct->flags[i] |= LEAF; }
  return f;
} /* LeafMark() */

/* ==================================== */
int32_t NbFilsNonFiltres(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  retourne le nombre de fils de 'som' non marques FILTERED_OUT.
*/
{
  int32_t i, n, j, NNM = 0;
  n = NBFILS(som);
  if (n == 0) 
    return 0;
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(som, i);
    j = cpct->fils[j];
    if (!(cpct->flags[j] & FILTERED_OUT)) NNM++;
    else if (NbFilsNonFiltres(cpct, j)>=1) NNM++;
  }
  return NNM;
} /* NbFilsNonFiltres() */

/* ==================================== */
void AjouteArc(CompTree *ct, uint32_t i, uint32_t j 
#ifdef ATTR_SURF
               , int32_t surf
#endif
#ifdef ATTR_PERIM
               , int32_t perim
#endif
#ifdef ATTR_HBORD
               , int32_t hbord
#endif
              )
/* ==================================== */
{
  if (ct->nbarcs >= ct->nbmaxarcs)
  {
    fprintf(stderr, "AjouteArc : structure CompTree saturee (%d arcs)\n", ct->nbarcs);
    exit(0);
  }
  ct->tete[ct->nbarcs] = i;
  ct->queue[ct->nbarcs] = j;
#ifdef ATTR_SURF
  ct->surf[ct->nbarcs] = surf;
#endif
#ifdef ATTR_PERIM
  ct->perim[ct->nbarcs] = perim;
#endif
#ifdef ATTR_HBORD
  ct->hbord[ct->nbarcs] = hbord;
#endif
  ct->nbarcs += 1;
} /* AjouteArc() */

/* ==================================== */
int32_t contrib_perim(int32_t p, uint8_t *ORI, uint32_t *STATUS, int32_t rs, int32_t N, int32_t incr_vois)
/* ==================================== */
{ /* calcule la contribution du point p au perimetre de la composante */
  int32_t q, k, nv = 0;

  for (k = 0; k < 8; k += incr_vois) /* compte le nombre nv de voisins deja traites */
  {
    q = voisin(p, k, rs, N);
    if ((q != -1) && (ORI[q] >= ORI[p]) && (STATUS[q] < NOT_ANALYZED)) nv++;
  } /* for (k = 0; k < 8; k += incr_vois) */

  return 4 - 2 * nv;
}

/* ==================================== */
int32_t contrib_hbord(int32_t p, uint8_t *ORI, uint32_t *STATUS, int32_t rs, int32_t N, int32_t incr_vois)
/* ==================================== */
{ /* calcule la contribution du point p au hbord de la composante */
  int32_t q, k, h = 0;

  for (k = 0; k < 8; k += incr_vois)
  {
    q = voisin(p, k, rs, N);
    if (q != -1) h += (ORI[p] - ORI[q]);
  } /* for (k = 0; k < 8; k += incr_vois) */

  return h;
}

/* ==================================== */
int32_t flood(int32_t h,                        /* niveau a inonder */
          Fah *FAH, 
          uint32_t *STATUS,         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
	                                /* en sortie, contient le numero de la comp. de niveau h */
                                        /* qui contient le pixel */
          uint32_t *number_nodes,   /* nombre de composantes par niveau */
          uint8_t *node_at_level, /* tableau de booleens */
          CompTree * tree,              /* l'arbre en construction */
          int32_t incr_vois,                /* = 1 pour la 8-connexite, 
                                           = 2 pour la 4-connexite */ 
          int32_t rs, int32_t N, 
          uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t p, q, k, m, i, j;
#ifdef ATTR_SURF
  int32_t surf = 0;
#endif
#ifdef ATTR_PERIM
  int32_t perim = 0;
#endif
#ifdef ATTR_HBORD
  int32_t hbord = 0;
#endif

#ifdef DEBUGFLOOD
  printf("debut flood niveau %d\n", h);
#endif

  node_at_level[h] = 1; /* CORRECTION BUG: LIGNE AJOUTEE LE 02/08/00 */
  while (!FahVideNiveau(FAH, h))               /* first step : propagation */
  {                                            /* ======================== */
    p = FahPopNiveau(FAH, h);
    STATUS[p] = number_nodes[h];
#ifdef DEBUGFLOOD
    printf("STATUS[p] %d\n", STATUS[p]);
#endif
    for (k = 0; k < 8; k += incr_vois)
    {
      q = voisin(p, k, rs, N);
      if ((q != -1) && (STATUS[q] == NOT_ANALYZED))
      {
        FahPush(FAH, q, ORI[q]);
        STATUS[q] = IN_THE_QUEUE;
        node_at_level[ORI[q]] = 1;
        if (ORI[q] > ORI[p])
	{
          m = ORI[q];
          do 
          {
#ifdef PARANO
            if ((m < 0) || (m > 255))
            {
              fprintf(stderr, "flood: mauvais niveau : %d ; ORI[q] = %d\n", m, ORI[q]);
              exit(0);
            }
#endif
            m = flood(m, FAH, STATUS, number_nodes, node_at_level, tree, incr_vois, rs, N, ORI); 
          } while ((m != h) && (m >= 0));
	} /* if (ORI[q] > ORI[p]) */
      } /* if ((q != -1) && (STATUS[q] == NOT_ANALYZED)) */
    } /* for (k = 0; k < 8; k += incr_vois) */
#ifdef ATTR_SURF
    surf += 1;
#endif
#ifdef ATTR_PERIM
    perim += contrib_perim(p, ORI, STATUS, rs, N, incr_vois);
#endif
#ifdef ATTR_HBORD
    hbord += contrib_hbord(p, ORI, STATUS, rs, N, incr_vois);
#endif
  } /* while (!FahVideNiveau(FAH, h)) */
#ifdef DEBUGFLOOD
  printf("retour flood niveau %d\n", h);
#endif
  number_nodes[h] += 1;

  m = h - 1;                                    /* second step : define the father */
  while ((m >= 0) && (!node_at_level[m])) m--;  /* =============================== */
  i = number_nodes[h] - 1;
  if (m >= 0)
  {
    j = number_nodes[m];
#ifdef DEBUGFLOOD
    printf("AjouteArc (%d %d) (%d %d)\n", m, j, h, i);
#endif
    AjouteArc(tree, ENCODE(j,m), ENCODE(i,h)    /* definit Cm,j comme le pere de Ch,i */
#ifdef ATTR_SURF
              , surf
#endif
#ifdef ATTR_PERIM
              , perim
#endif
#ifdef ATTR_HBORD
              , hbord
#endif
             ); 
  } /* if (m >= 0) */
  else
  {
#ifdef DEBUGFLOOD
    printf("DefinitRacine (%d %d)\n", h, i);
#endif
    tree->racine = ENCODE(i,h);                /* Ch,i est racine */
#ifdef ATTR_SURF
    tree->surf_racine = surf;
#endif

  }
  node_at_level[h] = 0;
#ifdef DEBUGFLOOD
  printf("fin flood niveau %d, retourne %d\n", h, m);
#endif
  return m;
} /* flood() */

/* ==================================== */
int32_t flood3d(
          int32_t h,                        /* niveau a inonder */
          Fah *FAH, 
          uint32_t *STATUS,         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
	                                /* en sortie, contient le numero de la comp. de niveau h */
                                        /* qui contient le pixel */
          uint32_t *number_nodes,   /* nombre de composantes par niveau */
          uint8_t *node_at_level, /* tableau de booleens */
          CompTree * tree,              /* l'arbre en construction */
          int32_t connex,
          int32_t rs, int32_t ps, int32_t N, 
          uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t p, q, k, m, i, j;
#ifdef ATTR_SURF
  int32_t surf = 0;
#endif
#ifdef ATTR_PERIM
  int32_t perim = 0;
#endif
#ifdef ATTR_HBORD
  int32_t hbord = 0;
#endif

#ifdef DEBUGFLOOD
  printf("debut flood3d niveau %d\n", h);
#endif

  node_at_level[h] = 1; /* CORRECTION BUG: LIGNE AJOUTEE LE 02/08/00 */
  while (!FahVideNiveau(FAH, h))               /* first step : propagation */
  {                                            /* ======================== */
    p = FahPopNiveau(FAH, h);
    STATUS[p] = number_nodes[h];
#ifdef DEBUGFLOOD
    printf("STATUS[p] %d\n", STATUS[p]);
#endif

    switch (connex)
    {
      case 6:
        for (k = 0; k <= 10; k += 2)   /* parcourt les 6 voisins */
        {
          q = voisin6(p, k, rs, ps, N);
          if ((q != -1) && (STATUS[q] == NOT_ANALYZED))
          {
            FahPush(FAH, q, ORI[q]);
            STATUS[q] = IN_THE_QUEUE;
            node_at_level[ORI[q]] = 1;
            if (ORI[q] > ORI[p])
	    {
              m = ORI[q];
              do 
              {
#ifdef PARANO
                if ((m < 0) || (m > 255))
                {
                  fprintf(stderr, "flood3d: mauvais niveau : %d ; ORI[q] = %d\n", m, ORI[q]);
                  exit(0);
                }
#endif
                m = flood3d(m, FAH, STATUS, number_nodes, node_at_level, tree, connex, rs, ps, N, ORI); 
              } while ((m != h) && (m >= 0));
	    } /* if (ORI[q] > ORI[p]) */
          } /* if ((q != -1) && (STATUS[q] == NOT_ANALYZED)) */
        } /* for (...) */
        break;
      case 18:
        for (k = 0; k < 18; k++)   /* parcourt les 18 voisins */
        {
          q = voisin18(p, k, rs, ps, N);
          if ((q != -1) && (STATUS[q] == NOT_ANALYZED))
          {
            FahPush(FAH, q, ORI[q]);
            STATUS[q] = IN_THE_QUEUE;
            node_at_level[ORI[q]] = 1;
            if (ORI[q] > ORI[p])
	    {
              m = ORI[q];
              do 
              {
#ifdef PARANO
                if ((m < 0) || (m > 255))
                {
                  fprintf(stderr, "flood3d: mauvais niveau : %d ; ORI[q] = %d\n", m, ORI[q]);
                  exit(0);
                }
#endif
                m = flood3d(m, FAH, STATUS, number_nodes, node_at_level, tree, connex, rs, ps, N, ORI); 
              } while ((m != h) && (m >= 0));
	    } /* if (ORI[q] > ORI[p]) */
          } /* if ((q != -1) && (STATUS[q] == NOT_ANALYZED)) */
        } /* for (...) */
        break;
      case 26:
        for (k = 0; k < 26; k++)   /* parcourt les 26 voisins */
        {
          q = voisin26(p, k, rs, ps, N);
          if ((q != -1) && (STATUS[q] == NOT_ANALYZED))
          {
            FahPush(FAH, q, ORI[q]);
            STATUS[q] = IN_THE_QUEUE;
            node_at_level[ORI[q]] = 1;
            if (ORI[q] > ORI[p])
	    {
              m = ORI[q];
              do 
              {
#ifdef PARANO
                if ((m < 0) || (m > 255))
                {
                  fprintf(stderr, "flood3d: mauvais niveau : %d ; ORI[q] = %d\n", m, ORI[q]);
                  exit(0);
                }
#endif
                m = flood3d(m, FAH, STATUS, number_nodes, node_at_level, tree, connex, rs, ps, N, ORI); 
              } while ((m != h) && (m >= 0));
	    } /* if (ORI[q] > ORI[p]) */
          } /* if ((q != -1) && (STATUS[q] == NOT_ANALYZED)) */
        } /* for (...) */
        break;
      default:
        fprintf(stderr, "flood3d: mauvaise connexite: %d\n", connex);
        exit(0);
    } /* switch (connex) */

#ifdef ATTR_SURF
    surf += 1;
#endif
  } /* while (!FahVideNiveau(FAH, h)) */
#ifdef DEBUGFLOOD
  printf("retour flood3d niveau %d\n", h);
#endif
  number_nodes[h] += 1;

  m = h - 1;                                    /* second step : define the father */
  while ((m >= 0) && (!node_at_level[m])) m--;  /* =============================== */
  i = number_nodes[h] - 1;
  if (m >= 0)
  {
    j = number_nodes[m];
#ifdef DEBUGFLOOD
    printf("AjouteArc (%d %d) (%d %d)\n", m, j, h, i);
#endif
    AjouteArc(tree, ENCODE(j,m), ENCODE(i,h)    /* definit Cm,j comme le pere de Ch,i */
#ifdef ATTR_SURF
              , surf
#endif
#ifdef ATTR_PERIM
              , perim
#endif
#ifdef ATTR_HBORD
              , hbord
#endif
             ); 
  } /* if (m >= 0) */
  else
  {
#ifdef DEBUGFLOOD
    printf("DefinitRacine (%d %d)\n", h, i);
#endif
    tree->racine = ENCODE(i,h);                /* Ch,i est racine */
#ifdef ATTR_SURF
    tree->surf_racine = surf;
#endif
  }
  node_at_level[h] = 0;
#ifdef DEBUGFLOOD
  printf("fin flood3d niveau %d, retourne %d\n", h, m);
#endif
  return m;
} /* flood3d() */

/* ==================================== */
int32_t LowestCommonAncestor(
  CompactTree * cpct,
  int32_t argc,
  int32_t *argv,
  uint8_t d)
/* Retourne le plus proche commun ancetre des cellules de la liste (argc, argv)
   dont le niveau est > d, ou -1 s'il n'existe pas. 
   Utilise le champ "flags". 
   
*/
/* ==================================== */
{
  int32_t x, i, lca, NoComAnc;
  if (argc <= 0) return -1;
  if (argc == 1) return argv[0];

  x = argv[0];      /* index de la premiere cellule */
  do
  {
    cpct->flags[x] |= LCA1;     /* marque LCA1 tous les ancetres de x plus hauts que d */
    x = cpct->pere[x];
  } while ((x != CPCT_ROOT) && (DECODENIV(cpct->comp[x]) > d));
  
  for (i = 1; i < argc; i++)   /* boucle pour les autres cellules */
  {
    x = argv[i];
    NoComAnc = 1;              /* pas d'ancetre commun trouve */
    do
    {                          /* remonte les ancetres de x sans depasser d */
      if (cpct->flags[x] & LCA1)  /* on a un ancetre commun */
      {
        NoComAnc = 0; 
      	cpct->flags[x] |= LCA2;   /* on le marque LCA2 */
      	break;                 /* on arrete la remontee (sort du do while) */
      }
      else
        x = cpct->pere[x];     /* on continue la remontee */
    } while ((x != CPCT_ROOT) && (DECODENIV(cpct->comp[x]) > d));
    if (NoComAnc) break;       /* pas d'AC: on sort aussi du for */
  } /* for (i = 1; i < argc; i++) */

  x = argv[0];        /* index de la premiere cellule */
  do
  {                   /* derniere remontee: demarque et repere le lca */
    if (cpct->flags[x] & LCA2) lca = x;
    cpct->flags[x] &= ~LCA;
    x = cpct->pere[x];
  }  while ((x != CPCT_ROOT) && (DECODENIV(cpct->comp[x]) > d));
  
  for (i = 1; i < argc; i++)   /* boucle pour les autres cellules */
  {
    x = argv[i];
    do
    { cpct->flags[x] &= ~LCA;
      x = cpct->pere[x];     /* on continue la remontee */
    } while ((x != CPCT_ROOT) && (DECODENIV(cpct->comp[x]) > d));
  } /* for (i = 1; i < argc; i++) */

#ifdef DEBUGLCA
printf("LowComAnc(");
for (i = 0; i < argc; i++) printf(" %d ", argv[i]);
printf(") -> ");
if (NoComAnc) printf("NIL\n"); else printf("%d\n", lca);
#endif

  if (NoComAnc)                /* pas d'ancetre commun */
    return -1;
  else
    return lca;
} /* LowestCommonAncestor() */

/* ==================================== */
int32_t Ancestor(CompactTree * cpct, int32_t c1, int32_t c2)
/* Teste si la composante c1 est ancetre de la composante c2
*/
/* ==================================== */
{
  do
  {
    if (c1 == c2) return 1;
    c2 = cpct->pere[c2];
  } while (c2 != CPCT_ROOT);
  return 0;
} /* Ancestor() */

/* ======================================================================== */
/* ======================================================================== */
/* OPERATEURS BASES SUR L'ARBRE DES COMPOSANTES */
/* ======================================================================== */
/* ======================================================================== */

#ifdef ATTR_HEIGHT
/* ==================================== */
int32_t lsegmentheight(struct xvimage *image, int32_t connex, int32_t param, int32_t maximise)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsegmentheight"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */


  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  (void)FiltreHeightRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreHeightRec terminee\n");
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lsegmentheight() */

#ifdef RECONS
/* ==================================== */
int32_t lsegreconsheight(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsegreconsheight"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t N = rs * cs;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 


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

  (void)FiltreHeightRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreHeightRec terminee\n");
#endif

  (void)MaximiseSegmentation(CTREE, 0);

#ifdef VERBOSE
  fprintf(stderr, "MaximiseSegmentation terminee\n");
#endif

  Reconstruction(CTREE, 0);

#ifdef VERBOSE
  fprintf(stderr, "Reconstruction terminee\n");
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lsegreconsheight() */
#endif
#endif

#ifdef ATTR_SURF
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
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */


  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lsegmentarea() */
#endif

#ifdef ATTR_VOL
/* ==================================== */
int32_t lsegmentvol(struct xvimage *image, int32_t connex, int32_t param, int32_t maximise)
/* ==================================== */
#undef F_NAME
#define F_NAME "lsegmentvol"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */
  int32_t nbcomp;

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  (void)FiltreVolRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreVolRec terminee\n");
#endif

  if ( maximise )
    (void)MaximiseSegmentation(CTREE, 0);

  nbcomp = NbLeafs(CTREE, 0);

#ifdef VERBOSE
  fprintf(stderr, "nombre de composantes pertinentes : %d\n", nbcomp);
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lsegmentvol() */
#endif

#ifdef ATTR_HEIGHT
/* ==================================== */
void ProcessLeafMins(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  Recherche a partir du sommet som, les sommets marques LEAFMIN.
  Demarque en remontant jusqu'au premier noeud.
  Version pour un filtre morphologique (avec RecupereImageFiltreeH).
*/
{
  int32_t i, j, n, h;

  if (!(cpct->flags[som] & LEAFMIN))
  {
    n = NBFILS(som);           /* remonte l'arbre pour trouver une LEAFMIN */ 
    for (i = 0; i < n; i++) 
    {
      j = INDEXFILS(som, i);
      j = cpct->fils[j];
      ProcessLeafMins(cpct, j);
    }
  }
  else /* (on a trouve une LEAFMIN) */
  {
    cpct->flags[som] &= ~LEAFMIN; /* demarque som */
    while (NBFILS(som) == 1)      /* remonte jusqu'au premier noeud ou feuille */
    {
      som = INDEXFILS(som, 0);
      som = cpct->fils[som];
      cpct->flags[som] &= ~FILTERED_OUT;
    }
    cpct->flags[som] |= LEAFMIN;  /* marque */
  }
} /* ProcessLeafMins() */

/* ==================================== */
void ProcessLeafMinsOp(CompactTree * cpct, int32_t som)
/* ==================================== */
/*
  Recherche a partir du sommet som, les sommets marques LEAFMIN.
  Demarque en remontant jusqu'a la feuille la plus haute.
  Version pour une ouverture (avec RecupereImageFiltree).
*/
{
  int32_t i, j, n, m, jm, h;

  if (!(cpct->flags[som] & LEAFMIN))
  {
    n = NBFILS(som);           /* remonte l'arbre pour trouver une LEAFMIN */ 
    for (i = 0; i < n; i++) 
    {
      j = INDEXFILS(som, i);
      j = cpct->fils[j];
      ProcessLeafMinsOp(cpct, j);
    }
  }
  else /* (on a trouve une LEAFMIN) */
  {
    cpct->flags[som] &= ~LEAFMIN; /* demarque som */
    while (NBFILS(som) > 0)       /* remonte jusqu'a une (vraie) feuille */
    {
      /* recherche l'indice im du fils de hauteur maxi */
      n = NBFILS(som);
      m = -1;
      for (i = 0; i < n; i++) 
      {
        j = INDEXFILS(som, i);
        j = cpct->fils[j];
        h = DECODENIV(cpct->comp[j]) + cpct->height[j];
        if (h > m) { m = h; jm = j; } 
      }
      /* remonte en demarquant */
      som = jm;
      cpct->flags[som] &= ~FILTERED_OUT;
    }
    cpct->flags[som] |= LEAFMIN;  /* marque LEAFMIN le sommet atteint */
  }
} /* ProcessLeafMinsOp() */

/* ==================================== */
void RecupereImageFiltreeH(CompactTree * cpct,           
       uint32_t *STATUS,
       int32_t rs, int32_t N, 
       uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t i, h;
  uint32_t c, comp;
  for (i = 0; i < N; i++) 
  {
    h = ORI[i];
    c = STATUS[i];
    comp = INDEXCOMP(h,c);
    while (cpct->flags[comp] == FILTERED_OUT) comp = cpct->pere[comp];
    if (cpct->flags[comp] & LEAFMIN)
      ORI[i] = DECODENIV(cpct->comp[comp]) + cpct->height[comp];
    else
      ORI[i] = DECODENIV(cpct->comp[comp]);
  }  
} /* RecupereImageFiltreeH() */

/* ==================================== */
int32_t lheightmaxima_variante(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lheightmaxima_variante"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  (void)FiltreHeightRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreHeightRec terminee\n");
#endif

#ifdef DEBUG
  AfficheCompactTree(CTREE);
  WriteCompactTree(CTREE, "ctree.graph");
#endif

  ProcessLeafMinsOp(CTREE, 0);

#ifdef VERBOSE
  fprintf(stderr, "ProcessLeafMinsOp terminee\n");
#endif

  RecupereImageFiltree(CTREE, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereImageFiltree terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lheightopening() */

/* ==================================== */
int32_t lheightmaxima(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lheightmaxima"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  (void)FiltreHeightRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreHeightRec terminee\n");
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lheightmaxima() */
#endif

#ifdef ATTR_SURF
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
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */
  
  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F); 

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

  FahTermine(FAH);
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
  for (i = 0; i < N; i++) F[i] = 255 - F[i];
  ret = lareaopening(image, connex, param);
  for (i = 0; i < N; i++) F[i] = 255 - F[i];
  return(ret);
} /* lareaclosing() */
#endif

#ifdef ATTR_VOL
/* ==================================== */
int32_t lvolmaxima(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lvolmaxima"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */
  
  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F); 

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

  (void)FiltreVolRec(CTREE, 0, param);

#ifdef VERBOSE
  fprintf(stderr, "FiltreVolRec terminee\n");
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lvolmaxima() */
#endif

#ifdef ATTR_HEIGHT
/* ==================================== */
int32_t lheightselnb(struct xvimage *image, int32_t connex, int32_t param, int32_t mode)
/* ==================================== */
#undef F_NAME
#define F_NAME "lheightselnb"
/* 
  param: nombre de composantes a trouver
  mode: parametre obsolete
*/
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
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

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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
  i_TriRapideStochastique (A, cpct->height, 0, nbcomp-1);
  i = 0;
  while ((nbfeuilles > param) && (i < nbcomp))
  {
    //printf("i = %d, A[i] = %d, cpct->height[A[i]] = %d, nbf = %d\n", 
    //       i, A[i], cpct->height[A[i]], nbfeuilles);
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

  //  RecupereImageFiltree(CTREE, STATUS, rs, N, F);
  RecupereSegmentation(cpct, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereSegmentation terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  free(A);
  return(1);
} /* lheightselnb() */
#endif

#ifdef ATTR_SURF
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
  Fah * FAH;                    /* la file d'attente hierarchique */
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

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  free(A);
  return(1);
} /* lareaselnb() */
#endif

#ifdef ATTR_VOL
/* ==================================== */
int32_t lvolselnb(struct xvimage *image, int32_t connex, int32_t param)
/* ==================================== */
#undef F_NAME
#define F_NAME "lvolselnb"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
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

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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
  i_TriRapideStochastique (A, cpct->vol, 0, nbcomp-1);
  i = 0;
  while ((nbfeuilles > param) && (i < nbcomp))
  {
    //printf("i = %d, A[i] = %d, cpct->vol[A[i]] = %d, nbf = %d\n", 
    //       i, A[i], cpct->vol[A[i]], nbfeuilles);
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

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  free(A);
  return(1);
} /* lvolselnb() */
#endif


#ifdef ATTR_DYN
/* ==================================== */
void CalculeDynamiqueMaxima(CompactTree * cpct)
/* ==================================== */
#undef F_NAME
#define F_NAME "CalculeDynamiqueMaxima"
/* 
Definition Grimaud : obligation d'atteindre un maximum de niveau strictement superieur
*/
{
  uint32_t nbcomp = cpct->nbcomp;
  int32_t i, h, ncompnivh, f, p, s;
  uint8_t *tmp;

  tmp = (uint8_t *)calloc(nbcomp, sizeof(char));
  if (tmp == NULL) 
  {   fprintf(stderr, "%s() : calloc failed for tmp\n", F_NAME);
      exit(0);
  }

  /* met le champ dyn a 0 */
  for (i = 0; i < nbcomp; i++)  cpct->dyn[i] = 0;

  /* balaye les feuilles par ordre decroissant */
  for (h = 255; h > 0; h--)
  {
    ncompnivh = cpct->hc[h] - cpct->hc[h-1]; /* nb composantes dans le niveau h */
    if (ncompnivh > 0)
    {
      for (i = cpct->hc[h] - 1; i > cpct->hc[h] - 1 - ncompnivh; i--)
      {
        if (NBFILS(i) == 0) /* feuille */
        {
          f = i;
          p = cpct->pere[f];
          /* 
            on remonte la branche a partir de la feuille i
            en marquant les noeuds (grace a tmp) par le niveau de i,
            jusqu'a trouver un noeud p marque (tmp) par une valeur plus forte
            ou la racine
            alors : dyn[i] = niv[i] - niv[p]
	  */
          while ((p != f) && (tmp[p] <= h))
	  {
            tmp[p] = h;
            f = p;
            p = cpct->pere[f];
	  } /* while ((p != f) && (tmp[p] <= h)) */
          cpct->dyn[i] = h - DECODENIV(cpct->comp[p]);
	} /* if (NBFILS(i) == 0) */
      } /* for i */
    } /* if (ncompnivh > 0) */
  } /* for h */

#ifdef VERBOSE
  printf("FIN DU CALCUL DE LA DYNAMIQUE DES MAXIMA\n");
#endif

  free(tmp);
} /* CalculeDynamiqueMaxima() */

/* ==================================== */
void CalculeDynamiqueContours(CompactTree * cpct)
/* ==================================== */
#undef F_NAME
#define F_NAME "CalculeDynamiqueContours"
/* 
Variante definition Grimaud : obligation d'atteindre un maximum de niveau superieur ou egal
*/
{
  uint32_t nbcomp = cpct->nbcomp;
  int32_t i, h, ncompnivh, f, p, s, v;
  uint8_t *tmp;

  tmp = (uint8_t *)calloc(nbcomp, sizeof(char));
  if (tmp == NULL) 
  {   fprintf(stderr, "%s() : calloc failed for tmp\n", F_NAME);
      exit(0);
  }

  /* met le champ dyn a 0 */
  for (i = 0; i < nbcomp; i++)  cpct->dyn[i] = 0;

  /* balaye les feuilles par ordre decroissant */
  for (h = 255; h > 0; h--)
  {
    ncompnivh = cpct->hc[h] - cpct->hc[h-1]; /* nb composantes dans le niveau h */
    if (ncompnivh > 0)
    {
      for (i = cpct->hc[h] - 1; i > cpct->hc[h] - 1 - ncompnivh; i--)
      {
        if (NBFILS(i) == 0) /* feuille */
        {
          f = i;
          p = cpct->pere[f];
          /* 
            on remonte la branche a partir de la feuille i
            en marquant les noeuds (grace a tmp) par le niveau de i,
            jusqu'a trouver un noeud p marque (tmp) par une valeur plus forte ou egale
            ou la racine
            alors : dyn[i] = niv[i] - niv[p]
            et (si non racine) : dyn[p] = max(dyn[p],dyn[i])
               (si racine) : -1 la premiere fois, idem ensuite
	  */
          while ((p != f) && (tmp[p] < h))
	  {
            tmp[p] = h;
            f = p;
            p = cpct->pere[f];
	  } /* while ((p != f) && (tmp[p] < h)) */
          cpct->dyn[i] = h - DECODENIV(cpct->comp[p]);
          if (p != f) cpct->dyn[p] = max(cpct->dyn[p],cpct->dyn[i]);
          else
	  {
            if (cpct->dyn[p] == 0) cpct->dyn[p] = -1;
            else cpct->dyn[p] = max(cpct->dyn[p],cpct->dyn[i]);
	  }
	} /* if (NBFILS(i) == 0) */
      } /* for i */
    } /* if (ncompnivh > 0) */
  } /* for h */

  // detection des noeuds i violant la condition : 
  //   val(i) >= val(p) pour tout p descendant de i

  // balaye les noeuds non feuilles par ordre de niveau decroissant.
  // a partir d'un noeud i (de valuation v)
  // on remonte vers la racine: 
  //   soit p le noeud courant
  //   si val(p) < v alors marque p (flag CONDITION_VIOLATION)
  //   sinon v = val(p) pour continuer la verification sur les ancetres de i
  for (h = 255; h > 0; h--)
  {
    ncompnivh = cpct->hc[h] - cpct->hc[h-1]; /* nb composantes dans le niveau h */
    if (ncompnivh > 0)
    {
      for (i = cpct->hc[h] - 1; i > cpct->hc[h] - 1 - ncompnivh; i--)
      {
        if (NBFILS(i) > 0) /* non feuille */
        {
          v = cpct->dyn[i];
          f = i;
          p = cpct->pere[f];
          while (p != f)
	  {
            if (cpct->dyn[p] < v) cpct->flags[p] |= CONDITION_VIOLATION;
            else v = cpct->dyn[p];
            f = p;
            p = cpct->pere[f];
	  } /* while (p != f) */
	} /* if (NBFILS(i) > 0) */
      } /* for i */
    } /* if (ncompnivh > 0) */
  } /* for h */

#ifdef VERBOSE
  printf("FIN DU CALCUL DE LA DYNAMIQUE DES CONTOURS\n");
#endif

  free(tmp);
} /* CalculeDynamiqueContours() */

/* ==================================== */
void RecupereDynamique(CompactTree * cpct,           
       uint32_t *STATUS,
       int32_t rs, int32_t N, 
       uint8_t *ORI            /* informations sur l'image originale */
)
/* ==================================== */
{
  int32_t i, h;
  uint32_t c, comp;
  for (i = 0; i < N; i++) 
  {
    h = ORI[i];
    c = STATUS[i];
    comp = INDEXCOMP(h,c);
    ORI[i] = cpct->dyn[comp];
  }  
} /* RecupereDynamique() */

/* ==================================== */
int32_t ldynamique(struct xvimage *image, int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "ldynamique"
{
  register int32_t i, k, l;         /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * CTREE;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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

  CalculeDynamiqueMaxima(CTREE);

#ifdef VERBOSE
  fprintf(stderr, "CalculeDynamique terminee\n");
#endif

#ifdef DEBUG
  AfficheCompactTree(CTREE);
  AfficheImaComp(CTREE, STATUS, rs, N, F);
#endif

  RecupereDynamique(CTREE, STATUS, rs, N, F);

#ifdef VERBOSE
  fprintf(stderr, "RecupereDynamique terminee\n");
#endif

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(CTREE);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* ldynamique() */

/* ==================================== */
void SimplifyComp(CompactTree *cpct, int32_t *ncomp, int32_t *tabcomp) 
/* ==================================== */
// supprime dans tabcomp les composantes qui sont ancetres d'une autre
{
  int32_t k, j, i; 
  for (i = 0; i < *ncomp; i++)
  if (tabcomp[i] != -1)
  {
    for (j = i + 1; j < *ncomp; j++)
    if (tabcomp[j] != -1)
    {
      if (Ancestor(cpct, tabcomp[j], tabcomp[i])) 
      {
        tabcomp[j] = -1;
      }
      else if (Ancestor(cpct, tabcomp[i], tabcomp[j])) 
      {
        tabcomp[i] = -1;
        break;
      }
    } // for j
  } // for i
  k = j = 0;
  for (i = 0; i < *ncomp; i++)
  {
    tabcomp[j] = tabcomp[i];
    if (tabcomp[i] != -1)
      j++;
    else
      k++; // nb elements supprimes
  }
  *ncomp -= k;
} /* SimplifyComp() */

/* ================================================ */
void BuildTree(uint8_t *F, int32_t rs, int32_t ps, int32_t N, int32_t connex, int32_t incr_vois,
	       Fah * FAH, uint32_t *STATUS, 
               uint32_t *number_nodes, uint8_t *node_at_level,
               CompTree * TREE, CompactTree ** cpct
              )
/* ================================================ */
{
  int32_t i, j, k;

  // INITIALISATIONS
  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 0; i < N; i++) if (F[i] < F[k]) k = i;
  FahPush(FAH, k, F[k]);
  // APPEL FONCTION RECURSIVE flood
  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);
  *cpct = CompTree2CompactTree(TREE, number_nodes);
} // BuildTree()

/* ==================================== */
int32_t TrouveComposantes(int32_t x, uint8_t *F, int32_t rs, int32_t N, int32_t incr_vois, 
                  uint32_t *STATUS, CompactTree * cpct, int32_t *tabcomp)
/* ==================================== */
{
    int32_t k, y, ncomp = 0;
    for (k = 0; k < 8; k += incr_vois) // parcourt les c-voisins y de x d'un niveau > F[x]
    {
      y = voisin(x, k, rs, N);
      if ((y != -1) && (F[y] > F[x]))
      {
        tabcomp[ncomp] = INDEXCOMP(F[y],STATUS[y]);
        ncomp++;
      }
    } /* for (k = 0; k < 8; k += incr_vois) */
#ifdef DEBUG
    printf("    Comp. du voisinage: %d [%d %d %d %d]\n", ncomp, tabcomp[0], tabcomp[1], tabcomp[2], tabcomp[3]);
#endif
    SimplifyComp(cpct, &ncomp, tabcomp); // supprime dans tabcomp les composantes qui sont ancetres d'une autre

#ifdef DEBUG
    printf("    Comp. du voisinage (simpl.): %d [%d %d %d %d]\n", ncomp, tabcomp[0], tabcomp[1], tabcomp[2], tabcomp[3]);
#endif
    return ncomp;
} // TrouveComposantes() 

#define EN_FAH     0 
#define WATERSHED  1
#define MASSIF     2
#define WATERSHED2 3
#define MODIFIE    4

/* ==================================== */
int32_t Watershed_old(
  uint8_t *F, 
  int32_t rs, 
  int32_t N, 
  int32_t incr_vois, 
  Fah * FAH, 
  uint32_t *STATUS, 
  CompactTree * cpct)
/* ==================================== */
#undef F_NAME
#define F_NAME "Watershed_old"
{
  int32_t i, j, k, x, y;
  int32_t c;                        /* une composante */
  int32_t tabcomp[8];               /* liste de composantes */
  int32_t ncomp;                    /* nombre de composantes dans tabcomp */
  int32_t nbelev;                   /* nombre d'elevations effectuees */

  // INITIALISATIONS
  FahFlush(FAH); // Re-initialise la FAH

  // etiquetage des c-maxima (doit pouvoir se faire au vol lors de la construction de l'arbre)
  for (i = 0; i < N; i++)
  {
    c = INDEXCOMP(F[i],STATUS[i]);
    if (NBFILS(c) == 0) Set(i,MASSIF);
  } // for (i = 0; i < N; i++)

  // empile les c-voisins des c-maxima
  for (i = 0; i < N; i++)
  {
    if (IsSet(i,MASSIF))
    {
      for (k = 0; k < 8; k += incr_vois)
      {
        j = voisin(i, k, rs, N);
        if ((j != -1) && (!IsSet(j,MASSIF)) && (!IsSet(j,EN_FAH)))
	{
          Set(j,EN_FAH);
          FahPush(FAH, j, NDG_MAX - F[j]);
	}
      } /* for (k = 0; k < 8; k += incr_vois) */
    } // if (IsSet(i,MASSIF))
  } // for (i = 0; i < N; i++)

  // BOUCLE PRINCIPALE
  nbelev = 0;
  while (!FahVide(FAH))
  {
    x = FahPop(FAH);
    UnSet(x,EN_FAH);
#ifdef DEBUG
    printf("Pop Point %d,%d Niveau %d\n", x % rs, x / rs, F[x]);
#endif

    ncomp = TrouveComposantes(x, F, rs, N, incr_vois, STATUS, cpct, tabcomp);

    if (ncomp == 1)
      c = tabcomp[0];
    else 
      c = LowestCommonAncestor(cpct, ncomp, tabcomp, F[x]);

#ifdef DEBUG
    printf("    LCA: %d\n", c);
#endif

    if ((c == -1) && (ncomp >= 2))
    {
      Set(x,WATERSHED);
#ifdef DEBUG
      printf("    Pas d'elevation ; LPE\n");
#endif
    }

    if (c != -1)
    {
      nbelev++;
      F[x] = DECODENIV(cpct->comp[c]);      // eleve le niveau du point x
      STATUS[x] = DECODENUM(cpct->comp[c]); // maj pointeur image -> composantes 
      Set(x,MODIFIE);
      if (NBFILS(c) == 0) // feuille
      {
        Set(x,MASSIF);
#ifdef DEBUG
        printf("    Eleve au niveau: %d ; MASSIF\n", F[x]);
#endif
      } // if feuille
      else
      if (NBFILS(c) > 1) // noeud
      {
        Set(x,WATERSHED);
#ifdef DEBUG
        printf("    Eleve au niveau: %d ; LPE\n", F[x]);
#endif
      }
#ifdef PARANO
      else
        printf("Watershed() : ERREUR COMPOSANTE BRANCHE!!!\n");
#endif

      // empile les c-voisins de x non marques MASSIF ni EN_FAH
      for (k = 0; k < 8; k += incr_vois)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (!IsSet(y,MASSIF)) && (!IsSet(y,EN_FAH)))
        {
          Set(y,EN_FAH);
          FahPush(FAH, y, NDG_MAX - F[y]);
#ifdef DEBUG
          printf("        Push Point %d,%d Niveau %d\n", y % rs, y / rs, F[y]);
#endif
        }
      } // for (k = 0; k < 8; k += incr_vois)
    } // if (c != -1)
  } // while (!FahVide(FAH))

#ifdef VERBOSE
    printf("Nombre d'elevations %d\n", nbelev);
#endif
} // Watershed_old()

/* ==================================== */
int32_t trouvefeuillerec(CompactTree * cpct, int32_t p, int32_t v)
/* ==================================== */
/*
  retourne l'index d'une feuille descendant de p tq dyn(f) = v, ou -1 en cas d'echec
*/ 
{
  int32_t i, n, j, f;
  n = NBFILS(p);
  if (n == 0) 
  {
    if (cpct->dyn[p] == v) return p;
    return -1;
  }
  for (i = 0; i < n; i++) 
  {
    j = INDEXFILS(p, i);
    j = cpct->fils[j];
    f = trouvefeuillerec(cpct, j, v);
    if (f != -1) return f;
  }
  return -1;
} /* trouvefeuillerec() */

/* ==================================== */
void ElimineDupliques(int32_t *ncomp, int32_t *tabcomp) 
/* ==================================== */
{
  int32_t k, j, i; 
  for (i = 0; i < *ncomp; i++)
    if (tabcomp[i] != -1)
      for (j = i + 1; j < *ncomp; j++)
        if (tabcomp[j] == tabcomp[i])
          tabcomp[j] = -1;
  k = j = 0;
  for (i = 0; i < *ncomp; i++)
  {
    tabcomp[j] = tabcomp[i];
    if (tabcomp[i] != -1)
      j++;
    else
      k++; // nb elements supprimes
  }
  *ncomp -= k;
} /* ElimineDupliques() */

/* ==================================== */
static int32_t InList(int32_t e, int32_t *list, int32_t n)                       
/* ==================================== */
{
/* renvoie 1 si e est dans list, 0 sinon */
/* e : l'element a rechercher */
/* list : la liste (tableau d'entiers) */
/* n : le nombre d'elements dans la liste */
  while (n > 0)
    if (list[--n] == e) return 1;
  return 0;
} /* InList() */

/* ==================================== */
int32_t lwshedval(struct xvimage *image, int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "lwshedval"
/*! \fn int32_t lwshedval(struct xvimage *image, int32_t connex)
    \param image (entrée/sortie) : une image 2D ndg
    \param connex (entrée) : 4 ou 8 
    \return code erreur : 0 si échec, 1 sinon
    \brief ligne de partage des eaux valuée (algo MC, GB, LN)
*/
/*
ndg := noyau de l'image originale
calculer arbre et dynamiques des feuilles
repeter
  f = feuille de dynamique minimale
  df = dyn(f)
  hcf = |ndg(f) - dyn(f)| 
  ptcol = (select x in border(f), ndg(x) == hcf)
  vf = (select f' feuille, ptcol in border(f'))
  ptsarc = {x in border(f) inter border(vf)}
  pour tout x dans ptsarc faire V(x) := dyn(f)
  supprimer f
  raccourcir branches
jusqu'a size(tree) ==  1
*/
{
  register int32_t i, j, k, l;      /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *V;             /* l'image resultat (valuations) */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * cpct;           /* resultat : l'arbre des composantes compacte' */
  Lifo * LIFO;                  /* liste d'attente pour les points a problemes */
                                /* OPTIM remarque : on peut gagner de la place en 
                                   utilisant un niveau "bidon" dans la FAH pour stocker ces points 
                                   vu qu'ils ne peuvent se trouver dans les 2 a la fois
                                */
  int32_t vmin;

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);
  LIFO = CreeLifoVide(N);

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

  V = (uint8_t *)calloc(N, sizeof(char));
  if (V == NULL)
  {   fprintf(stderr, "%s() : calloc failed for V\n", F_NAME);
      return(0);
  }

  IndicsInit(N);

  /* ======================================================================= */
  /* 1ERE ETAPE : CALCUL DE L'ARBRE DES COMPOSANTES ET DE LA DYNAMIQUE */
  /* ======================================================================= */

  BuildTree(F, rs, ps, N, connex, incr_vois,
	    FAH, STATUS, number_nodes, node_at_level, TREE, &cpct);

  CalculeDynamiqueMaxima(cpct);

#ifdef DEBUG
  AfficheCompactTree(cpct);
#endif

  /* ======================================================================= */
  /* 2EME ETAPE : CALCUL DE LA LIGNE DE PARTAGE DES EAUX */
  /* ======================================================================= */

  Watershed(F, rs, N, incr_vois, FAH, STATUS, cpct);

{ int32_t nbcomp = cpct->nbcomp;
  int32_t h, c, comp;  
  int32_t f, df, hcf;
  int32_t vf, nvf, svf, ptcol, voisinf, voisinvf;

  i = 0;
  while (nbcomp > 1)
  {
    // trouve la feuille f de dyn. minimale
    f = -1;
    df = NDG_MAX+1;
    for (k = 0; k < cpct->nbcomp; k++) 
      if (!(cpct->flags[k] & FILTERED_OUT) && 
           (NbFilsNonFiltres(cpct,k) == 0) && (cpct->dyn[k] < df))
      {
        f = k;
        df = cpct->dyn[f];
      }

    //  hcf = |ndg(f) - dyn(f)| 
    hcf = DECODENIV(cpct->comp[f]) - df;

#ifdef DEBUGWV
printf("f = %d ; dyn = %d ; ndg = %d ; hcf = %d\n", f, cpct->dyn[f], hcf+df, hcf);
#endif

    //  ptcol = (select x in border(f), ndg(x) == hcf)
    // attention il faut remonter les "branches mortes" 
    for (x = 0; x < N; x++)
    {
      h = F[x];
      c = STATUS[x];
      comp = INDEXCOMP(h,c);
      while ((comp != cpct->pere[comp]) && (cpct->flags[cpct->pere[comp]] & FILTERED_OUT)) 
        comp = cpct->pere[comp]; // remonte branche morte
      if ((comp != f) && (h == hcf))
      {
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(x, k, rs, N);
          if (y != -1)
	  {
            h = F[y];
            c = STATUS[y];
            comp = INDEXCOMP(h,c);
            while ((comp != cpct->pere[comp]) && (cpct->flags[cpct->pere[comp]] & FILTERED_OUT)) 
              comp = cpct->pere[comp]; // remonte branche morte
            if (comp == f)
            {
              ptcol = x;
              goto ptcolfound;
	    }
	  } // if (y != -1)
        } // for (k = 0; k < 8; k += incr_vois)
      } // if (comp == f) 
    } // for (x = 0; x < N; x++)
ptcolfound:
#ifdef DEBUGWV
printf("ptcol = %d,%d\n", ptcol%rs, ptcol/rs);
#endif

    //  vf = (select f' feuille, ptcol in border(f'))
    x = ptcol;
    for (k = 0; k < 8; k += incr_vois)
    {
      y = voisin(x, k, rs, N);
      if (y != -1)
      {
        h = F[y];
        c = STATUS[y];
        comp = INDEXCOMP(h,c);
        // while ((comp != cpct->pere[comp]) && (cpct->flags[cpct->pere[comp]] & FILTERED_OUT)) 
        //  comp = cpct->pere[comp]; // remonte branche morte
        if ((comp != f) && !(cpct->flags[comp] & FILTERED_OUT) && (NbFilsNonFiltres(cpct,comp) == 0))
        {
          vf = comp;
          nvf = h;
          svf = c;
          break;
        }
      } // if (y != -1)
    } // for (k = 0; k < 8; k += incr_vois)
#ifdef DEBUGWV
printf("vf = %d, nvf = %d\n", vf, nvf);
#endif

    //  ptsarc = {x in border(f) inter border(vf)}
    //  pour tout x dans ptsarc faire V(x) := dyn(f)
    for (x = 0; x < N; x++)
    {
      voisinf = voisinvf  = 0;
      for (k = 0; k < 8; k += incr_vois)
      {
        y = voisin(x, k, rs, N);
        if (y != -1)
        {
          h = F[y];
          c = STATUS[y];
          comp = INDEXCOMP(h,c);
	  //          while ((comp != cpct->pere[comp]) && (cpct->flags[cpct->pere[comp]] & FILTERED_OUT)) 
          //  comp = cpct->pere[comp]; // remonte branche morte
          if (comp == f) { voisinf = 1; printf("x = %d,%d ; y = %d,%d ; voisin de f =  %d\n", x % rs, x / rs, y % rs, y / rs, f); }
          if (comp == vf) { voisinvf = 1; printf("x = %d,%d ; y = %d,%d ; voisin de vf =  %d\n", x % rs, x / rs, y % rs, y / rs, vf); }
	} // if (y != -1)
      } // for (k = 0; k < 8; k += incr_vois)
      if (voisinf && voisinvf) 
      {
        V[x] = df;
#ifdef DEBUGWV
printf("x = %d,%d ; V[x] = %d\n", x % rs, x / rs, V[x]);
#endif
      }
    } // for (x = 0; x < N; x++)

    //  supprimer f
    cpct->flags[f] |= FILTERED_OUT;
    nbcomp--;
#ifdef DEBUGWV
printf("supprime %d ; nbcomp = %d\n", f, nbcomp);
#endif
    // fusionne f et vf
    for (x = 0; x < N; x++)
    {
      h = F[x];
      c = STATUS[x];
      comp = INDEXCOMP(h,c);
      if (comp == f)
      {
        F[x] = nvf;
        STATUS[x] = svf;
#ifdef DEBUGWV
printf("x = %d,%d ; nouveau F[x] = %d\n", x % rs, x / rs, F[x]);
#endif
      }
    }

    //  raccourcir branches
    //  i.e. supprimer peres des noeuds dont le pere n'a qu'un fils
    for (comp = 0; comp < cpct->nbcomp; comp++)
      if (!(cpct->flags[comp] & FILTERED_OUT) && (NbFilsNonFiltres(cpct,cpct->pere[comp]) == 1))
        {
          if (!(cpct->flags[cpct->pere[comp]] & FILTERED_OUT))
          {          
            cpct->flags[cpct->pere[comp]] |= FILTERED_OUT;
            nbcomp--;
#ifdef DEBUGWV
printf("supprime %d ; nbcomp = %d\n", cpct->pere[comp], nbcomp);
#endif
	  }
	}

AfficheCompactTree(cpct);

  } // while (nbcomp > 1)
}

  memcpy(F, V, N); // recopie de V dans F

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahTermine(FAH);
  LifoTermine(LIFO);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  free(V);
  return(1);
} /* lwshedval() */

/* ==================================== */
int32_t lwshedtopo(struct xvimage *image, int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "lwshedtopo"
/*! \fn int32_t lwshedtopo(struct xvimage *image, int32_t connex)
    \param image (entrée/sortie) : une image 2D ndg
    \param connex (entrée) : 4 ou 8 
    \return code erreur : 0 si échec, 1 sinon
    \brief ligne de partage des eaux "topologique" (algo MC, GB)
*/
{
  register int32_t i, j, k, l;      /* index muet */
  register int32_t w, x, y, z;      /* index muet de pixel */
  int32_t rs = rowsize(image);      /* taille ligne */
  int32_t cs = colsize(image);      /* taille colonne */
  int32_t ds = depth(image);        /* nb plans */
  int32_t ps = rs * cs;             /* taille plan */
  int32_t N = ps * ds;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                    /* la file d'attente hierarchique */
  int32_t incr_vois;                /* 1 pour la 8-connexite,  2 pour la 4-connexite */
  uint32_t *STATUS;         /* etat d'un pixel - doit etre initialise a NOT_ANALYZED */
                                /* en sortie, contient le numero de la composante de niveau h */
                                /* qui contient le pixel (avec h = valeur du pixel) */
  uint32_t *number_nodes;   /* nombre de composantes par niveau */
  uint8_t *node_at_level; /* tableau de booleens */
  CompTree * TREE;              /* resultat : l'arbre des composantes */
  CompactTree * cpct;          /* resultat : l'arbre des composantes compacte' */

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
  } /* switch (connex) */

  FAH = CreeFahVide(N);

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

  /* ======================================================================= */
  /* 1ERE ETAPE : CALCUL DE L'ARBRE DES COMPOSANTES */
  /* ======================================================================= */

  /* ================================================ */
  /* INITIALISATIONS                                  */
  /* ================================================ */

  for (i = 0; i < N; i++) STATUS[i] = NOT_ANALYZED;
  k = 0;             /* recherche un pixel k de niveau de gris minimal dans l'image */
  for (i = 0; i < N; i++) if (F[i] < F[k]) k = i;
  FahPush(FAH, k, F[k]);

#ifdef VERBOSE
  fprintf(stderr, "init terminee\n");
#endif

  /* ================================================ */
  /* APPEL FONCTION RECURSIVE flood                   */
  /* ================================================ */

  if ((connex == 4) || (connex == 8))
    (void)flood(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, incr_vois, rs, N, F); 
  else
    (void)flood3d(F[k], FAH, STATUS, number_nodes, node_at_level, TREE, connex, rs, ps, N, F);

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
  AfficheImaComp(cpct, STATUS, rs, N, F);
#endif

  /* ======================================================================= */
  /* 2EME ETAPE : CALCUL DE LA LIGNE DE PARTAGE DES EAUX */
  /* ======================================================================= */

  IndicsInit(N);
  Watershed(F, rs, N, incr_vois, FAH, STATUS, cpct);

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahTermine(FAH);
  TermineCompTree(TREE);
  TermineCompactTree(cpct);
  free(STATUS);
  free(number_nodes);
  free(node_at_level);
  return(1);
} /* lwshedtopo() */

#endif



