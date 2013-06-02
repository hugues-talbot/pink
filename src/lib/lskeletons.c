/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/* Michel Couprie -  juillet 2001 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcrbt.h>
#include <mctopo3d.h>
#include <mckhalimsky3d.h>
#include <mcfifo.h>
#include <mcliste.h>
#include <mcrlifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <pinktypes.h>
#include <ldist.h>
#include <lskeletons.h>

// valeurs pour mcindic
#define EN_RBT        0
#define EN_FIFO       1
#define EN_LISTE      1
#define CONTRAINTE    2
#define CONTRAINTE1   2
#define CONTRAINTE2   3
#define IN_SET_W      0
#define IN_SET_Y      1
#define IN_SET_K      3

// valeurs pour points objet - algos *CK*
#define OBJ 1
#define CAN 2
#define CR2 3
#define CR1 4
#define CR0 5

#define LARGE_VAL 1E40

//#define VERBOSE

//#define DEBUG
#ifdef DEBUG
#define DEBUGlskelCKSC3
//#define DEBUG_callmatch
#define DEBUG_resmatch
#define DEBUGmatch
#endif

//#define CONJECTURE_1
#define CONJECTURE_2

#define lskelCKSC3_VARIANTE_C
//#define USE_NKP_END
//#define lskelCKSC3_SANS_CONTRAINTE
//#define DCRUCIAL_MARK

#define PRIODIR

//#define NBDIR27
#define NBDIR44

#ifdef NBDIR27
#define NBDIR 27
#else
#define NBDIR 44
#endif

//#define TEST_SIMPLE_PAR_COLLAPSE

#ifdef DEBUG
/* ==================================== */
static void print_vois(uint8_t *vois)    
/* ==================================== */
{
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n", 
	 vois[12],vois[11],vois[10],vois[3],vois[2],vois[1],vois[21],vois[20],vois[19]);
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n", 
	 vois[13],vois[8],vois[9],vois[4],vois[26],vois[0],vois[22],vois[17],vois[18]);
  printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n\n", 
	 vois[14],vois[15],vois[16],vois[5],vois[6],vois[7],vois[23],vois[24],vois[25]);
} /* print_vois() */
#endif

/* ==================================== */
static int32_t typedir2d(uint8_t *F, index_t x, index_t rs, index_t N)
/* ==================================== */
{
  index_t y;
  int32_t n, s, e, o, sum;
  n = s = e = o = 0;
  y = voisin(x, NORD, rs, N); if ((y!=-1) && (F[y]==0)) n = 1;
  y = voisin(x, SUD, rs, N); if ((y!=-1) && (F[y]==0)) s = 1;
  y = voisin(x, EST, rs, N); if ((y!=-1) && (F[y]==0)) e = 1;
  y = voisin(x, OUEST, rs, N); if ((y!=-1) && (F[y]==0)) o = 1;
  sum = n + s + e + o;
  if (sum == 0) return 9 - 0;
  else if (sum == 1) 
  {
    if (n)      return 9 - 1;
    else if (s) return 9 - 2;
    else if (e) return 9 - 3;
    else /* if (o) */ return 9 - 4;
  }
  else // if (sum >= 2) 
  {
    if (n && e)      return 9 - 5;
    else if (s && o) return 9 - 6;
    else if (n && o) return 9 - 7;
    else /* if (s && e) */ return 9 - 8;
  }
} /* typedir2d() */

#ifdef NBDIR44
/* ==================================== */
static int32_t typedir3d(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
// classe le point x selon ses 6-voisins qui sont hors de l'objet (nombre et direction)
{
  index_t y;
  int32_t n, s, e, o, a, d, sum;
  n = s = e = o = a = d = 0;
  y = voisin6(x, NORD, rs, ps, N); if ((y!=-1) && (F[y]==0)) n = 1;
  y = voisin6(x, SUD, rs, ps, N); if ((y!=-1) && (F[y]==0)) s = 1;
  y = voisin6(x, EST, rs, ps, N); if ((y!=-1) && (F[y]==0)) e = 1;
  y = voisin6(x, OUEST, rs, ps, N); if ((y!=-1) && (F[y]==0)) o = 1;
  y = voisin6(x, DEVANT, rs, ps, N); if ((y!=-1) && (F[y]==0)) a = 1;
  y = voisin6(x, DERRIERE, rs, ps, N); if ((y!=-1) && (F[y]==0)) d = 1;
  sum = n + s + e + o + a + d;
  if (sum == 0) return 0;
  else if (sum == 1) 
  {
    if (n)      return 45 - 1;
    else if (s) return 45 - 2;
    else if (e) return 45 - 3;
    else if (o) return 45 - 4;
    else if (a) return 45 - 5;
    else /* if (d) */ return 45 - 6;
  }
  else if (sum == 2) 
  {
    if (n && e)      return 45 - 7;
    else if (s && o) return 45 - 8;
    else if (e && a) return 45 - 9;
    else if (o && d) return 45 - 10;
    else if (a && n) return 45 - 11;
    else if (d && s) return 45 - 12;
    else if (n && o) return 45 - 13;
    else if (s && e) return 45 - 14;
    else if (e && d) return 45 - 15;
    else if (o && a) return 45 - 16;
    else if (a && s) return 45 - 17;
    else /* if (d && n) */ return 45 - 18;
  }
  else if (sum == 3) 
  {
    if (n && e && a) return 45 - 19;
    else if (s && o && d) return 45 - 20;
    else if (n && e && d) return 45 - 21;
    else if (s && o && a) return 45 - 22;
    else if (n && o && a) return 45 - 23;
    else if (s && e && d) return 45 - 24;
    else if (n && o && d) return 45 - 25;
    else /* if (s && e && a) */ return 45 - 26;
  }
  else if (sum == 4) 
  {
    if      (!n && !e) return 45 - 27;
    else if (!s && !o) return 45 - 28;
    else if (!e && !a) return 45 - 29;
    else if (!o && !d) return 45 - 30;
    else if (!a && !n) return 45 - 31;
    else if (!d && !s) return 45 - 32;
    else if (!n && !o) return 45 - 33;
    else if (!s && !e) return 45 - 34;
    else if (!e && !d) return 45 - 35;
    else if (!o && !a) return 45 - 36;
    else if (!a && !s) return 45 - 37;
    else /* if (!d && !n) */ return 45 - 38;
  }
  else if (sum == 5) 
  {
    if      (!n) return 45 - 39;
    else if (!s) return 45 - 40;
    else if (!e) return 45 - 41;
    else if (!o) return 45 - 42;
    else if (!a) return 45 - 43;
    else /* if (!d) */ return 45 - 44;
  }
  assert(1);
  return 0;
} /* typedir3d() */
#endif

#ifdef NBDIR27
/* ==================================== */
static int32_t typedir3d(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
// classe le point x selon ses 6-voisins qui sont hors de l'objet (nombre et direction)
{
  index_t y;
  int32_t n, s, e, o, a, d, sum;
  n = s = e = o = a = d = 0;
  y = voisin6(x, NORD, rs, ps, N); if ((y!=-1) && (F[y]==0)) n = 1;
  y = voisin6(x, SUD, rs, ps, N); if ((y!=-1) && (F[y]==0)) s = 1;
  y = voisin6(x, EST, rs, ps, N); if ((y!=-1) && (F[y]==0)) e = 1;
  y = voisin6(x, OUEST, rs, ps, N); if ((y!=-1) && (F[y]==0)) o = 1;
  y = voisin6(x, DEVANT, rs, ps, N); if ((y!=-1) && (F[y]==0)) a = 1;
  y = voisin6(x, DERRIERE, rs, ps, N); if ((y!=-1) && (F[y]==0)) d = 1;
  sum = n + s + e + o + a + d;
  if (sum == 0) return 0;
  else if (sum == 1) 
  {
    if (n)      return 27 - 1;
    else if (s) return 27 - 2;
    else if (e) return 27 - 3;
    else if (o) return 27 - 4;
    else if (a) return 27 - 5;
    else /* if (d) */ return 27 - 6;
  }
  else if (sum == 2) 
  {
    if (n && e)      return 27 - 7;
    else if (s && o) return 27 - 8;
    else if (e && a) return 27 - 9;
    else if (o && d) return 27 - 10;
    else if (a && n) return 27 - 11;
    else if (d && s) return 27 - 12;
    else if (n && o) return 27 - 13;
    else if (s && e) return 27 - 14;
    else if (e && d) return 27 - 15;
    else if (o && a) return 27 - 16;
    else if (a && s) return 27 - 17;
    else /* if (d && n) */ return 27 - 18;
  }
  else //if (sum >= 3) 
  {
    if (n && e && a) return 27 - 19;
    else if (s && o && d) return 27 - 20;
    else if (n && e && d) return 27 - 21;
    else if (s && o && a) return 27 - 22;
    else if (n && o && a) return 27 - 23;
    else if (s && e && d) return 27 - 24;
    else if (n && o && d) return 27 - 25;
    else if (s && e && a) return 27 - 26;
    else return 27; // all other cases : n-s-e, n-s-e-o, etc.
  }
} /* typedir3d() */
#endif

/* ==================================== */
static int32_t typedir3dlab(int32_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  index_t y;
  int32_t n, s, e, o, a, d, sum;
  n = s = e = o = a = d = 0;
  y = voisin6(x, NORD, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) n = 1;
  y = voisin6(x, SUD, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) s = 1;
  y = voisin6(x, EST, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) e = 1;
  y = voisin6(x, OUEST, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) o = 1;
  y = voisin6(x, DEVANT, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) a = 1;
  y = voisin6(x, DERRIERE, rs, ps, N); if ((y!=-1) && (F[y]!=F[x])) d = 1;
  sum = n + s + e + o + a + d;
  if (sum == 0) return 27 - 0;
  else if (sum == 1) 
  {
    if (n)      return 27 - 1;
    else if (s) return 27 - 2;
    else if (e) return 27 - 3;
    else if (o) return 27 - 4;
    else if (a) return 27 - 5;
    else /* if (d) */ return 27 - 6;
  }
  else if (sum == 2) 
  {
    if (n && e)      return 27 - 7;
    else if (s && o) return 27 - 8;
    else if (e && a) return 27 - 9;
    else if (o && d) return 27 - 10;
    else if (a && n) return 27 - 11;
    else if (d && s) return 27 - 12;
    else if (n && o) return 27 - 13;
    else if (s && e) return 27 - 14;
    else if (e && d) return 27 - 15;
    else if (o && a) return 27 - 16;
    else if (a && s) return 27 - 17;
    else /* if (d && n) */ return 27 - 18;
  }
  else //if (sum == 3) 
  {
    if (n && e && a) return 27 - 19;
    else if (s && o && d) return 27 - 20;
    else if (n && e && d) return 27 - 21;
    else if (s && o && a) return 27 - 22;
    else if (n && o && a) return 27 - 23;
    else if (s && e && d) return 27 - 24;
    else if (n && o && d) return 27 - 25;
    else /* if (s && e && a) */ return 27 - 26;
  }
} /* typedir3dlab() */

/* ==================================== */
static int32_t testabaisse4bin(uint8_t *F, index_t x, index_t rs, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple4(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse4bin() */

/* ==================================== */
static int32_t testabaisse8bin(uint8_t *F, index_t x, index_t rs, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple8(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse8bin() */

/* ==================================== */
static int32_t testabaisse6bin(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple6(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse6bin() */

/* ==================================== */
static int32_t testabaisse18bin(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple18(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse18bin() */

/* ==================================== */
static int32_t testabaisse26bin(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
#ifdef TEST_SIMPLE_PAR_COLLAPSE
  if (simple_26_att(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
#else
  if (mctopo3d_simple26(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
#endif
  return modifie;
} /* testabaisse26bin() */

/* ==================================== */
static int32_t testabaisse6lab(int32_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple6lab(F, x, rs, ps, N)) { modifie = 1; F[x] = 0; }
  return modifie;
} /* testabaisse6lab() */

/* ==================================== */
static int32_t testabaisse18lab(int32_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple18lab(F, x, rs, ps, N)) { modifie = 1; F[x] = 0; }
  return modifie;
} /* testabaisse18lab() */

/* ==================================== */
static int32_t testabaisse26lab(int32_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple26lab(F, x, rs, ps, N)) { modifie = 1; F[x] = 0; }
  return modifie;
} /* testabaisse26lab() */

/* ==================================== */
int32_t lskelubp(struct xvimage *image, 
              struct xvimage *imageprio, 
              int32_t connex, 
              int32_t val_inhibit)
/* ==================================== */
/* 
Squelette ultime 2D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.
Si le parametre 'inhibit' a une valeur differente de -1,
il indique une valeur de priorite telle que les points affectes de
cette valeur, ou plus, ne seront jamais selectionnes. 

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points ayant une valeur sup�rieure ou �gale � une valeur 
specifiee VAL_INHIBIT ne seront pas selectionnes.

On definit l'operateur Ultimate Thinning UT(F,P) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que P[x] < VAL_INHIBIT 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/

#undef F_NAME
#define F_NAME "lskelubp"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  index_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be int32_t\n", F_NAME);
    fprintf(stderr, "    otherwise, use inhibit map\n");
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (P[x] < val_inhibit) && bordext8(F, x, rs, N))
    {
      mcrbt_RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse4bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] < val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] < val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }


  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelubp() */

/* ==================================== */
int32_t lskelubp2(struct xvimage *image, 
               struct xvimage *imageprio, 
               int32_t connex, 
               struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelubp2"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  if (imageinhib != NULL) I = UCHARDATA(imageinhib);

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (!I || !I[x]) && bordext8(F, x, rs, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse4bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelubp2() */

/* ==================================== */
int32_t lskelubp3d(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 int32_t val_inhibit)
/* ==================================== */
/* 
Squelette ultime 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.
Si le parametre 'inhibit' a une valeur differente de -1,
il indique une valeur de priorite telle que les points affectes de
cette valeur, ou plus, ne seront jamais selectionnes. 

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points ayant une valeur sup�rieure ou �gale � une valeur 
specifiee VAL_INHIBIT ne seront pas selectionnes.

On definit l'operateur Ultimate Thinning UT(F,P) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que P[x] < VAL_INHIBIT 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelubp3d"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t d = depth(image);
  index_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  index_t taillemaxrbt;

  IndicsInit(N);

  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != d))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be int32_t\n", F_NAME);
    fprintf(stderr, "    otherwise, use inhibit map\n");
    return(0);
  }
  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (P[x] < val_inhibit) && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      mcrbt_RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] < val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse26bin(F, x, rs, ps, N))         /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] < val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelubp3d() */

/* ==================================== */
int32_t lskelubp3d2(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelubp3d2"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);
  index_t N = ds * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;

  IndicsInit(N);

  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }
  if (imageinhib != NULL) 
  {
    COMPARE_SIZE(image, imageinhib);
    ACCEPTED_TYPES1(imageinhib, VFF_TYP_1_BYTE);
    I = UCHARDATA(imageinhib);
  }
  COMPARE_SIZE(image, imageprio);
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 

  taillemaxrbt = 2 * rs * cs +  2 * rs * ds +  2 * ds * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && (!I || !I[x]) && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse26bin(F, x, rs, ps, N))         /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelubp3d2() */

/* ==================================== */
int32_t lskelubp3d2lab(struct xvimage *image, 
		       struct xvimage *imageprio, 
		       int32_t connex, 
		       struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelubp3d2lab"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);
  index_t N = ds * ps;              /* taille image */
  int32_t *F = SLONGDATA(image);   /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;

  IndicsInit(N);

  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }
  if (imageinhib != NULL) 
  {
    COMPARE_SIZE(image, imageinhib);
    ACCEPTED_TYPES1(imageinhib, VFF_TYP_1_BYTE);
    I = UCHARDATA(imageinhib);
  }
  COMPARE_SIZE(image, imageprio);
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  ACCEPTED_TYPES1(image, VFF_TYP_4_BYTE);
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 

  taillemaxrbt = 2 * rs * cs +  2 * rs * ds +  2 * ds * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (!I || !I[x]) && mctopo3d_simple6lab(F, x, rs, ps, N))
      {
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
	Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (!I || !I[x]) && mctopo3d_simple26lab(F, x, rs, ps, N))
      {
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
	Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6lab(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6lab(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse26lab(F, x, rs, ps, N))         /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (!I || !I[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26lab(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelubp3d2lab() */

/* ==================================== */
int32_t lskelcurv(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette curviligne 2D binaire guide par une image de priorites, bas� sur les isthmes 1D
Les valeurs les plus basses correspondent a la plus grande priorite.

On definit l'operateur Curvilinear Thinning CT(F,P) : 
C = {y in F | T(y) > 1}
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C = C u {y}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelcurv"
{ 
  int32_t k, t, tb;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  Fifo * FIFO1;
  Fifo * FIFO2;
  int32_t prio, oldprio;
  index_t taillemaxrbt;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != 1))
    {
      fprintf(stderr, "%s: bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s: datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  FIFO1 = CreeFifoVide(N);
  if (FIFO1 == NULL)
  {
    fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
    return(0);
  }
  FIFO2 = CreeFifoVide(N);
  if (FIFO2 == NULL)
  {
    fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  if (connex == 4)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord(x,rs,N)))
      {
	top4(F, x, rs, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
    }
  }
  else 
  if (connex == 8)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord(x,rs,N)))
      {
	top8(F, x, rs, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
    }
  }

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 4)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext4(F, x, rs, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*10+typedir2d(F,x,rs,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*10+typedir2d(F,x,rs,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*10+typedir2d(F,x,rs,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
#ifdef DEBUG_lskelcurv
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir2d(F, x, rs, N));
#endif
      }
    }
  }
  else if (connex == 8)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext8(F, x, rs, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*10+typedir2d(F,x,rs,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*10+typedir2d(F,x,rs,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*10+typedir2d(F,x,rs,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
#ifdef DEBUG_lskelcurv
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir2d(F, x, rs, N));
#endif
      }
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = mcrbt_RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x,CONTRAINTE)) && testabaisse4bin(F, x, rs, N))
        {
          for (k = 0; k < 8; k += 2)          /* parcourt les voisins en 4-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin(x, k, rs, N);                             /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*10+typedir2d(F,y,rs,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*10+typedir2d(F,y,rs,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*10+typedir2d(F,y,rs,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x,CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top4(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x,CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else if (connex == 8)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = mcrbt_RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x,CONTRAINTE)) && testabaisse8bin(F, x, rs, N))
        {
          for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin(x, k, rs, N);                             /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*10+typedir2d(F,y,rs,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*10+typedir2d(F,y,rs,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*10+typedir2d(F,y,rs,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse8bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x,CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top8(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x,CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  FifoTermine(FIFO1);
  FifoTermine(FIFO2);
  return(1);
} /* lskelcurv() */

/* ==================================== */
int32_t lskelcurv3d(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette curviligne 3D binaire guide par une image de priorites, bas� sur les isthmes 1D
Les valeurs les plus basses correspondent a la plus grande priorite.

De facon tres schematique,
on definit l'operateur Curvilinear Thinning CT(F,P) : 
C = {y in F | T(y) > 1}
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C = C u {y}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelcurv3d"
{ 
  int32_t k, t, tb;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  int32_t prio, oldprio;
  index_t taillemaxrbt;

  IndicsInit(N);
  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }
  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != ds))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != ds))
    {
      fprintf(stderr, "%s: bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s: datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
    }
  }
  else 
  if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
    }
  }

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && mctopo3d_bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*30+typedir3d(F,x,rs,ps,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*30+typedir3d(F,x,rs,ps,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    Fifo * FIFO1;
    Fifo * FIFO2;
    FIFO1 = CreeFifoVide(N/2);
    if (FIFO1 == NULL)
    {
      fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
      return(0);
    }
    FIFO2 = CreeFifoVide(N/2);
    if (FIFO2 == NULL)
    {
      fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
      return(0);
    }

    while (!mcrbt_RbtVide(RBT))
    {
      prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = mcrbt_RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x,CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
        {
          for (k = 0; k <= 10; k += 2)        /* parcourt les voisins en 6-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin6(x, k, rs, ps, N);                        /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x,CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
          if (t > 1) Set(x,CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!mcrbt_RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x,CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y,CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              mctopo3d_top26(F, y, rs, ps, N, &t, &tb);
              if (t > 1) 
	      {
                Set(y,CONTRAINTE);
	      }
	    }
            if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelcurv3d() */

/* ==================================== */
int32_t lskelsurf3d(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette surfacique 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

De facon tres schematique,
on definit l'operateur Surfacic Thinning ST(F,P) : 
C = {y in F | Tb(y) > 1}
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si Tb(y) > 1 alors C[y] = 1
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelsurf3d"
{ 
  int32_t k, t, tb;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  int32_t prio, oldprio;
  index_t taillemaxrbt;

  IndicsInit(N);
  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != ds))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  if (inhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(inhibit) != rs) || (colsize(inhibit) != cs) || (depth(inhibit) != ds))
    {
      fprintf(stderr, "%s: bad size for inhibit\n", F_NAME);
      return(0);
    }
    if (datatype(inhibit) == VFF_TYP_1_BYTE) 
      I = UCHARDATA(inhibit);
    else 
    {
      fprintf(stderr, "%s: datatype(inhibit) must be uint8_t\n", F_NAME);
      return(0);
    }
    for (x = 0; x < N; x++)
      if (I[x]) Set(x,CONTRAINTE);
  }

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
	if (tb > 1) Set(x,CONTRAINTE);
      }
    }
  }
  else 
  if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (tb > 1) Set(x,CONTRAINTE);
      }
    }
  }

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */


  if (connex == 6)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && mctopo3d_bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*30+typedir3d(F,x,rs,ps,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*30+typedir3d(F,x,rs,ps,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*30+typedir3d(F,x,rs,ps,N),x); break;
	}
#else
	switch(datatype(imageprio))
	{
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
	}
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    Fifo * FIFO1;
    Fifo * FIFO2;
    FIFO1 = CreeFifoVide(N/2);
    if (FIFO1 == NULL)
    {
      fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
      return(0);
    }
    FIFO2 = CreeFifoVide(N/2);
    if (FIFO2 == NULL)
    {
      fprintf(stderr, "%s: CreeFifoVide failed\n", F_NAME);
      return(0);
    }

    while (!mcrbt_RbtVide(RBT))
    {
      prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = mcrbt_RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)mcrbt_RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x,CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
        {
          for (k = 0; k <= 10; k += 2)        /* parcourt les voisins en 6-connexite */
          {                                              /* pour empiler les voisins */
            y = voisin6(x, k, rs, ps, N);                        /* non deja empiles */
            if ((y != -1) && (F[y]))
            {
              if (! IsSet(y,EN_FIFO))
	      {
                FifoPush(FIFO2, y);
                Set(y, EN_FIFO);
	      }
              if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
                Set(y, EN_RBT);
	      }
            } /* if y */
          } /* for k */      
        } /* if (testabaisse4bin(F, x, rs, N)) */
      } // while (!FifoVide(FIFO1))

      while (!FifoVide(FIFO2))
      {
        x = FifoPop(FIFO2);
        UnSet(x,EN_FIFO);
        if ((! IsSet(x,CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
          if (tb > 1) Set(x,CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!mcrbt_RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x,CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y,CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              mctopo3d_top26(F, y, rs, ps, N, &t, &tb);
              if (tb > 1) 
	      {
                Set(y,CONTRAINTE);
	      }
	    }
            if ((! IsSet(y,CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*30+typedir3d(F,y,rs,ps,N),y); break;
		}
#else
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
		case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
		}
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelsurf3d() */

/* ==================================== */
int32_t ltoposhrink(struct xvimage *image, 
              struct xvimage *imageprio, 
              int32_t connex, 
              int32_t tmin, 
              int32_t tmax, 
              int32_t tbmin, 
              int32_t tbmax, 
              struct xvimage *imageinhibit)
/* ==================================== */
/* 
Amincissement 2D binaire avec controle topologique.

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points de l'image I (imageinhibit) ne seront
pas selectionnes.

repeter jusqu'a stabilite
  choisir un point x de F \ I tel que (tmin <= T(x) <= tmax) et (tbmin <= Tb(x) <= tbmax)
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "ltoposhrink"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  int32_t t, tb;

  IndicsInit(N);

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != 1))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  if (imageinhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != 1))
    {
      fprintf(stderr, "%s: bad size for imageinhibit\n", F_NAME);
      return(0);
    }
    if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
    {
      fprintf(stderr, "%s: bad type for imageinhibit\n", F_NAME);
      return(0);
    }
    I = UCHARDATA(imageinhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x,CONTRAINTE) && bordext8(F, x, rs, N))
    {
      switch(datatype(imageprio))
      {
      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top4(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y,CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	    case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	    case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	    case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	    case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top8(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y,CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	    case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	    case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	    case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	    case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }


  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* ltoposhrink() */

/* ==================================== */
int32_t ltoposhrink3d(struct xvimage *image, 
                 struct xvimage *imageprio, 
                 int32_t connex, 
                 int32_t tmin, 
                 int32_t tmax, 
                 int32_t tbmin, 
                 int32_t tbmax, 
                 struct xvimage *imageinhibit)
/* ==================================== */
/* 
Amincissement 3D binaire avec controle topologique.

Soit F l'objet initial.
Soit P une fonction de D dans l'ensemble des entiers, 
P specifie un ordre de priorite dans le traitement des points. 
Les points ayant les plus basses valeurs sont traites en priorite.
Les points de l'image I (imageinhibit) ne seront
pas selectionnes.

repeter jusqu'a stabilite
  choisir un point x de F \ I tel que (tmin <= T(x) <= tmax) et (tbmin <= Tb(x) <= tbmax)
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "ltoposhrink3d"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t d = depth(image);
  index_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  int32_t t, tb;

  IndicsInit(N);

  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }

  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != d))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  if (imageinhibit != NULL)
  {
    uint8_t *I;
    if ((rowsize(imageinhibit) != rs) || (colsize(imageinhibit) != cs) || (depth(imageinhibit) != d))
    {
      fprintf(stderr, "%s: bad size for imageinhibit\n", F_NAME);
      return(0);
    }
    if (datatype(imageinhibit) != VFF_TYP_1_BYTE) 
    {
      fprintf(stderr, "%s: bad type for imageinhibit\n", F_NAME);
      return(0);
    }
    I = UCHARDATA(imageinhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x,CONTRAINTE) && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y,CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	    case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	    case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	    case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	    case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 18)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top18(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y,CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	    case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	    case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	    case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	    case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 18) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y,CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	    case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	    case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	    case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	    case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* ltoposhrink3d() */

/* ==================================== */
int32_t lskeleucl(struct xvimage *image, 
              int32_t connex, 
              struct xvimage *imageinhib)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskeleucl"
{ 
  int32_t k;
  index_t x, y;                 /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ds * ps;             /* taille image */
  struct xvimage *imageprio;
  struct xvimage *imagedist;
  double *P = NULL;     /* l'image de priorites */
  double *D = NULL;     /* l'image de distances */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  Rbt *RBT;
  index_t taillemaxrbt;
  double prio;

  IndicsInit(N);

  imageprio = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  imagedist = allocimage(NULL, rs, cs, ds, VFF_TYP_DOUBLE);
  if (imageinhib == NULL)
  {
    imageinhib = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    razimage(imageinhib);
  }
  if ((imagedist == NULL) || (imageprio == NULL)  || (imageinhib == NULL))
  {   
    fprintf(stderr, "%s(): allocimage failed\n", F_NAME);
    return 0;
  }
  P = DOUBLEDATA(imageprio); 
  D = DOUBLEDATA(imagedist); 
  I = UCHARDATA(imageinhib);
 
  if (!ldistMeijster(image, imagedist))
  {   
    fprintf(stderr, "%s(): ldistMeijster failed\n", F_NAME);
    return 0;
  }

  copy2image(imageprio, imagedist);

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (ds == 1) // 2D
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (!I[x]) && bordext8(F, x, rs, N))
      {
	mcrbt_RbtInsert(&RBT, D[x], x);
	Set(x,EN_RBT);
      }
      if (F[x] && I[x])
      {
	for (k = 0; k < 8; k += 1)
        {
	  y = voisin(x, k, rs, N);
	  if ((y != -1) && F[y] && (!I[y]) && (D[y] > D[x]))
	  {
	    prio = D[x] + (D[y] - D[x]) / 
	           dist2((double)(y%rs), (double)(y/rs), (double)(x%rs), (double)(x/rs));
	    mcrbt_RbtInsert(&RBT, prio, y);
	    P[y] = prio;
	    Set(y,EN_RBT);
	  }
	} // for k
      } // if (F[x] && I[x])
    } // for x
  } // if (ds == 1)
  else // 3D
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && (!I[x]) && mctopo3d_bordext26(F, x, rs, ps, N))
      {
	mcrbt_RbtInsert(&RBT, D[x], x);
	Set(x,EN_RBT);
      }
      if (F[x] && I[x])
      {
	for (k = 0; k < 26; k += 1)
        {
	  y = voisin26(x, k, rs, ps, N);
	  if ((y != -1) && F[y] && (!I[y]) && (D[y] > D[x]))
	  {
	    prio = D[x] + (D[y] - D[x]) / 
	           dist3((double)(y%rs), (double)((y%ps)/rs), (double)(y/ps), 
			 (double)(x%rs), (double)((x%ps)/rs), (double)(x/ps)); 
	    mcrbt_RbtInsert(&RBT, prio, y);
	    P[y] = prio;
	    Set(y,EN_RBT);
	  }
	} // for k
      } // if (F[x] && I[x])
    } // for x
  } // 3D

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if ((connex == 4) || (connex == 6) || (connex == 18))
  {
    fprintf(stderr, "%s(): connex %d not yet implemented\n", F_NAME, connex);
    return 0;
  } /* if ((connex == 4) ... */
  else
  if (connex == 8)
  {
    if (ds > 1)
    {
      fprintf(stderr, "%s: bad value for connex in 3D : 8\n", F_NAME);
      return(0);
    }

    while (!mcrbt_RbtVide(RBT))
    {
      prio = mcrbt_RbtMinLevel(RBT); 
      x = mcrbt_RbtPopMin(RBT);
      if (I[x]) goto finwhile;
      if (prio < P[x]) goto finwhile;
      if (testabaisse8bin(F, x, rs, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && F[y] && (!I[y]) && (!IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, D[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
      else
      {
        I[x] = 1; // le point x ne sera plus consid�r�, il fait partie du squelette
        for (k = 0; k < 8; k += 1)
        {
          y = voisin(x, k, rs, N);
          if ((y != -1) && F[y] && (!I[y]) && (D[y] > D[x]))
	  {
	    prio = D[x] + (D[y] - D[x]) / dist2((double)(y%rs), (double)(y/rs), (double)(x%rs), (double)(x/rs));
	    if (prio < P[y]) 
	    { 
	      mcrbt_RbtInsert(&RBT, prio, y);
	      P[y] = prio;
	    }
	  } // if y
        } // for k
      } // else if (D[x] >= r)
    finwhile:;
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  if (connex == 26)
  {
    mctopo3d_init_topo3d();
    while (!mcrbt_RbtVide(RBT))
    {
      prio = mcrbt_RbtMinLevel(RBT); 
      x = mcrbt_RbtPopMin(RBT);
      if (I[x]) goto finwhile26;
      if (prio < P[x]) goto finwhile26;
      if (testabaisse26bin(F, x, rs, ps, N))          /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && F[y] && (!I[y]) && (!IsSet(y, EN_RBT)))
          {
            mcrbt_RbtInsert(&RBT, D[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
      else 
      {
        I[x] = 1; // le point x ne sera plus consid�r�, il fait partie du squelette
        for (k = 0; k < 26; k += 1)
        {
          y = voisin26(x, k, rs, ps, N);
          if ((y != -1) && F[y] && (!I[y]) && (D[y] > D[x]))
	  {
	    prio = D[x] + (D[y] - D[x]) / 
	           dist3((double)(y%rs), (double)((y%ps)/rs), (double)(y/ps), 
			 (double)(x%rs), (double)((x%ps)/rs), (double)(x/ps)); 
	    if (prio < P[y]) 
	    { 
	      mcrbt_RbtInsert(&RBT, prio, y);
	      P[y] = prio;
	    }
	  } // if y
        } // for k
      } // else
    finwhile26:;
    } /* while (!mcrbt_RbtVide(RBT)) */
    mctopo3d_termine_topo3d();
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex : %d\n", F_NAME, connex);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  freeimage(imageprio);
  freeimage(imagedist);
  return(1);
} /* lskeleucl() */

#define IsEnd(x) (endpoint[x/8]&(1<<(x%8)))

/* codage des points du cube 3x3 : 
   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26

z  0  0  0  0  0  0  0  0  0  1  1  1  1  1  1  1  1  1  2  2  2  2  2  2  2  2  2
y  0  0  0  1  1  1  2  2  2  0  0  0  1  1  1  2  2  2  0  0  0  1  1  1  2  2  2  
x  0  1  2  0  1  2  0  1  2  0  1  2  0  1  2  0  1  2  0  1  2  0  1  2  0  1  2 */ 

/* ========================================== */
uint32_t encodevois(index_t p, uint8_t *F, index_t rs, index_t ps, index_t N)
/* ========================================== */
#undef F_NAME
#define F_NAME "encodevois"
{
  uint32_t i = 0;
  int32_t n = 0;

  if (bord3d(p, rs, ps, N)) 
  {
    fprintf(stderr, "%s : voxel must not hit the frame\n", F_NAME);
    exit(0);
  }

  if (F[p-ps-rs-1]) i = i | (1<<n); n++;
  if (F[p-ps-rs  ]) i = i | (1<<n); n++;
  if (F[p-ps-rs+1]) i = i | (1<<n); n++;
  if (F[p-ps   -1]) i = i | (1<<n); n++;
  if (F[p-ps     ]) i = i | (1<<n); n++;
  if (F[p-ps   +1]) i = i | (1<<n); n++;
  if (F[p-ps+rs-1]) i = i | (1<<n); n++;
  if (F[p-ps+rs  ]) i = i | (1<<n); n++;
  if (F[p-ps+rs+1]) i = i | (1<<n); n++;

  if (F[p   -rs-1]) i = i | (1<<n); n++;
  if (F[p   -rs  ]) i = i | (1<<n); n++;
  if (F[p   -rs+1]) i = i | (1<<n); n++;
  if (F[p      -1]) i = i | (1<<n); n++;
  if (F[p        ]) i = i | (1<<n); n++;
  if (F[p      +1]) i = i | (1<<n); n++;
  if (F[p   +rs-1]) i = i | (1<<n); n++;
  if (F[p   +rs  ]) i = i | (1<<n); n++;
  if (F[p   +rs+1]) i = i | (1<<n); n++;

  if (F[p+ps-rs-1]) i = i | (1<<n); n++;
  if (F[p+ps-rs  ]) i = i | (1<<n); n++;
  if (F[p+ps-rs+1]) i = i | (1<<n); n++;
  if (F[p+ps   -1]) i = i | (1<<n); n++;
  if (F[p+ps     ]) i = i | (1<<n); n++;
  if (F[p+ps   +1]) i = i | (1<<n); n++;
  if (F[p+ps+rs-1]) i = i | (1<<n); n++;
  if (F[p+ps+rs  ]) i = i | (1<<n); n++;
  if (F[p+ps+rs+1]) i = i | (1<<n); n++;

  return i;
} /* encodevois() */

/* ==================================== */
int32_t lskelend3d_sav(struct xvimage *image, 
	       struct xvimage *imageprio, 
	       int32_t connex, 
	       uint8_t *endpoint)
/* ==================================== */
/* 
Squelette curviligne ou surfacique 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

Le pr�dicat "endpoint" est d�fini par un tableau de 2^27 bool�ens 
(cod�s sur 2^24 octets) pass� en param�tre.

De facon tres schematique,
on definit l'operateur EndThinning CT(F,P,E) : 
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que E(x) == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
fin repeter 
resultat: F
*/
#undef F_NAME
#define F_NAME "lskelend3d"
{ 
  int32_t k;
  index_t x;                       /* index de pixel */
  index_t y;                       /* index (generalement un voisin de x) */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint32_t config;

//#define VERIF
#ifdef VERIF
{ int32_t i, s = 1 << 27;
  for (i = 0; i < s; i++)
    if (IsEnd(i)) printf("Is End: %x\n", i);
}
#endif

  IndicsInit(N);
  mctopo3d_init_topo3d();

  if (imageprio == NULL)
  {
    fprintf(stderr, "%s: imageprio is needed\n", F_NAME);
    return(0);
  }
  if ((rowsize(imageprio) != rs) || (colsize(imageprio) != cs) || (depth(imageprio) != ds))
  {
    fprintf(stderr, "%s: bad size for imageprio\n", F_NAME);
    return(0);
  }
  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be int32_t\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    fprintf(stderr, "%s: Connex 6 not implemented\n", F_NAME);
    return(0);
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        mcrbt_RbtInsert(&RBT, P[x]*30 + typedir3d(F, x, rs, ps, N), x);
#else
        mcrbt_RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
  } /* if (connex == 6) */
  else if (connex == 26)
  {           
    while (!mcrbt_RbtVide(RBT))
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      config = encodevois(x, F, rs, ps, N);

      if ((!IsEnd(config)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
#ifdef PRIODIR
	    mcrbt_RbtInsert(&RBT, P[y]*30 + typedir3d(F, y, rs, ps, N), y);
#else
	    mcrbt_RbtInsert(&RBT, P[y], y);
#endif
	    Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelend3d_sav() */

/* ==================================== */
int32_t lskelendcurv3d(struct xvimage *image, 
		   int32_t connex, 
		   int32_t niseuil)
/* ==================================== */
#undef F_NAME
#define F_NAME "lskelendcurv3d"
{ 
  uint8_t *endpoint;
  char tablefilename[128];
  int32_t tablesize, ret;
  FILE *fd;

    tablesize = 1<<24;
    endpoint = (uint8_t *)malloc(tablesize);
    if (! endpoint)
    {
      fprintf(stderr, "%s: malloc failed\n", F_NAME);
      return 0;
    }
    
    sprintf(tablefilename, "%s/src/tables/TabEndPoints.txt", getenv("PINK"));
    fd = fopen (tablefilename, "r");
    if (fd == NULL) 
    {   
      fprintf(stderr, "%s: error while opening table\n", F_NAME);
      return 0;
    }
    ret = fread(endpoint, sizeof(char), tablesize, fd);
    if (ret != tablesize)
    {
      fprintf(stderr,"%s : fread failed : %d asked ; %d read\n", F_NAME, tablesize, ret);
      return 0;
    }
    fclose(fd);

    if (! lskelend3d(image, connex, endpoint, niseuil))
    {
      fprintf(stderr, "%s: lskelend3d failed\n", F_NAME);
      return 0;
    }
    //    freeimage(prio);
    free(endpoint);
    return 1;
} // lskelendcurv3d()

/* ==================================== */
int32_t lskelend3d(struct xvimage *image, 
		   int32_t connex, 
		   uint8_t *endpoint,
		   int32_t niseuil)
/* ==================================== */
/* 
Squelette curviligne ou surfacique 3D binaire.

Algo par passes directionnelles.

Le pr�dicat "endpoint" est d�fini par un tableau de 2^27 bool�ens 
(cod�s sur 2^24 octets) pass� en param�tre.
*/
#undef F_NAME
#define F_NAME "lskelend3d"
{ 
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint32_t config;

//#define VERIF
#ifdef VERIF
{ int32_t i, s = 1 << 27;
  for (i = 0; i < s; i++)
    if (IsEnd(i)) printf("Is End: %x\n", i);
}
#endif

#ifdef DEBUG_lskelend3d
 printf("%s: begin niseuil=%d\n", F_NAME, niseuil);
#endif

  IndicsInit(N);
  mctopo3d_init_topo3d();

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple6(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
  }
  else if (connex == 18)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple18(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple26(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	config = encodevois(x, F, rs, ps, N);
	if (((nbiter < niseuil) || (!IsEnd(config))) && testabaisse6bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple6(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else if (connex == 18)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	config = encodevois(x, F, rs, ps, N);
	if (((nbiter < niseuil) || (!IsEnd(config))) && testabaisse18bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple18(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 18) */
  else if (connex == 26)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
#ifdef DEBUG_lskelend3d
	{ int32_t lev = mcrbt_RbtMinLevel(RBT);
	printf("pop: prio %d ", lev); }
#endif	
	x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelend3d
	printf("; point %d (%d,%d,%d)\n", x, x % rs, (x % ps) / rs, x / ps);
#endif
	config = encodevois(x, F, rs, ps, N);
	if (((nbiter < niseuil) || (!IsEnd(config))) && testabaisse26bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple26(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
#ifdef DEBUG_lskelend3d
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelend3d() */

/* ==================================== */
int32_t lskelend2d(struct xvimage *image, 
		   int32_t connex, 
		   int32_t niseuil)
/* ==================================== */
/* 
Squelette curviligne ou surfacique 2D binaire.
Algo par passes directionnelles.
*/
#undef F_NAME
#define F_NAME "lskelend2d"
{ 
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Rbt * RBT;
  index_t taillemaxrbt;

#ifdef DEBUG_lskelend2d
 printf("%s: begin niseuil=%d\n", F_NAME, niseuil);
#endif

  IndicsInit(N);

  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 4)
  {
    for (x = 0; x < N; x++)
      if (F[x] && simple4(F, x, rs, N))
        mcrbt_RbtInsert(&RBT, typedir2d(F, x, rs, N), x);
  }
  else if (connex == 8)
  {
    for (x = 0; x < N; x++)
      if (F[x] && simple8(F, x, rs, N))
        mcrbt_RbtInsert(&RBT, typedir2d(F, x, rs, N), x);
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	if (((nbiter < niseuil) || (nbvois4(F, x, rs, N) != 1)) && testabaisse4bin(F, x, rs, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && simple4(F, x, rs, N))
          mcrbt_RbtInsert(&RBT, typedir2d(F, x, rs, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else // if (connex == 8)
  {
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	if (((nbiter < niseuil) || (nbvois8(F, x, rs, N) != 1)) && testabaisse8bin(F, x, rs, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && simple8(F, x, rs, N))
          mcrbt_RbtInsert(&RBT, typedir2d(F, x, rs, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelend2d() */

/* ==================================== */
int32_t lskelendcurvlab3d(struct xvimage *image, 
			  int32_t connex, 
			  int32_t niseuil)
/* ==================================== */
/* 
Squelette curviligne 3D sur une image de labels (chaque label est trait� comme une image binaire).

Algo par passes directionnelles.
*/
#undef F_NAME
#define F_NAME "lskelendcurvlab3d"
{ 
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  int32_t *F = SLONGDATA(image);      /* l'image de depart */
  Rbt * RBT;
  index_t taillemaxrbt;

#ifdef DEBUG_lskelendcurvlab3d
 printf("%s: begin niseuil=%d\n", F_NAME, niseuil);
#endif

  assert(datatype(image) == VFF_TYP_4_BYTE);

  IndicsInit(N);
  mctopo3d_init_topo3d();

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt); assert(RBT != NULL);

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  if (connex == 6)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple6lab(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
  }
  else if (connex == 18)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple18lab(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
      if (F[x] && mctopo3d_simple26lab(F, x, rs, ps, N))
        mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
  }
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	if (((nbiter < niseuil) || (mctopo3d_nbvoislab6(F, x, rs, ps, N) > 1)) && testabaisse6lab(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple6lab(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else if (connex == 18)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	if (((nbiter < niseuil) || (mctopo3d_nbvoislab18(F, x, rs, ps, N) > 1)) && testabaisse18lab(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple18lab(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 18) */
  else if (connex == 26)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!mcrbt_RbtVide(RBT))
      {
	x = mcrbt_RbtPopMin(RBT);
	if (((nbiter < niseuil) || (mctopo3d_nbvoislab26(F, x, rs, ps, N) > 1)) && testabaisse26lab(F, x, rs, ps, N)) nbdel++;
      } /* while (!mcrbt_RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && mctopo3d_simple26lab(F, x, rs, ps, N))
          mcrbt_RbtInsert(&RBT, typedir3dlab(F, x, rs, ps, N), x);
#ifdef VERBOSE
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
#ifdef DEBUG_lskelendcurvlab3d
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
#endif
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  return(1);
} /* lskelendcurvlab3d() */

// ========================================================
// ========================================================
// ALGO DIRECTIONEL SEQUENTIEL
// ========================================================
// ========================================================

/* ==================================== */
int32_t lskeldir3d(struct xvimage *image, 
		   struct xvimage *inhibit, 
		   int32_t connex, 
		   int32_t nsteps)
/* ==================================== */
/* 
Squelette curviligne 3D binaire - algo directionel s�quentiel bas� sur les isthmes 1D

Variante 0 : d�tection des isthmes au niveau de l'it�ration

C = {}
r�p�ter jusqu'� stabilit�
  C = C u {x in F | T(x) > 1}
  B = {}
  pour tout point x simple pour F
    D[x] = typedir3d(x) ; B = B u {x}
  fin pour
  pour toutes les directions d de 0 � NBDIR
    pour tout x de B\C, simple pour F tel que D[x] = d
      F = F \ {x}
    fin pour
  fin pour
fin r�p�ter 
r�sultat: F

Impl�mentation non optimis�e (NBDIR scans de la liste des points de bord)
*/
#undef F_NAME
#define F_NAME "lskeldir3d"
{ 
  int32_t i, t, tb, dir, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  index_t n;                       /* taille liste */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  uint8_t *D;                      /* pour les directions */
  Liste * LISTE1; // repr�sentation de l'ensemble B (pts de bord) 

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  if (connex != 26)
  {
    fprintf(stderr, "%s: connex %d not implemented\n", F_NAME, connex);
    return(0);
  }

  if (nsteps == -1) nsteps = 1000000000;

  D = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(D != NULL);
  LISTE1 = CreeListeVide(N);
  if (LISTE1 == NULL)
  {
    fprintf(stderr, "%s: CreeListeVide failed\n", F_NAME);
    return(0);
  }
  IndicsInit(N);
  mctopo3d_init_topo3d();

  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  if (connex == 26)
  {
#ifdef DEBUG_lskeldir3d
    char DBGBUF[100];
#endif

    n = 0;
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
	if ((tb == 1) && (t == 1) && !IsSet(x,CONTRAINTE))
	{ 
	  ListePush(LISTE1, x); 
	  n++;
	  D[x] = typedir3d(F, x, rs, ps, N);
	}
      }
    }

    while (!ListeVide(LISTE1) && (nbiter < nsteps))
    {
      nbiter++;

#ifdef VERBOSE
      printf("%s: nbiter %d ; nb simple points %d\n", F_NAME, nbiter, n);
#endif
      for (dir = 0; dir <= NBDIR; dir++)
      {
#ifdef VERBOSE2
      printf("%s: subiter %d\n", F_NAME, dir);
#endif
	for (i = 0; i < n; i++)
	{
	  x = ListeElt(LISTE1, i);
	  if (F[x] && (D[x] == dir) && !IsSet(x,CONTRAINTE))
	    (void)testabaisse26bin(F, x, rs, ps, N);
	} // for (i = 0; i < n; i++)
#ifdef DEBUG_lskeldir3d
sprintf(DBGBUF, "_dir%d", dir);
writeimage(image, DBGBUF);
#endif
      } // for (dir = 1; dir <= NBDIR; dir++)

      ListeFlush(LISTE1);
      n = 0;
      for (x = 0; x < N; x++)
      {
	if (F[x] && (nonbord3d(x,rs,ps,N)))
	{
	  mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	  if (t > 1) Set(x,CONTRAINTE);
	  if ((tb == 1) && (t == 1) && !IsSet(x,CONTRAINTE))
	  { 
	    ListePush(LISTE1, x); 
	    n++;
	    D[x] = typedir3d(F, x, rs, ps, N);
	  } // if ((tb == 1) && (t == 1))
	} // if (F[x] && (nonbord3d(x,rs,ps,N)))
      } // for (x = 0; x < N; x++)
    } // while (!ListeVide(LISTE1))
  } // if (connex == 26)

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  free(D);
  ListeTermine(LISTE1);
  IndicsTermine();
  mctopo3d_termine_topo3d();
  return(1);
} /* lskeldir3d() */

/* ==================================== */
int32_t lskeldir3d_1(struct xvimage *image, 
		   struct xvimage *inhibit, 
		   int32_t connex, 
		   int32_t nsteps)
/* ==================================== */
/* 
Squelette curviligne 3D binaire - algo directionel s�quentiel bas� sur les isthmes 1D

Variante 1 : d�tection des isthmes au niveau de la sous-it�ration

C = {x in F | T(x) > 1}
r�p�ter jusqu'� stabilit�
  B = {}
  pour tout point x simple pour F
    D[x] = typedir3d(x) ; B = B u {x}
  fin pour
  pour toutes les directions d de 0 � NBDIR
    pour tout x de B\C 
      si T(x) > 1 alors C = C u {x}
    fin pour
    pour tout x de B\C, simple pour F tel que D[x] = d
      F = F \ {x}
    fin pour
  fin pour
fin r�p�ter 
r�sultat: F

Impl�mentation non optimis�e (NBDIR scans de la liste des points de bord)
*/
#undef F_NAME
#define F_NAME "lskeldir3d_1"
{ 
  int32_t i, t, tb, dir, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ds = depth(image);       /* nb plans */
  index_t ps = rs * cs;            /* taille plan */
  index_t N = ps * ds;             /* taille image */
  index_t n;                       /* taille liste */
  uint8_t *F = UCHARDATA(image);   /* l'image de depart */
  uint8_t *D;                      /* pour les directions */
  Liste * LISTE1; // repr�sentation de l'ensemble B (pts de bord) 

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);
  if (connex != 26)
  {
    fprintf(stderr, "%s: connex %d not implemented\n", F_NAME, connex);
    return(0);
  }

  if (nsteps == -1) nsteps = 1000000000;

  D = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(D != NULL);
  LISTE1 = CreeListeVide(N);
  if (LISTE1 == NULL)
  {
    fprintf(stderr, "%s: CreeListeVide failed\n", F_NAME);
    return(0);
  }
  IndicsInit(N);
  mctopo3d_init_topo3d();

  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  if (connex == 26)
  {
#ifdef DEBUG_lskeldir3d
    char DBGBUF[100];
#endif

    n = 0;
    for (x = 0; x < N; x++)
    {
      if (F[x] && (nonbord3d(x,rs,ps,N)))
      {
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
	if ((tb == 1) && (t == 1) && !IsSet(x,CONTRAINTE))
	{ 
	  ListePush(LISTE1, x); 
	  n++;
	  D[x] = typedir3d(F, x, rs, ps, N);
	}
      }
    }

    while (!ListeVide(LISTE1) && (nbiter < nsteps))
    {
      nbiter++;

#ifdef VERBOSE
      printf("%s: nbiter %d ; nb simple points %d\n", F_NAME, nbiter, n);
#endif
      for (dir = 0; dir <= NBDIR; dir++)
      {
#ifdef VERBOSE2
      printf("%s: subiter %d\n", F_NAME, dir);
#endif
	for (i = 0; i < n; i++)
	{
	  x = ListeElt(LISTE1, i);
	  if (F[x] && (D[x] == dir) && !IsSet(x,CONTRAINTE))
	  {
	    mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	    if (t > 1) Set(x,CONTRAINTE);
	  }
	} // for (i = 0; i < n; i++)
	for (i = 0; i < n; i++)
	{
	  x = ListeElt(LISTE1, i);
	  if (F[x] && (D[x] == dir) && !IsSet(x,CONTRAINTE))
	    (void)testabaisse26bin(F, x, rs, ps, N);
	} // for (i = 0; i < n; i++)

#ifdef DEBUG_lskeldir3d
sprintf(DBGBUF, "_dir%d", dir);
writeimage(image, DBGBUF);
#endif
      } // for (dir = 1; dir <= NBDIR; dir++)

      ListeFlush(LISTE1);
      n = 0;
      for (x = 0; x < N; x++)
      {
	if (F[x] && (nonbord3d(x,rs,ps,N)))
	{
	  mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	  if (t > 1) Set(x,CONTRAINTE);
	  if ((tb == 1) && (t == 1) && !IsSet(x,CONTRAINTE))
	  { 
	    ListePush(LISTE1, x); 
	    n++;
	    D[x] = typedir3d(F, x, rs, ps, N);
	  } // if ((tb == 1) && (t == 1))
	} // if (F[x] && (nonbord3d(x,rs,ps,N)))
      } // for (x = 0; x < N; x++)
    } // while (!ListeVide(LISTE1))
  } // if (connex == 26)

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  free(D);
  ListeTermine(LISTE1);
  IndicsTermine();
  mctopo3d_termine_topo3d();
  return(1);
} /* lskeldir3d_1() */

// ========================================================
// ========================================================
// SQUELETTES GUIDES PARALLELES - POINTS P_SIMPLES
// ========================================================
// ========================================================

/* ==================================== */
int32_t lskelPSG(struct xvimage *image, 
		 struct xvimage *imageprio, 
		 double val)
/* ==================================== */
/*
Squelette sym�trique guid� ultime bas� sur les points P-simples

Attention : l'objet ne doit pas toucher le bord de l'image

EXPERIMENTAL - Ne pas utiliser dans des applications
*/
{ 
  if (depth(image) == 1)
    return lskelPSG2(image, imageprio, val);
  else
    return lskelPSG3(image, imageprio, val);
} // lskelPSG()

/* ==================================== */
int32_t lskelPSG2(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
// EXPERIMENTAL - Ne pas utiliser dans des applications
#undef F_NAME
#define F_NAME "lskelPSG2"
#define PSIMPLE      1
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  struct xvimage *candidats;
  uint8_t *F = UCHARDATA(image);   /* objet */
  uint8_t *C;                      /* candidats */
  Rlifo * RLIFO;
  double curprio;

  ONLY_2D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  if (val == -1) val = LARGE_VAL;

  IndicsInit(N);

  candidats = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (candidats == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  C = UCHARDATA(candidats);
  razimage(candidats);

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * cs +  2 * rs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && bordext8(F, x, rs, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
#define DEBUG_lskelPSG2
#ifdef DEBUG_lskelPSG2
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelPSG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	C[x] = 1;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (P_simple8(F, C, x, rs, N))
      {
#ifdef DEBUG_lskelPSG2
	printf("P_simple: %d\n", x);
#endif
	Set(x, PSIMPLE); // marque le point pour effacement ulterieur
        for (k = 0; k < 8; k += 1) // parcourt les voisins en 8-connexite
        {                          // pour empiler les voisins non deja empiles
          y = voisin(x, k, rs, N);
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
#ifdef DEBUG_lskelPSG2
	    printf("push: %d\n", y);
#endif
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (P_simple8(F, C, x, rs, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, PSIMPLE)) F[x] = 0;
      C[x] = 0;
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  freeimage(candidats);
  return(1);
} /* lskelPSG2() */

/* ==================================== */
int32_t lskelPSG3(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
// EXPERIMENTAL - Ne pas utiliser dans des applications
#undef F_NAME
#define F_NAME "lskelPSG3"
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(imageprio);
  index_t N = ds * ps;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  struct xvimage *candidats;
  uint8_t *F = UCHARDATA(image);   /* objet */
  uint8_t *C;                      /* candidats */
  Rlifo * RLIFO;
  double curprio;

  if (val == -1) val = LARGE_VAL;

#define DEBUG_lskelPSG3
#ifdef DEBUG_lskelPSG3
  printf("entering %s: val = %g\n", F_NAME, val);
#endif

  ONLY_3D(imageprio);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  IndicsInit(N);

  candidats = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (candidats == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  C = UCHARDATA(candidats);
  razimage(candidats);
  for (x = 0; x < N; x++)

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  mctopo3d_init_topo3d();

  taillemaxrbt = 2 * rs * cs +  2 * rs * ds +  2 * ds * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
#ifdef DEBUG_lskelPSG3
  printf("%s: curprio = %g\n", F_NAME, curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = mcrbt_RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (mctopo3d_simple26(F, x, rs, ps, N))
      {
	RlifoPush(&RLIFO, x);
	C[x] = 1;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (P_simple26(F, C, x, rs, ps, N))
      {
	Set(x, PSIMPLE); // marque le point pour effacement ult�rieur
#ifdef DEBUG_lskelPSG3
	printf("P_simple: %d\n", x);
#endif
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (P_simple26(F, C, x, rs, ps, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, PSIMPLE)) F[x] = 0;
      C[x] = 0;
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  mctopo3d_termine_topo3d();
  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  freeimage(candidats);
  return(1);
} /* lskelPSG3() */

// ========================================================
// ========================================================
// SQUELETTES GUIDES PARALLELES - NOYAUX CRITIQUES
// ========================================================
// ========================================================

/* ==================================== */
int32_t lskelCKG(struct xvimage *image, 
		 struct xvimage *imageprio, 
		 double val)
/* ==================================== */
/*
Squelette sym�trique guid� ultime bas� sur les noyaux critiques (cruciaux)

Attention : l'objet ne doit pas toucher le bord de l'image

EXPERIMENTAL - Ne pas utiliser dans des applications
*/
{ 
  if (depth(image) == 1)
    return lskelCKG2(image, imageprio, val);
  else
    return lskelCKG3(image, imageprio, val);
} // lskelCKG()

/* ==================================== */
int32_t lskelCKGmap(struct xvimage *imageprio, 
		    struct xvimage *image)
/* ==================================== */
/*
Carte topologique par squelettisation sym�trique guid�e ultime bas�e sur les noyaux critiques (cruciaux)

Attention : l'objet ne doit pas toucher le bord de l'image

EXPERIMENTAL - Ne pas utiliser dans des applications
*/
{ 
  if (depth(image) == 1)
    return lskelCKG2map(imageprio, image);
  else
    return lskelCKG3map(imageprio, image);
} // lskelCKGmap()

/* ==================================== */
static void extract_vois(uint8_t *img, int32_t p, int32_t rs, int32_t N, uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 8 voisins de p, dans l'ordre suivant:

		3	2	1
		4	p	0
		5	6	7
  le point p ne doit pas �tre un point de bord de l'image (test effectu�)
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p<rs) || (p%rs==0) || (p>=N-rs)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  vois[0] = *(ptr+1);
  vois[1] = *(ptr+1-rs);
  vois[2] = *(ptr-rs);
  vois[3] = *(ptr-rs-1);
  vois[4] = *(ptr-1);
  vois[5] = *(ptr-1+rs);
  vois[6] = *(ptr+rs);
  vois[7] = *(ptr+rs+1);
} /* extract_vois() */

/* ==================================== */
static void extract_vois3d(
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N,                       /* taille image */
  uint8_t *vois)    
/* 
  retourne dans "vois" les valeurs des 27 voisins de p, dans l'ordre suivant: 

               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7

               21      20      19
               22      17      18
               23      24      25

  le point p ne doit pas �tre un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "extract_vois3d"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  vois[ 0] = *(ptr+1);
  vois[ 1] = *(ptr+1-rs);
  vois[ 2] = *(ptr-rs);
  vois[ 3] = *(ptr-rs-1);
  vois[ 4] = *(ptr-1);
  vois[ 5] = *(ptr-1+rs);
  vois[ 6] = *(ptr+rs);
  vois[ 7] = *(ptr+rs+1);

  vois[ 8] = *(ptr-ps);
  vois[ 9] = *(ptr-ps+1);
  vois[10] = *(ptr-ps+1-rs);
  vois[11] = *(ptr-ps-rs);
  vois[12] = *(ptr-ps-rs-1);
  vois[13] = *(ptr-ps-1);
  vois[14] = *(ptr-ps-1+rs);
  vois[15] = *(ptr-ps+rs);
  vois[16] = *(ptr-ps+rs+1);

  vois[17] = *(ptr+ps);
  vois[18] = *(ptr+ps+1);
  vois[19] = *(ptr+ps+1-rs);
  vois[20] = *(ptr+ps-rs);
  vois[21] = *(ptr+ps-rs-1);
  vois[22] = *(ptr+ps-1);
  vois[23] = *(ptr+ps-1+rs);
  vois[24] = *(ptr+ps+rs);
  vois[25] = *(ptr+ps+rs+1);

  vois[26] = *(ptr);
} /* extract_vois3d() */

/* ==================================== */
static void insert_vois3d(
  uint8_t *vois,			
  uint8_t *img,          /* pointeur base image */
  int32_t p,                       /* index du point */
  int32_t rs,                      /* taille rangee */
  int32_t ps,                      /* taille plan */
  int32_t N)                       /* taille image */    
/* 
  recopie vois dans le voisinage de p
  le point p ne doit pas �tre un point de bord de l'image
*/
/* ==================================== */
{
#undef F_NAME
#define F_NAME "insert_vois3d"
  register uint8_t * ptr = img+p;
  if ((p%rs==rs-1) || (p%ps<rs) || (p%rs==0) || (p%ps>=ps-rs) || 
      (p < ps) || (p >= N-ps)) /* point de bord */
  {
    printf("%s: ERREUR: point de bord\n", F_NAME);
    exit(0);
  }
  *(ptr+1) = vois[ 0];
  *(ptr+1-rs) = vois[ 1];
  *(ptr-rs) = vois[ 2];
  *(ptr-rs-1) = vois[ 3];
  *(ptr-1) = vois[ 4];
  *(ptr-1+rs) = vois[ 5];
  *(ptr+rs) = vois[ 6];
  *(ptr+rs+1) = vois[ 7];

  *(ptr-ps) = vois[ 8];
  *(ptr-ps+1) = vois[ 9];
  *(ptr-ps+1-rs) = vois[10];
  *(ptr-ps-rs) = vois[11];
  *(ptr-ps-rs-1) = vois[12];
  *(ptr-ps-1) = vois[13];
  *(ptr-ps-1+rs) = vois[14];
  *(ptr-ps+rs) = vois[15];
  *(ptr-ps+rs+1) = vois[16];

  *(ptr+ps) = vois[17];
  *(ptr+ps+1) = vois[18];
  *(ptr+ps+1-rs) = vois[19];
  *(ptr+ps-rs) = vois[20];
  *(ptr+ps-rs-1) = vois[21];
  *(ptr+ps-1) = vois[22];
  *(ptr+ps-1+rs) = vois[23];
  *(ptr+ps+rs) = vois[24];
  *(ptr+ps+rs+1) = vois[25];

  *(ptr) = vois[26];
} /* insert_vois3d() */

/* ==================================== */
static void rotate90_vois(uint8_t *vois)
/*
   effectue une rotation du voisinage "vois" de 90 degres dans le sens
   trigonom�trique
*/
/* ==================================== */
{
  uint8_t tmp;
  tmp = vois[0]; vois[0] = vois[6]; vois[6] = vois[4]; vois[4] = vois[2]; vois[2] = tmp;
  tmp = vois[7]; vois[7] = vois[5]; vois[5] = vois[3]; vois[3] = vois[1]; vois[1] = tmp;
} /* rotate90_vois() */

// Etiquetage des points objet
// Tout point objet a une �tiquette >= OBJ
// Tout point candidat (donc simple) a une �tiquette >= CAN

/* ==================================== */
static void CrucialPass1( /* pour un objet en 8-connexite */
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  index_t N)       /* taille image */
/*
  Rep�re et marque CR1 les cliques 1-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir pr�alablement �t� �tiquet�s CAN.
  Il est suppos� que X[p] == CAN (pas de test).
*/
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];

  extract_vois(X, p, rs, N, v);

/*
x a a
x C C  avec au moins un des a et au moins un des b non nuls
x b b  ou : tous les a et tous les b nuls
*/
  for (i = 0; i < 4; i++)
  {
    if (v[0] < CAN) goto fail1;
    if ((v[1] == 0) && (v[2] == 0) && (v[6] == 0) && (v[7] == 0)) 
    { X[p] = CR1; return; }
    if ((v[1] == 0) && (v[2] == 0)) goto fail1;
    if ((v[6] == 0) && (v[7] == 0)) goto fail1;
    X[p] = CR1; 
    return;
  fail1:
    if (i < 3) rotate90_vois(v);
  }
} // CrucialPass1()

/* ==================================== */
static void CrucialPass0( /* pour un objet en 8-connexite */
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  index_t N)       /* taille image */
/*
  Rep�re et marque CR0 les cliques 0-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir pr�alablement �t� �tiquet�s CAN.
  Il est suppos� que X[p] == CAN (pas de test).
  Les cliques 1-cruciales doivent avoir pr�alablement �t� �tiquet�es CR1 (voir CrucialPass1).
*/
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];

  extract_vois(X, p, rs, N, v);

/*
x b a  on doit avoir le point 'a' ou les 2 points 'b' dans X (ou les deux), 
x C b  et tous les points 'a,b' de X marqu�s CAN,
x x x  pour que le point central soit �tiquet� CR0
*/
  for (i = 0; i < 4; i++)
  {
    if ((v[1] == 0) && ((v[0] == 0) || (v[2] == 0))) goto fail1;
    if (v[0] && (v[0] < CAN)) goto fail1;
    if (v[1] && (v[1] < CAN)) goto fail1;
    if (v[2] && (v[2] < CAN)) goto fail1;
    X[p] = CR0;
    return;
  fail1:
    if (i < 3) rotate90_vois(v);
  }
} // CrucialPass0()

/* ==================================== */
static void isometrieXZ_vois(uint8_t *vois) 
// effectue une isom�trie du voisinage "vois" par �change des axes X et Z (+ sym�tries)
// cette isom�trie est de plus une involution
/* ==================================== */
{
  uint8_t v[26];
  int32_t i;
  v[ 0] = vois[17];  v[ 1] = vois[20];  v[ 2] = vois[ 2];  v[ 3] = vois[11];
  v[ 4] = vois[ 8];  v[ 5] = vois[15];  v[ 6] = vois[ 6];  v[ 7] = vois[24];
  v[ 8] = vois[ 4];  v[ 9] = vois[22];  v[10] = vois[21];  v[11] = vois[ 3];
  v[12] = vois[12];  v[13] = vois[13];  v[14] = vois[14];  v[15] = vois[ 5];
  v[16] = vois[23];  v[17] = vois[ 0];  v[18] = vois[18];  v[19] = vois[19];
  v[20] = vois[ 1];  v[21] = vois[10];  v[22] = vois[ 9];  v[23] = vois[16];
  v[24] = vois[ 7];  v[25] = vois[25];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieXZ_vois() */

/* ==================================== */
static void isometrieYZ_vois(uint8_t *vois)
// effectue une isom�trie du voisinage "vois" par �change des axes Y et Z (+ sym�tries)  
// cette isom�trie est de plus une involution
/* ==================================== */
{
  uint8_t v[26];
  int32_t i;
  v[ 0] = vois[ 0];  v[ 1] = vois[18];  v[ 2] = vois[17];  v[ 3] = vois[22];
  v[ 4] = vois[ 4];  v[ 5] = vois[13];  v[ 6] = vois[ 8];  v[ 7] = vois[ 9];
  v[ 8] = vois[ 6];  v[ 9] = vois[ 7];  v[10] = vois[25];  v[11] = vois[24];
  v[12] = vois[23];  v[13] = vois[ 5];  v[14] = vois[14];  v[15] = vois[15];
  v[16] = vois[16];  v[17] = vois[ 2];  v[18] = vois[ 1];  v[19] = vois[19];
  v[20] = vois[20];  v[21] = vois[21];  v[22] = vois[ 3];  v[23] = vois[12];
  v[24] = vois[11];  v[25] = vois[10];
  for (i = 0; i < 26; i++) vois[i] = v[i];
} /* isometrieYZ_vois() */

#ifdef DEBUG
static int trace = 1;
#endif

/* ==================================== */
static int32_t match_vois2(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Teste si les conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre marqu�s CAN
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, les points 8, 26 sont marqu�s CR2
*/
{
  uint8_t t;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois2\n");
    //    print_vois(v);
  }
#endif
  if ((v[8] < CAN) || (v[26] < CAN)) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
  v[8] = v[26] = CR2;
#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return 1;
} // match_vois2()

/* ==================================== */
static int32_t match_vois1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisations possibles :
// 12 11   3  2   21 20 
// 13  8   4 26   22 17
// et :
// 11 10    2 1   20 19
//  8  9   26 0   17 18
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  int32_t ret = 0;
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois1\n");
    //    print_vois(v);
  }
#endif
  if (!((v[2] && v[4]) || (v[3] && v[26]))) goto next1;
  if ((v[2]  && (v[2] != CAN)) ||
      (v[3]  && (v[3] != CAN)) ||
      (v[4]  && (v[4] != CAN)) ||
      (v[26] && (v[26] != CAN))) goto next1;
  if ((v[12] || v[11] || v[13] || v[8] || v[21] || v[20] || v[22] || v[17]) &&
      ((!v[12] && !v[11] && !v[13] && !v[8]) || 
       (!v[21] && !v[20] && !v[22] && !v[17]))) goto next1;
  if (v[2])  v[2] = CR1;
  if (v[3])  v[3] = CR1;
  if (v[4])  v[4] = CR1;
  if (v[26]) v[26] = CR1;
  ret = 1;
 next1:
  if (!((v[2] && v[0]) || (v[1] && v[26]))) goto next2;
  if ((v[2]  && ((v[2] < CAN) || (v[2] == CR2))) ||
      (v[1]  && ((v[1] < CAN) || (v[1] == CR2))) ||
      (v[0]  && ((v[0] < CAN) || (v[0] == CR2))) ||
      (v[26]  && ((v[26] < CAN) || (v[26] == CR2)))) goto next2;
  if ((v[10] || v[11] || v[9] || v[8] || v[19] || v[20] || v[18] || v[17]) &&
      ((!v[10] && !v[11] && !v[9] && !v[8]) || 
       (!v[19] && !v[20] && !v[18] && !v[17]))) goto next2;
  if (v[2])  v[2] = CR1;
  if (v[1])  v[1] = CR1;
  if (v[0])  v[0] = CR1;
  if (v[26]) v[26] = CR1;
  ret = 1;
 next2:
#ifdef DEBUG
  if (trace && ret)
    printf("match !\n");
#endif
  return ret;
} // match_vois1()

/* ==================================== */
static int32_t match_vois0(uint8_t *v)
/* ==================================== */
/*
               12      11
               13       8

		3	2
		4      26

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles {12,26}, {11,4}, {13,2}, {8,3} est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqu�s 2M_CRUCIAL et non marqu�s 1M_CRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s 0M_CRUCIAL
*/
{
#ifdef DEBUG
  if (trace)
  {  
    printf("match_vois0\n");
    //    print_vois(v);
  }
#endif
  if (!((v[12]&&v[26]) || (v[11]&&v[4]) || (v[13]&&v[2]) || (v[8]&&v[3]) )) return 0;

  if (v[12] && (v[12] != CAN)) return 0;
  if (v[26] && (v[26] != CAN)) return 0;
  if (v[11] && (v[11] != CAN)) return 0;
  if (v[ 4] && (v[ 4] != CAN)) return 0;
  if (v[13] && (v[13] != CAN)) return 0;
  if (v[ 2] && (v[ 2] != CAN)) return 0;
  if (v[ 8] && (v[ 8] != CAN)) return 0;
  if (v[ 3] && (v[ 3] != CAN)) return 0;

  if (v[12]) v[12] = CR0;
  if (v[26]) v[26] = CR0;
  if (v[11]) v[11] = CR0;
  if (v[ 4]) v[ 4] = CR0;
  if (v[13]) v[13] = CR0;
  if (v[ 2]) v[ 2] = CR0;
  if (v[ 8]) v[ 8] = CR0;
  if (v[ 3]) v[ 3] = CR0;
#ifdef DEBUG
  if (trace)
    printf("match !\n");
#endif
  return 1;
} // match_vois0()

/* ==================================== */
static void CrucialPass3d2(
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  int32_t ps,      /* taille plan */
  index_t N)       /* taille image */
/*
  Rep�re et marque CR2 les cliques 2-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir pr�alablement �t� �tiquet�s CAN.
  Il est suppos� que X[p] == CAN (pas de test).
*/
/* ==================================== */
{
  uint8_t v[27];
  extract_vois3d(X, p, rs, ps, N, v);
  (void)match_vois2(v);
  isometrieXZ_vois(v);
  (void)match_vois2(v);
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  (void)match_vois2(v);
  isometrieYZ_vois(v);
  insert_vois3d(v, X, p, rs, ps, N);
} // CrucialPass3d2()

/* ==================================== */
static void CrucialPass3d1(
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  int32_t ps,      /* taille plan */
  index_t N)       /* taille image */
/*
  Rep�re et marque CR1 les cliques 1-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir pr�alablement �t� �tiquet�s CAN.
  Il est suppos� que X[p] == CAN (pas de test).
*/
/* ==================================== */
{
  uint8_t v[27];
  extract_vois3d(X, p, rs, ps, N, v);
  (void)match_vois1(v);
  isometrieXZ_vois(v);
  (void)match_vois1(v);
  isometrieXZ_vois(v);
  isometrieYZ_vois(v);
  (void)match_vois1(v);
  isometrieYZ_vois(v);
  insert_vois3d(v, X, p, rs, ps, N);
} // CrucialPass3d1()

/* ==================================== */
static void CrucialPass3d0(
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  int32_t ps,      /* taille plan */
  index_t N)       /* taille image */
/*
  Rep�re et marque CR0 les cliques 0-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir pr�alablement �t� �tiquet�s CAN.
  Il est suppos� que X[p] == CAN (pas de test).
*/
/* ==================================== */
{
  uint8_t v[27];
  extract_vois3d(X, p, rs, ps, N, v);
  (void)match_vois0(v);
  insert_vois3d(v, X, p, rs, ps, N);
} // CrucialPass3d0()

#define NONCRUCIAL 1

/* ==================================== */
int32_t lskelCKG2(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
// parallel 2D binary guided thinning
// EXPERIMENTAL - Ne pas utiliser dans des applications
#undef F_NAME
#define F_NAME "lskelCKG2"
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint8_t *F = UCHARDATA(image);   /* objet */
  Rlifo * RLIFO;
  double curprio;

  ONLY_2D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  IndicsInit(N);

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * cs +  2 * rs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x]) F[x] = OBJ;
    if (F[x] && bordext8(F, x, rs, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
//#define DEBUG_lskelCKG2
#ifdef DEBUG_lskelCKG2
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass1(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass0(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (F[x] == CAN)
      {
#ifdef DEBUG_lskelCKG2
	printf("Non Crucial: %d\n", x);
#endif
	Set(x, NONCRUCIAL); // marque le point pour effacement ulterieur
        for (k = 0; k < 8; k += 1) // parcourt les voisins en 8-connexite
        {                          // pour empiler les voisins non deja empiles
          y = voisin(x, k, rs, N);
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
#ifdef DEBUG_lskelCKG2
	    printf("push: %d\n", y);
#endif
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (NonCrucial(F, C, x, rs, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, NONCRUCIAL)) F[x] = 0;
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  return(1);
} /* lskelCKG2() */

/* ==================================== */
int32_t lskelCKG2map(struct xvimage *imageprio,
		     struct xvimage *image)
/* ==================================== */
// EXPERIMENTAL - Ne pas utiliser dans des applications
// the result is in imageprio : a "topological map"
#undef F_NAME
#define F_NAME "lskelCKG2map"
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t N = rs * cs;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint8_t *F = UCHARDATA(image);   /* objet */
  Rlifo * RLIFO;
  double curprio, incrprio;

  ONLY_2D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  IndicsInit(N);

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * cs +  2 * rs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x]) F[x] = OBJ;
    if (F[x] && bordext8(F, x, rs, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  incrprio = DOUBLE_MIN; // this value will only increase during execution
  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
    if (curprio > incrprio) incrprio = curprio;
//#define DEBUG_lskelCKG2
#ifdef DEBUG_lskelCKG2
    printf("entering loop, curprio = %g, incrprio = %g\n", curprio, incrprio);
#endif
    do
    {
      x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass1(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass0(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (F[x] == CAN)
      {
#ifdef DEBUG_lskelCKG2
	printf("Non Crucial: %d\n", x);
#endif
	Set(x, NONCRUCIAL); // marque le point pour effacement ulterieur
        for (k = 0; k < 8; k += 1) // parcourt les voisins en 8-connexite
        {                          // pour empiler les voisins non deja empiles
          y = voisin(x, k, rs, N);
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
#ifdef DEBUG_lskelCKG2
	    printf("push: %d\n", y);
#endif
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (NonCrucial(F, C, x, rs, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, NONCRUCIAL)) 
      {
	F[x] = 0;
	switch(datatype(imageprio))
	{
	case VFF_TYP_4_BYTE: P[x] = (int32_t)incrprio; break;
	case VFF_TYP_1_BYTE: PB[x] = (uint8_t)incrprio; break;
	case VFF_TYP_FLOAT : PF[x] = (float)incrprio; break;
	case VFF_TYP_DOUBLE: PD[x] = incrprio; break;
	}
      }
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  return(1);
} /* lskelCKG2map() */

/* ==================================== */
int32_t lskelCKG3map(struct xvimage *imageprio,
		     struct xvimage *image)
/* ==================================== */
// EXPERIMENTAL - Ne pas utiliser dans des applications
// the result is in imageprio : a "topological map"
#undef F_NAME
#define F_NAME "lskelCKG3map"
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(imageprio);   /* nb plans */
  index_t N = ps * ds;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint8_t *F = UCHARDATA(image);   /* objet */
  Rlifo * RLIFO;
  double curprio, incrprio;

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  mctopo3d_init_topo3d();  
  IndicsInit(N);

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * cs +  2 * rs +  2 * ps;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x]) F[x] = OBJ;
    if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  incrprio = DOUBLE_MIN; // this value will only increase during execution
  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
    if (curprio > incrprio) incrprio = curprio;
//#define DEBUG_lskelCKG2
#ifdef DEBUG_lskelCKG2
    printf("entering loop, curprio = %g, incrprio = %g\n", curprio, incrprio);
#endif
    do
    {
      x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && mctopo3d_simple26(F, x, rs, ps, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d2(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d1(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d0(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (F[x] == CAN)
      {
#ifdef DEBUG_lskelCKG2
	printf("Non Crucial: %d\n", x);
#endif
	Set(x, NONCRUCIAL); // marque le point pour effacement ulterieur
        for (k = 0; k < 26; k += 1) // parcourt les voisins en 26-connexite
        {                          // pour empiler les voisins non deja empiles
          y = voisin26(x, k, rs, ps, N);
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
#ifdef DEBUG_lskelCKG2
	    printf("push: %d\n", y);
#endif
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (NonCrucial(F, C, x, rs, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, NONCRUCIAL)) 
      {
	F[x] = 0;
	switch(datatype(imageprio))
	{
	case VFF_TYP_4_BYTE: P[x] = (int32_t)incrprio; break;
	case VFF_TYP_1_BYTE: PB[x] = (uint8_t)incrprio; break;
	case VFF_TYP_FLOAT : PF[x] = (float)incrprio; break;
	case VFF_TYP_DOUBLE: PD[x] = incrprio; break;
	}
      }
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  return(1);
} /* lskelCKG3map() */

/* ==================================== */
int32_t lskelCKG3(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
// parallel 3D binary guided thinning
// EXPERIMENTAL - Ne pas utiliser dans des applications
#undef F_NAME
#define F_NAME "lskelCKG3"
{ 
  int32_t i, k;
  index_t x, y;                    /* index de pixel */
  index_t rs = rowsize(imageprio); /* taille ligne */
  index_t cs = colsize(imageprio); /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(imageprio);   /* nb plans */
  index_t N = ps * ds;             /* taille image */
  int32_t *P = NULL;   /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  index_t taillemaxrbt;
  uint8_t *F = UCHARDATA(image);   /* objet */
  Rlifo * RLIFO;
  double curprio;

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  
  ACCEPTED_TYPES4(imageprio, VFF_TYP_1_BYTE, VFF_TYP_4_BYTE, VFF_TYP_FLOAT, VFF_TYP_DOUBLE);
  COMPARE_SIZE(image, imageprio);

  mctopo3d_init_topo3d();  
  IndicsInit(N);

  if (datatype(imageprio) == VFF_TYP_4_BYTE) 
    P = SLONGDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_1_BYTE) 
    PB = UCHARDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_FLOAT) 
    PF = FLOATDATA(imageprio); 
  else if (datatype(imageprio) == VFF_TYP_DOUBLE) 
    PD = DOUBLEDATA(imageprio); 
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be uint8_t, int32_t, float or double\n", F_NAME);
    return(0);
  }

  taillemaxrbt = 2 * cs +  2 * rs +  2 * ps;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = mcrbt_CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: mcrbt_CreeRbtVide failed\n", F_NAME);
    return(0);
  }
  RLIFO = CreeRlifoVide(taillemaxrbt);
  if (RLIFO == NULL)
  {
    fprintf(stderr, "%s : CreeRlifoVide failed\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x]) F[x] = OBJ;
    if (F[x] && mctopo3d_bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  while (!mcrbt_RbtVide(RBT))
  {
    curprio = mcrbt_RbtMinLevel(RBT);
//#define DEBUG_lskelCKG3
#ifdef DEBUG_lskelCKG3
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = mcrbt_RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG3
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && mctopo3d_simple26(F, x, rs, ps, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (mcrbt_RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d2(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d1(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass3d0(F, x, rs, ps, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];

      if (F[x] == CAN)
      {
#ifdef DEBUG_lskelCKG3
	printf("Non Crucial: %d\n", x);
#endif
	Set(x, NONCRUCIAL); // marque le point pour effacement ulterieur
        for (k = 0; k < 26; k += 1) // parcourt les voisins en 26-connexite
        {                          // pour empiler les voisins non deja empiles
          y = voisin26(x, k, rs, ps, N);
          if ((y != -1) && (F[y]) && (! IsSet(y, EN_RBT)))
          {
	    switch(datatype(imageprio))
	    {
	      case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: mcrbt_RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT, PD[y], y); break;
	    }
#ifdef DEBUG_lskelCKG3
	    printf("push: %d\n", y);
#endif
            Set(y, EN_RBT);
          } // if y
        } // for k
      } // if (NonCrucial(F, C, x, rs, N)
    } // for (i = 0; i < RLIFO->Sp; i++)

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (IsSet(x, NONCRUCIAL)) F[x] = 0;
    }

    RlifoFlush(RLIFO);

  } // while (!mcrbt_RbtVide(RBT))

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  mcrbt_RbtTermine(RBT);
  RlifoTermine(RLIFO);
  return(1);
} /* lskelCKG3() */

/* ==================================== */
static int32_t match_seq_vois2_a(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9
               14      15      16

		3	2	1			
		4      26	0
		5	6	7
Teste si les conditions suivantes sont r�unies:
1: v[8] et v[26] doivent �tre marqu�s CAN
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit �tre non 2D-simple
Si le test r�ussit, les points 8, 26 sont marqu�s CR2
C = {x, x-ps}
*/
{
  uint8_t t;
  if ((v[8] < CAN) || (v[26] < CAN)) return 0;
  if (v[0] || v[9]) t = 1; else t = 0;
  if (v[1] || v[10]) t |= 2;
  if (v[2] || v[11]) t |= 4;
  if (v[3] || v[12]) t |= 8;
  if (v[4] || v[13]) t |= 16;
  if (v[5] || v[14]) t |= 32;
  if (v[6] || v[15]) t |= 64;
  if (v[7] || v[16]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
#ifdef DCRUCIAL_MARK
  v[8] = v[26] = CR2;
#endif
#ifdef DEBUGmatch
printf("match 2a\n");
#endif
  return 1;
} // match_seq_vois2_a()

/* ==================================== */
static int32_t match_seq_vois2_b(uint8_t *v)
/* ==================================== */
/*
               12      11      10       
               13       8       9

		3	2	1			
		4      26	0

               21      20      19
               22      17      18
Teste si les conditions suivantes sont r�unies:
1: v[2] et v[26] doivent �tre marqu�s CAN
2: ... doit �tre non 2D-simple
Si le test r�ussit, les points 2, 26 sont marqu�s CR2
C = {x, x-rs}
*/
{
  uint8_t t;
  if ((v[2] < CAN) || (v[26] < CAN)) return 0;
  if (v[ 0] || v[ 1]) t = 1; else t = 0;
  if (v[ 9] || v[10]) t |= 2;
  if (v[ 8] || v[11]) t |= 4;
  if (v[13] || v[12]) t |= 8;
  if (v[ 4] || v[ 3]) t |= 16;
  if (v[22] || v[21]) t |= 32;
  if (v[17] || v[20]) t |= 64;
  if (v[18] || v[19]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
#ifdef DCRUCIAL_MARK
  v[2] = v[26] = CR2;
#endif
#ifdef DEBUGmatch
printf("match 2b\n");
#endif
  return 1;
} // match_seq_vois2_b()

/* ==================================== */
static int32_t match_seq_vois2_c(uint8_t *v)
/* ==================================== */
/*
               12      11
               13       8
               14      15

		3	2
		4      26
		5	6

               21      20
               22      17
               23      24
	       
Teste si les conditions suivantes sont r�unies:
1: v[4] et v[26] doivent �tre marqu�s CAN
2: le ... doit �tre non 2D-simple
Si le test r�ussit, les points 4, 26 sont marqu�s CR2
C = {x, x-1}
*/
{
  uint8_t t;
  if ((v[4] < CAN) || (v[26] < CAN)) return 0;
  if (v[ 3] || v[ 2]) t = 1; else t = 0;
  if (v[12] || v[11]) t |= 2;
  if (v[13] || v[ 8]) t |= 4;
  if (v[14] || v[15]) t |= 8;
  if (v[ 5] || v[ 6]) t |= 16;
  if (v[23] || v[24]) t |= 32;
  if (v[22] || v[17]) t |= 64;
  if (v[21] || v[20]) t |= 128;
  if ((t4b(t) == 1) && (t8(t) == 1)) return 0; // simple 2D
#ifdef DCRUCIAL_MARK
  v[4] = v[26] = CR2;
#endif
#ifdef DEBUGmatch
printf("match 2c\n");
#endif
  return 1;
} // match_seq_vois2_c()

/* ==================================== */
static int32_t Clique2Critique(
  uint8_t *F,      /* pointeur base image */
  index_t p1,      /* index du point */
  index_t p2,      /* index du point */
  index_t rs,      /* taille rangee */
  int32_t ps,      /* taille plan */
  index_t N)       /* taille image */
/* ==================================== */
{
  int32_t i1, j1, k1, i2, j2, k2;
  uint8_t v[27];
  i1 = p1%rs; j1 = (p1%ps)/rs; k1 = p1/ps;
  i2 = p2%rs; j2 = (p2%ps)/rs; k2 = p2/ps;
#ifdef DEBUGmatch
  printf("Clique2Critique %d %d %d - %d %d %d\n", i1, j1, k1, i2, j2, k2);
#endif
  if (k1 < k2) { extract_vois3d(F, p2, rs, ps, N, v); return match_seq_vois2_a(v); }
  else if (k1 > k2) { extract_vois3d(F, p1, rs, ps, N, v); return match_seq_vois2_a(v); }
  else if (j1 < j2) { extract_vois3d(F, p2, rs, ps, N, v); return match_seq_vois2_b(v); }
  else if (j1 > j2) { extract_vois3d(F, p1, rs, ps, N, v); return match_seq_vois2_b(v); }
  else if (i1 < i2) { extract_vois3d(F, p2, rs, ps, N, v); return match_seq_vois2_c(v); }
  else if (i1 > i2) { extract_vois3d(F, p1, rs, ps, N, v); return match_seq_vois2_c(v); }
  assert(1); return 0;
} // Clique2Critique()

/* ==================================== */
static int32_t match_seq_vois1_a_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
// 12 11
// 13  8
//
// 3  2 
// 4 26 
//
// 21 20
// 22 17

// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[2] && v[4]) || (v[3] && v[26]))) return 0;
  if ((v[2]  && ((v[2] < CAN) || (v[2] == CR2))) ||
      (v[3]  && ((v[3] < CAN) || (v[3] == CR2))) ||
      (v[4]  && ((v[4] < CAN) || (v[4] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[12] || v[11] || v[13] || v[8] || v[21] || v[20] || v[22] || v[17]) &&
      ((!v[12] && !v[11] && !v[13] && !v[8]) || 
       (!v[21] && !v[20] && !v[22] && !v[17]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[2])  v[2] = CR1;
  if (v[3])  v[3] = CR1;
  if (v[4])  v[4] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1a - 1\n");
#endif
  return 1; // C = {x, x-rs, x-1, x-rs-1}
} // match_seq_vois1_a_1()

/* ==================================== */
static int32_t match_seq_vois1_a_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
// 11 10
//  8  9
//
//  2  1
// 26  0
//
// 20 19
// 17 18

// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[2] && v[0]) || (v[1] && v[26]))) return 0;
  if ((v[2]  && ((v[2] < CAN) || (v[2] == CR2))) ||
      (v[1]  && ((v[1] < CAN) || (v[1] == CR2))) ||
      (v[0]  && ((v[0] < CAN) || (v[0] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[10] || v[11] || v[9] || v[8] || v[19] || v[20] || v[18] || v[17]) &&
      ((!v[10] && !v[11] && !v[9] && !v[8]) || 
       (!v[19] && !v[20] && !v[18] && !v[17]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[2])  v[2] = CR1;
  if (v[1])  v[1] = CR1;
  if (v[0])  v[0] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1a - 2\n");
#endif
  return 1; // C = {x, x-rs, x+1, x-rs+1}
} // match_seq_vois1_a_2()

/* ==================================== */
static int32_t match_seq_vois1_b_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  12 11 10
//  13  8  9
//
//   3  2  1
//   4 26  0
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[6] && v[8]) || (v[15] && v[26]))) return 0;
  if ((v[6]  && ((v[6] < CAN) || (v[6] == CR2))) ||
      (v[8]  && ((v[8] < CAN) || (v[8] == CR2))) ||
      (v[15]  && ((v[15] < CAN) || (v[15] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[14] || v[13] || v[5] || v[4] || v[7] || v[0] || v[9] || v[16]) &&
      ((!v[14] && !v[13] && !v[5] && !v[4]) || 
       (!v[7] && !v[0] && !v[9] && !v[16]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[6])  v[6] = CR1;
  if (v[8])  v[8] = CR1;
  if (v[15]) v[15] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1b - 1\n");
#endif
  return 1; // C = {x, x+rs, x-ps, x-ps+rs}
} // match_seq_vois1_b_1()

/* ==================================== */
static int32_t match_seq_vois1_b_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  13  8  9
//  14 15 16
//
//   4 26  0			
//   5  6  7
//
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[2] && v[8]) || (v[11] && v[26]))) return 0;
  if ((v[2]  && ((v[2] < CAN) || (v[2] == CR2))) ||
      (v[8]  && ((v[8] < CAN) || (v[8] == CR2))) ||
      (v[11]  && ((v[11] < CAN) || (v[11] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[12] || v[13] || v[3] || v[4] || v[1] || v[0] || v[9] || v[10]) &&
      ((!v[12] && !v[13] && !v[3] && !v[4]) || 
       (!v[1] && !v[0] && !v[9] && !v[10]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[2])  v[2] = CR1;
  if (v[8])  v[8] = CR1;
  if (v[11]) v[11] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1b - 2\n");
#endif
  return 1; // C = {x, x-rs, x-ps, x-ps-rs}
} // match_seq_vois1_b_2()

/* ==================================== */
static int32_t match_seq_vois1_c_1(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  12 11
//  13  8
//  14 15
//
//   3  2
//   4 26
//   5  6
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[8] && v[0]) || (v[9] && v[26]))) return 0;
  if ((v[8]  && ((v[8] < CAN) || (v[8] == CR2))) ||
      (v[9]  && ((v[9] < CAN) || (v[9] == CR2))) ||
      (v[0]  && ((v[0] < CAN) || (v[0] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[11] || v[10] || v[2] || v[1] || v[16] || v[15] || v[7] || v[6]) &&
      ((!v[11] && !v[10] && !v[2] && !v[1]) || 
       (!v[16] && !v[15] && !v[7] && !v[6]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[8])  v[8] = CR1;
  if (v[9])  v[9] = CR1;
  if (v[0])  v[0] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1c - 1\n");
#endif
  return 1; // C = {x, x-ps, x+1, x-ps+1}
} // match_seq_vois1_c_1()

/* ==================================== */
static int32_t match_seq_vois1_c_2(uint8_t *v)
/* ==================================== */
// A A  P1 P2  B B
// A A  P3 P4  B B
// avec pour localisation :
//  11 10
//   8  9
//  15 16
//
//   2  1
//  26  0
//   6  7
// Teste si les trois conditions suivantes sont r�unies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent �tre simples et non marqu�s CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test r�ussit, les points Pi non nuls sont marqu�s CR1
{
  if (!((v[8] && v[4]) || (v[13] && v[26]))) return 0;
  if ((v[8]  && ((v[8] < CAN) || (v[8] == CR2))) ||
      (v[13] && ((v[13] < CAN) || (v[13] == CR2))) ||
      (v[4]  && ((v[4] < CAN) || (v[4] == CR2))) ||
      (v[26] && ((v[26] < CAN) || (v[26] == CR2)))) return 0;
  if ((v[12] || v[11] || v[3] || v[2] || v[14] || v[15] || v[5] || v[6]) &&
      ((!v[12] && !v[11] && !v[3] && !v[2]) || 
       (!v[14] && !v[15] && !v[5] && !v[6]))) return 0;
#ifdef DCRUCIAL_MARK
  if (v[8])  v[8] = CR1;
  if (v[13]) v[13] = CR1;
  if (v[4])  v[4] = CR1;
  if (v[26]) v[26] = CR1;
#endif
#ifdef DEBUGmatch
printf("match 1c - 2\n");
#endif
  return 1; // C = {x, x-ps, x-1, x-ps-1}
} // match_seq_vois1_c_2()

/* ==================================== */
static int32_t match_seq_vois0_1(uint8_t *v)
/* ==================================== */
/*
  12 11
  13  8

   3  2
   4 26

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles {12,26}, {11,4}, {13,2}, {8,3} est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqu�s 2M_CRUCIAL et non marqu�s 1M_CRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s 0M_CRUCIAL
*/
{
  //printf("match 01:\n");
  //print_vois(v);
  if (!((v[12]&&v[26]) || (v[11]&&v[4]) || (v[13]&&v[2]) || (v[8]&&v[3]) )) return 0;
  if (v[12] && (v[12] != CAN)) return 0;
  if (v[26] && (v[26] != CAN)) return 0;
  if (v[11] && (v[11] != CAN)) return 0;
  if (v[ 4] && (v[ 4] != CAN)) return 0;
  if (v[13] && (v[13] != CAN)) return 0;
  if (v[ 2] && (v[ 2] != CAN)) return 0;
  if (v[ 8] && (v[ 8] != CAN)) return 0;
  if (v[ 3] && (v[ 3] != CAN)) return 0;
#ifdef DEBUGmatch
printf("match 0 - 1\n");
#endif
  return 1; // C = {x, x-ps, x-1, x-ps-1, x-rs, x-ps-rs, x-rs-1, x-ps-rs-1}
} // match_seq_vois0_1()

/* ==================================== */
static int32_t match_seq_vois0_2(uint8_t *v)
/* ==================================== */
/*
  11 10
   8  9

   2  1
  26  0

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqu�s 2M_CRUCIAL et non marqu�s 1M_CRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s 0M_CRUCIAL
*/
{
  //printf("match 02:\n");
  //print_vois(v);
  if (!((v[10]&&v[26]) || (v[11]&&v[0]) || (v[9]&&v[2]) || (v[8]&&v[1]) )) return 0;
  if (v[10] && (v[10] != CAN)) return 0;
  if (v[26] && (v[26] != CAN)) return 0;
  if (v[11] && (v[11] != CAN)) return 0;
  if (v[ 1] && (v[ 1] != CAN)) return 0;
  if (v[ 9] && (v[ 9] != CAN)) return 0;
  if (v[ 2] && (v[ 2] != CAN)) return 0;
  if (v[ 8] && (v[ 8] != CAN)) return 0;
  if (v[ 0] && (v[ 0] != CAN)) return 0;
#ifdef DEBUGmatch
printf("match 0 - 2\n");
#endif
  return 1; // C = {x, x-ps, x+1, x-ps+1, x-rs, x-ps-rs, x-rs+1, x-ps-rs+1}
} // match_seq_vois0_2()


/* ==================================== */
static int32_t match_seq_vois0_3(uint8_t *v)
/* ==================================== */
/*
   8  9
  15 16

  26  0
   6  7

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqu�s 2M_CRUCIAL et non marqu�s 1M_CRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s 0M_CRUCIAL
*/
{
  //printf("match 03:\n");
  //print_vois(v);
  if (!((v[16]&&v[26]) || (v[15]&&v[0]) || (v[6]&&v[9]) || (v[8]&&v[7]) )) return 0;
  if (v[16] && (v[16] != CAN)) return 0;
  if (v[26] && (v[26] != CAN)) return 0;
  if (v[15] && (v[15] != CAN)) return 0;
  if (v[ 0] && (v[ 0] != CAN)) return 0;
  if (v[ 6] && (v[ 6] != CAN)) return 0;
  if (v[ 9] && (v[ 9] != CAN)) return 0;
  if (v[ 8] && (v[ 8] != CAN)) return 0;
  if (v[ 7] && (v[ 7] != CAN)) return 0;
#ifdef DEBUGmatch
printf("match 0 - 3\n");
#endif
  return 1; // C = {x, x-ps, x+1, x-ps+1, x+rs, x-ps+rs, x+rs+1, x-ps+rs+1}
} // match_seq_vois0_3()

/* ==================================== */
static int32_t match_seq_vois0_4(uint8_t *v)
/* ==================================== */
/*
  13  8
  14 15

   4 26
   5  6

Teste si les conditions suivantes sont r�unies:
1: au moins un des ensembles ... est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqu�s 2M_CRUCIAL et non marqu�s 1M_CRUCIAL
Si le test r�ussit, les points non nuls sont marqu�s 0M_CRUCIAL
*/
{
  //printf("match 04:\n");
  //print_vois(v);
  if (!((v[14]&&v[26]) || (v[15]&&v[4]) || (v[13]&&v[6]) || (v[8]&&v[5]) )) return 0;
  if (v[14] && (v[14] != CAN)) return 0;
  if (v[26] && (v[26] != CAN)) return 0;
  if (v[15] && (v[15] != CAN)) return 0;
  if (v[ 4] && (v[ 4] != CAN)) return 0;
  if (v[13] && (v[13] != CAN)) return 0;
  if (v[ 6] && (v[ 6] != CAN)) return 0;
  if (v[ 8] && (v[ 8] != CAN)) return 0;
  if (v[ 5] && (v[ 5] != CAN)) return 0;
#ifdef DEBUGmatch
printf("match 0 - 4\n");
#endif
  return 1; // C = {x, x-ps, x-1, x-ps-1, x+rs, x-ps+rs, x+rs-1, x-ps+rs-1}
} // match_seq_vois0_4()

static void lskelCKSC3_aux2(uint8_t *Y,  int32_t x, int32_t x1, index_t rs, index_t ps, index_t N);
static void lskelCKSC3_aux1(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, index_t rs, index_t ps, index_t N);
static void lskelCKSC3_aux0(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, int32_t x4, int32_t x5, int32_t x6, int32_t x7, index_t rs, index_t ps, index_t N);

static void lskelCKSC3_process_cliques(struct xvimage *image, struct xvimage *imageY)
{
  uint8_t *F = UCHARDATA(image);
  uint8_t *Y = UCHARDATA(imageY);
  index_t rs = rowsize(image);
  index_t cs = colsize(image);
  index_t ds = depth(image);
  index_t ps = rs * cs;
  index_t N = ps * ds;
  int32_t i, j, k;
  index_t x, x1, x2, x3, x4, x5, x6, x7;
  uint8_t v[27];

    // DIM = 2
    // =============================
    for (k = 1; k < ds-1; k++) // dim = 2, C = {x, x-ps}
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 2a: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois2_a(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps;
	  lskelCKSC3_aux2(Y, x, x1, rs, ps, N);
	}
      }
    }

    for (k = 1; k < ds-1; k++) // dim = 2, C = {x, x-rs}
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 2b: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois2_b(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-rs;
	  lskelCKSC3_aux2(Y, x, x1, rs, ps, N);
	}
      }
    }

    for (k = 1; k < ds-1; k++) // dim = 2, C = {x, x-1}
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 2c: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois2_c(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-1;
	  lskelCKSC3_aux2(Y, x, x1, rs, ps, N);
	}
      }
    }

#ifdef DEBUG_lskelCKSC3_process_cliques
writeimage(imageY, "_Y2");
#endif

    // DIM = 1
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 1a1: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_a_1(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-rs; x2 = x-1; x3 = (x-rs)-1;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 1a2: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_a_2(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-rs; x2 = x+1; x3 = (x-rs)+1;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
      }
    }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 1b1: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_b_1(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x+rs; x2 = x-ps; x3 = x-ps+rs;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 1b2: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_b_2(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-rs; x2 = x-ps; x3 = x-ps-rs;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
      }
    }

    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 1c1: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_c_1(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x+1; x3 = x-ps+1;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 1c2: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois1_c_2(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x-1; x3 = x-ps-1;
	  lskelCKSC3_aux1(Y, x, x1, x2, x3, rs, ps, N);
	}
      }
    }

#ifdef DEBUG_lskelCKSC3_process_cliques
writeimage(imageY, "_Y1");
#endif

    // DIM = 0
    // =============================
    for (k = 1; k < ds-1; k++)
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	extract_vois3d(F, x, rs, ps, N, v);
#ifdef DEBUG_callmatch
printf("call match 0_1: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois0_1(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x-1; x3 = x-ps-1; 
	  x4 = x-rs; x5 = x-ps-rs; x6 = x-rs-1; x7 = x-ps-rs-1;
	  lskelCKSC3_aux0(Y, x, x1, x2, x3, x4, x5, x6, x7, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 0_2: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois0_2(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x+1; x3 = x-ps+1; 
	  x4 = x-rs; x5 = x-ps-rs; x6 = x-rs+1; x7 = x-ps-rs+1;
	  lskelCKSC3_aux0(Y, x, x1, x2, x3, x4, x5, x6, x7, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 0_3: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois0_3(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x+1; x3 = x-ps+1; 
	  x4 = x+rs; x5 = x-ps+rs; x6 = x+rs+1; x7 = x-ps+rs+1;
	  lskelCKSC3_aux0(Y, x, x1, x2, x3, x4, x5, x6, x7, rs, ps, N);
	}
#ifdef DEBUG_callmatch
printf("call match 0_4: %d %d %d\n", i, j, k);
#endif
	if (match_seq_vois0_4(v))
	{
#ifdef DCRUCIAL_MARK
	  insert_vois3d(v, F, x, rs, ps, N);
#endif
	  x1 = x-ps; x2 = x-1; x3 = x-ps-1; 
	  x4 = x+rs; x5 = x-ps+rs; x6 = x+rs-1; x7 = x-ps+rs-1;
	  lskelCKSC3_aux0(Y, x, x1, x2, x3, x4, x5, x6, x7, rs, ps, N);
	}
      }
    }
} // lskelCKSC3_process_cliques

#ifdef lskelCKSC3_VARIANTE_B0
static void lskelCKSC3_aux2(uint8_t *Y,  int32_t x, int32_t x1, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
#ifdef DEBUG_resmatch
  printf("match2: %d %d %d - %d %d %d\n", x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps);
#endif
  //     W = W u {points de C inter Y et non simples pour Y}
  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
    Set(x,IN_SET_W);
  }
  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
    Set(x1,IN_SET_W);
  }
  //     s'il existe des points dans C inter Y inter W alors
  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)))
  {
    //      retirer de Y les points de C non dans W
    if (Y[x] && !IsSet(x,IN_SET_W)) 
    {
#ifdef DEBUGlskelCKSC3
      printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
      Y[x] = 0;
#ifdef CONJECTURE_1
      for (k = 0; k < 26; k += 1)
      {
	y = voisin26(x, k, rs, ps, N);
	if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
	  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
		 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
      }
#endif
    }
    if (Y[x1] && !IsSet(x1,IN_SET_W)) 
    {
#ifdef DEBUGlskelCKSC3
      printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
      Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
    }
  }
  else
  {
    //      choisir un point x dans C inter Y
    //      W = W u {x}
    //      retirer de Y les autres points de C inter Y
    if (Y[x]) 
    { 
#ifdef DEBUGlskelCKSC3
      printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
      printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
      Y[x1] = 0; 
    } 
    else if (Y[x1]) 
    {
#ifdef DEBUGlskelCKSC3
      printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
      Set(x1,IN_SET_W);
    }
  }
} // lskelCKSC3_aux2

static void lskelCKSC3_aux1(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
#ifdef DEBUG_resmatch
printf("match1: %d %d %d - %d %d %d - %d %d %d - %d %d %d ; Y: %d %d %d %d\n", 
       x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps, x2%rs, (x2%ps)/rs, x2/ps, x3%rs, (x3%ps)/rs, x3/ps, Y[x], Y[x1], Y[x2], Y[x3]);
#endif
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	    Set(x1,IN_SET_W);
	  }
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	    Set(x2,IN_SET_W);
	  }
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	    Set(x3,IN_SET_W);
	  }
	  if (Y[x] && Y[x1] && !IsSet(x,IN_SET_W) && !IsSet(x1,IN_SET_W) && 
	      Clique2Critique(Y, x, x1, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set_2 W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x] && Y[x2] && !IsSet(x,IN_SET_W) && !IsSet(x2,IN_SET_W) && 
	      Clique2Critique(Y, x, x2, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set_2 W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x3] && Y[x1] && !IsSet(x3,IN_SET_W) && !IsSet(x1,IN_SET_W) && 
	      Clique2Critique(Y, x3, x1, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set_2 W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	    Set(x3,IN_SET_W);
	  }
	  if (Y[x3] && Y[x2] && !IsSet(x3,IN_SET_W) && !IsSet(x2,IN_SET_W) && 
	      Clique2Critique(Y, x3, x2, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set_2 W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	    Set(x3,IN_SET_W);
	  }
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)))
	  {
	    if (Y[x] && !IsSet(x,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Y[x] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x2] && !IsSet(x2,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	      Y[x2] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x2, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x2%rs, (x2%ps)/rs, x2/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x3] && !IsSet(x3,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x3] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x3, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x3%rs, (x3%ps)/rs, x3/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	  }
	  else
	  {
	    if (Y[x]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x1]) printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x1] = Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x1]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Set(x1,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x2]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	      Set(x2,IN_SET_W); 
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x3] = 0;
	    }
	    else if (Y[x3])
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Set(x3,IN_SET_W);
	    }
	  }
} // lskelCKSC3_aux1

static void lskelCKSC3_aux0(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, int32_t x4, int32_t x5, int32_t x6, int32_t x7, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) Set(x,IN_SET_W);
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) Set(x1,IN_SET_W);
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N)) Set(x2,IN_SET_W);
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N)) Set(x3,IN_SET_W);
	  if (Y[x4] && !mctopo3d_simple26(Y, x4, rs, ps, N)) Set(x4,IN_SET_W);
	  if (Y[x5] && !mctopo3d_simple26(Y, x5, rs, ps, N)) Set(x5,IN_SET_W);
	  if (Y[x6] && !mctopo3d_simple26(Y, x6, rs, ps, N)) Set(x6,IN_SET_W);
	  if (Y[x7] && !mctopo3d_simple26(Y, x7, rs, ps, N)) Set(x7,IN_SET_W);
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)) ||
	      (Y[x4] && IsSet(x4,IN_SET_W)) || (Y[x5] && IsSet(x5,IN_SET_W)) ||
	      (Y[x6] && IsSet(x6,IN_SET_W)) || (Y[x7] && IsSet(x7,IN_SET_W))
	     )
	  {
	    if (Y[x] && !IsSet(x,IN_SET_W))
	    {
	      Y[x] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W))
	    {
	      Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x2] && !IsSet(x2,IN_SET_W))
	    {
	      Y[x2] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x2, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x2%rs, (x2%ps)/rs, x2/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x3] && !IsSet(x3,IN_SET_W))
	    {
	      Y[x3] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x3, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x3%rs, (x3%ps)/rs, x3/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x4] && !IsSet(x4,IN_SET_W))
	    {
	      Y[x4] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x4, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x4%rs, (x4%ps)/rs, x4/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x5] && !IsSet(x5,IN_SET_W))
	    {
	      Y[x5] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x5, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x5%rs, (x5%ps)/rs, x5/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x6] && !IsSet(x6,IN_SET_W))
	    {
	      Y[x6] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x6, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x6%rs, (x6%ps)/rs, x6/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x7] && !IsSet(x7,IN_SET_W))
	    {
	      Y[x7] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x7, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x7%rs, (x7%ps)/rs, x7/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	  }
	  else
	  {
	    if (Y[x]) { Set(x,IN_SET_W); Y[x1] = Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x1]) { Set(x1,IN_SET_W); Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x2]) { Set(x2,IN_SET_W); Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x3]) { Set(x3,IN_SET_W); Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x4]) { Set(x4,IN_SET_W); Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x5]) { Set(x5,IN_SET_W); Y[x6] = Y[x7] = 0; }
	    else if (Y[x6]) { Set(x6,IN_SET_W); Y[x7] = 0; }
	    else if (Y[x7]) Set(x7,IN_SET_W);
	  }
} // lskelCKSC3_aux0

/* ==================================== */
int32_t lskelCKSC3(
		   struct xvimage *image, 
		   struct xvimage *inhibit,
		   int32_t nsteps)
/* ==================================== */
/* 3D binary sequential curvilinear thinning
 EXPERIMENTAL - Ne pas utiliser dans des applications

VARIANTE_B0
repeter
  Y = X ; W = {points non simples de X}
  pour dim = 2,1,0
    pour dir = a,b,c
      pour chaque clique critique C de X de dimension dim et de direction dir
        W = W u {points de C inter Y et non simples pour Y}
        s'il existe une clique C' dim+1 critique pour Y incluse dans C\W alors
          choisir un point de C' et le rajouter � W	
        s'il existe des points dans C inter Y inter W alors
          retirer de Y les points de C non dans W
        sinon
          choisir un point x dans C inter Y
          W = W u {x}
          retirer de Y les autres points de C inter Y      
  X = W
*/
#undef F_NAME
#define F_NAME "lskelCKSC3"
{ 
  int32_t i, j, k, t, tb, stab, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* objet */
  struct xvimage *imageY = copyimage(image);
  uint8_t *Y = UCHARDATA(imageY);

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  

#ifndef lskelCKSC3_SANS_CONTRAINTE
  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }
#endif

  mctopo3d_init_topo3d();  
  IndicsInit(N);
  //  Y = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(Y != NULL);

  if (nsteps == -1) nsteps = 1000000000;

  for (k = 1; k < ds-1; k++) 
  for (j = 1; j < cs-1; j++) 
  for (i = 1; i < rs-1; i++) 
  {
    x = k*ps + j*rs + i;
    if (F[x] && ((k<2)||(j<2)||(i<2)||(k>ds-3)||(j>cs-3)||(i>rs-3))) 
      printf("%s: WARNING - points on extended border may not be treated\n", F_NAME);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  stab = 0;
  while (!stab && (nbiter < nsteps))
  {
    nbiter++;
#ifdef VERBOSE
    printf("%s: nbiter %d\n", F_NAME, nbiter);
#endif
    stab = 1;

    for (x = 0; x < N; x++) if (F[x]) F[x] = OBJ;
    memset(Y, 0, N * sizeof(uint8_t));

    // d�tection des isthmes 1D
#ifndef lskelCKSC3_SANS_CONTRAINTE
    for (x = 0; x < N; x++) 
      if (F[x])
      { 
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
#endif

    for (k = 1; k < ds-1; k++) 
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	Y[x] = CAN;
	if (!mctopo3d_simple26(F, x, rs, ps, N) || IsSet(x,CONTRAINTE))
	  Set(x, IN_SET_W);
	else
	{
	  UnSet(x, IN_SET_W);
	  F[x] = CAN;
	}
      }
    }

    lskelCKSC3_process_cliques(image, imageY);

    for (x = 0; x < N; x++)
    {
      if (F[x] && !IsSet(x,IN_SET_W))
      {
        F[x] = 0;
	stab = 0;
      }
    }
  } // while (!stab && (nbiter < nsteps))

#ifdef CONJECTURE_2
  for (x = 0; x < N; x++) 
    if (F[x] && !IsSet(x,CONTRAINTE) && mctopo3d_simple26(F, x, rs, ps, N))
      printf("CONJECTURE 2 FAUSSE: x=%d %d %d\n", x%rs, (x%ps)/rs, x/ps);	
#endif

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  free(Y);
  return(1);
} /* lskelCKSC3() */
#endif

#ifdef lskelCKSC3_VARIANTE_B
static void lskelCKSC3_aux2(uint8_t *Y,  int32_t x, int32_t x1, index_t rs, index_t ps, index_t N)
{
#ifdef DEBUG_resmatch
printf("match2: %d %d %d - %d %d %d\n", x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps);
#endif
	  //     W = W u {points de C inter Y et non simples pour Y}
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	    Set(x1,IN_SET_W);
	  }
	  //     s'il existe des points dans C inter Y inter W alors
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)))
	  {
	    //      retirer de Y les points de C non dans W
	    if (Y[x] && !IsSet(x,IN_SET_W)) 
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Y[x] = 0;
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W)) 
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Y[x1] = 0;
	    }
	  }
	  else
	  {
	    //      choisir un point x dans C inter Y
	    //      W = W u {x}
	    //      retirer de Y les autres points de C inter Y
	    if (Y[x]) 
	    { 
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Y[x1] = 0; 
	    } 
	    else if (Y[x1]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Set(x1,IN_SET_W);
	    }
	  }
} // lskelCKSC3_aux2

static void lskelCKSC3_aux1(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, index_t rs, index_t ps, index_t N)
{
#ifdef DEBUG_resmatch
printf("match1: %d %d %d - %d %d %d - %d %d %d - %d %d %d ; Y: %d %d %d %d\n", 
       x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps, x2%rs, (x2%ps)/rs, x2/ps, x3%rs, (x3%ps)/rs, x3/ps, Y[x], Y[x1], Y[x2], Y[x3]);
#endif
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	    Set(x1,IN_SET_W);
	  }
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	    Set(x2,IN_SET_W);
	  }
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	    Set(x3,IN_SET_W);
	  }
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)))
	  {
	    //         s'il existe des points dans C inter Y inter W alors
	    //          retirer de Y les points de C non dans W
	    if (Y[x] && !IsSet(x,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
              if (mctopo3d_simple26(Y, x, rs, ps, N)) Y[x] = 0;
	      else Set(x,IN_SET_W);
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
              if (mctopo3d_simple26(Y, x1, rs, ps, N)) Y[x1] = 0;
	      else Set(x1,IN_SET_W);
	    }
	    if (Y[x2] && !IsSet(x2,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
              if (mctopo3d_simple26(Y, x2, rs, ps, N)) Y[x2] = 0;
	      else Set(x2,IN_SET_W);
	    }
	    if (Y[x3] && !IsSet(x3,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
              if (mctopo3d_simple26(Y, x3, rs, ps, N)) Y[x3] = 0;
	      else Set(x3,IN_SET_W);
	    }
	  }
	  else
	  {
	    if (Y[x]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x1]) printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x1] = Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x1]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Set(x1,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x2]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	      Set(x2,IN_SET_W); 
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x3] = 0;
	    }
	    else if (Y[x3])
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Set(x3,IN_SET_W);
	    }
	  }
} // lskelCKSC3_aux1

static void lskelCKSC3_aux0(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, int32_t x4, int32_t x5, int32_t x6, int32_t x7, index_t rs, index_t ps, index_t N)
{
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) Set(x,IN_SET_W);
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) Set(x1,IN_SET_W);
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N)) Set(x2,IN_SET_W);
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N)) Set(x3,IN_SET_W);
	  if (Y[x4] && !mctopo3d_simple26(Y, x4, rs, ps, N)) Set(x4,IN_SET_W);
	  if (Y[x5] && !mctopo3d_simple26(Y, x5, rs, ps, N)) Set(x5,IN_SET_W);
	  if (Y[x6] && !mctopo3d_simple26(Y, x6, rs, ps, N)) Set(x6,IN_SET_W);
	  if (Y[x7] && !mctopo3d_simple26(Y, x7, rs, ps, N)) Set(x7,IN_SET_W);
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)) ||
	      (Y[x4] && IsSet(x4,IN_SET_W)) || (Y[x5] && IsSet(x5,IN_SET_W)) ||
	      (Y[x6] && IsSet(x6,IN_SET_W)) || (Y[x7] && IsSet(x7,IN_SET_W))
	     )
	  {
	    if (mctopo3d_simple26(Y, x, rs, ps, N)) Y[x] = 0; else Set(x,IN_SET_W);
	    if (mctopo3d_simple26(Y, x1, rs, ps, N)) Y[x1] = 0; else Set(x1,IN_SET_W);
	    if (mctopo3d_simple26(Y, x2, rs, ps, N)) Y[x2] = 0; else Set(x2,IN_SET_W);
	    if (mctopo3d_simple26(Y, x3, rs, ps, N)) Y[x3] = 0; else Set(x3,IN_SET_W);
	    if (mctopo3d_simple26(Y, x4, rs, ps, N)) Y[x4] = 0; else Set(x4,IN_SET_W);
	    if (mctopo3d_simple26(Y, x5, rs, ps, N)) Y[x5] = 0; else Set(x5,IN_SET_W);
	    if (mctopo3d_simple26(Y, x6, rs, ps, N)) Y[x6] = 0; else Set(x6,IN_SET_W);
	    if (mctopo3d_simple26(Y, x7, rs, ps, N)) Y[x7] = 0; else Set(x7,IN_SET_W);
	  }
	  else
	  {
	    if (Y[x]) { Set(x,IN_SET_W); Y[x1] = Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x1]) { Set(x1,IN_SET_W); Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x2]) { Set(x2,IN_SET_W); Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x3]) { Set(x3,IN_SET_W); Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x4]) { Set(x4,IN_SET_W); Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x5]) { Set(x5,IN_SET_W); Y[x6] = Y[x7] = 0; }
	    else if (Y[x6]) { Set(x6,IN_SET_W); Y[x7] = 0; }
	    else if (Y[x7]) Set(x7,IN_SET_W);
	  }
} // lskelCKSC3_aux0

/* ==================================== */
int32_t lskelCKSC3(
		   struct xvimage *image, 
		   struct xvimage *inhibit,
		   int32_t nsteps)
/* ==================================== */
/* 3D binary sequential curvilinear thinning
 EXPERIMENTAL - Ne pas utiliser dans des applications

VARIANTE_B
repeter
  Y = X ; W = {points non simples de X}
  pour dim = 2,1,0
    pour dir = a,b,c
      pour chaque clique critique C de X de dimension dim et de direction dir
        W = W u {points de C inter Y et non simples pour Y}
        s'il existe des points dans C inter Y inter W alors
          pour tout x dans (C inter Y) \ W
	    si x simple pour Y alors Y = Y \ {x} sinon W = W u {x}
        sinon
          choisir un point x dans C inter Y
          W = W u {x}
          retirer de Y les autres points de C inter Y      
  X = W
*/
#undef F_NAME
#define F_NAME "lskelCKSC3"
{ 
  int32_t i, j, k, t, tb, stab, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* objet */
  struct xvimage *imageY = copyimage(image);
  uint8_t *Y = UCHARDATA(imageY);

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  

#ifndef lskelCKSC3_SANS_CONTRAINTE
  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }
#endif

  mctopo3d_init_topo3d();  
  IndicsInit(N);
  //  Y = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(Y != NULL);

  if (nsteps == -1) nsteps = 1000000000;

  for (k = 1; k < ds-1; k++) 
  for (j = 1; j < cs-1; j++) 
  for (i = 1; i < rs-1; i++) 
  {
    x = k*ps + j*rs + i;
    if (F[x] && ((k<2)||(j<2)||(i<2)||(k>ds-3)||(j>cs-3)||(i>rs-3))) 
      printf("%s: WARNING - points on extended border may not be treated\n", F_NAME);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  stab = 0;
  while (!stab && (nbiter < nsteps))
  {
    nbiter++;
#ifdef VERBOSE
    printf("%s: nbiter %d\n", F_NAME, nbiter);
#endif
    stab = 1;

    for (x = 0; x < N; x++) if (F[x]) F[x] = OBJ;
    memset(Y, 0, N * sizeof(uint8_t));

    // d�tection des isthmes 1D
#ifndef lskelCKSC3_SANS_CONTRAINTE
    for (x = 0; x < N; x++) 
      if (F[x])
      { 
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
      }
#endif

    for (k = 1; k < ds-1; k++) 
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	Y[x] = CAN;
	if (!mctopo3d_simple26(F, x, rs, ps, N) || IsSet(x,CONTRAINTE))
	  Set(x, IN_SET_W);
	else
	{
	  UnSet(x, IN_SET_W);
	  F[x] = CAN;
	}
      }
    }

    lskelCKSC3_process_cliques(image, imageY);

    for (x = 0; x < N; x++)
    {
      if (F[x] && !IsSet(x,IN_SET_W))
      {
        F[x] = 0;
	stab = 0;
      }
    }
  } // while (!stab && (nbiter < nsteps))

#ifdef CONJECTURE_2
  for (x = 0; x < N; x++) 
    if (F[x] && !IsSet(x,CONTRAINTE) && mctopo3d_simple26(F, x, rs, ps, N))
      printf("CONJECTURE 2 FAUSSE: x=%d %d %d\n", x%rs, (x%ps)/rs, x/ps);	
#endif

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  free(Y);
  return(1);
} /* lskelCKSC3() */
#endif

#ifdef lskelCKSC3_VARIANTE_A
static void lskelCKSC3_aux2(uint8_t *Y,  int32_t x, int32_t x1, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
#ifdef DEBUG_resmatch
printf("match2: %d %d %d - %d %d %d\n", x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps);
#endif
         // 	si C inter Y non vide
         if (Y[x] || Y[x1])
         {
	  //     W = W u {points de C inter Y et non simples pour Y}
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) 
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	    Set(x1,IN_SET_W);
	  }
	  //     s'il existe des points dans C inter Y inter W alors
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)))
	  {
	    //      retirer de Y les points de C non dans W
	    if (Y[x] && !IsSet(x,IN_SET_W)) 
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Y[x] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W)) 
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	  }
	  else
	  {
	    //      choisir un point x dans C inter Y
	    //      W = W u {x}
	    //      retirer de Y les autres points de C inter Y
	    if (Y[x]) 
	    { 
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Y[x1] = 0; 
	    } 
	    else if (Y[x1]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Set(x1,IN_SET_W);
	    }
	  }
	 }
	 else // C inter Y est vide
	 {
	   Y[x] = CAN;
	   Set(x,IN_SET_W);
	 }
} // lskelCKSC3_aux2

static void lskelCKSC3_aux1(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
#ifdef DEBUG_resmatch
printf("match1: %d %d %d - %d %d %d - %d %d %d - %d %d %d ; Y: %d %d %d %d\n", 
       x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps, x2%rs, (x2%ps)/rs, x2/ps, x3%rs, (x3%ps)/rs, x3/ps, Y[x], Y[x1], Y[x2], Y[x3]);
#endif
         // 	si C inter Y non vide
         if (Y[x] || Y[x1] || Y[x2] || Y[x3])
         {
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	    Set(x,IN_SET_W);
	  }
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	    Set(x1,IN_SET_W);
	  }
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	    Set(x2,IN_SET_W);
	  }
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N))
	  {
#ifdef DEBUGlskelCKSC3
printf("set W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	    Set(x3,IN_SET_W);
	  }
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)))
	  {
	    //         s'il existe des points dans C inter Y inter W alors
	    //          retirer de Y les points de C non dans W
	    if (Y[x] && !IsSet(x,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
              Y[x] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
              Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x2] && !IsSet(x2,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
              Y[x2] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x2, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x2%rs, (x2%ps)/rs, x2/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x3] && !IsSet(x3,IN_SET_W))
	    {
#ifdef DEBUGlskelCKSC3
printf("unset Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
              Y[x3] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x3, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x3%rs, (x3%ps)/rs, x3/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	  }
	  else
	  {
	    if (Y[x]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
	      Set(x,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x1]) printf("unset_ Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x1] = Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x1]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
	      Set(x1,IN_SET_W);
#ifdef DEBUGlskelCKSC3
	      if (Y[x2]) printf("unset_ Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x2] = Y[x3] = 0;
	    }
	    else if (Y[x2]) 
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
	      Set(x2,IN_SET_W); 
#ifdef DEBUGlskelCKSC3
	      if (Y[x3]) printf("unset_ Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Y[x3] = 0;
	    }
	    else if (Y[x3])
	    {
#ifdef DEBUGlskelCKSC3
printf("set_ W: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
	      Set(x3,IN_SET_W);
	    }
	  }
	 }
	 else // C inter Y est vide
	 {
	   Y[x] = CAN;
	   Set(x,IN_SET_W);
	 }
} // lskelCKSC3_aux1

static void lskelCKSC3_aux0(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, int32_t x4, int32_t x5, int32_t x6, int32_t x7, index_t rs, index_t ps, index_t N)
{
#ifdef CONJECTURE_1
  int32_t k;
  index_t y;
#endif
         // 	si C inter Y non vide
         if (Y[x] || Y[x1] || Y[x2] || Y[x3] || Y[x4] || Y[x5] || Y[x6] || Y[x7])
         {
	  if (Y[x] && !mctopo3d_simple26(Y, x, rs, ps, N)) Set(x,IN_SET_W);
	  if (Y[x1] && !mctopo3d_simple26(Y, x1, rs, ps, N)) Set(x1,IN_SET_W);
	  if (Y[x2] && !mctopo3d_simple26(Y, x2, rs, ps, N)) Set(x2,IN_SET_W);
	  if (Y[x3] && !mctopo3d_simple26(Y, x3, rs, ps, N)) Set(x3,IN_SET_W);
	  if (Y[x4] && !mctopo3d_simple26(Y, x4, rs, ps, N)) Set(x4,IN_SET_W);
	  if (Y[x5] && !mctopo3d_simple26(Y, x5, rs, ps, N)) Set(x5,IN_SET_W);
	  if (Y[x6] && !mctopo3d_simple26(Y, x6, rs, ps, N)) Set(x6,IN_SET_W);
	  if (Y[x7] && !mctopo3d_simple26(Y, x7, rs, ps, N)) Set(x7,IN_SET_W);
	  if ((Y[x] && IsSet(x,IN_SET_W)) || (Y[x1] && IsSet(x1,IN_SET_W)) ||
	      (Y[x2] && IsSet(x2,IN_SET_W)) || (Y[x3] && IsSet(x3,IN_SET_W)) ||
	      (Y[x4] && IsSet(x4,IN_SET_W)) || (Y[x5] && IsSet(x5,IN_SET_W)) ||
	      (Y[x6] && IsSet(x6,IN_SET_W)) || (Y[x7] && IsSet(x7,IN_SET_W))
	     )
	  {
	    if (Y[x] && !IsSet(x,IN_SET_W))
	    {
	      Y[x] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x%rs, (x%ps)/rs, x/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x1] && !IsSet(x1,IN_SET_W))
	    {
	      Y[x1] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x1, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x1%rs, (x1%ps)/rs, x1/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x2] && !IsSet(x2,IN_SET_W))
	    {
	      Y[x2] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x2, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x2%rs, (x2%ps)/rs, x2/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x3] && !IsSet(x3,IN_SET_W))
	    {
	      Y[x3] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x3, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x3%rs, (x3%ps)/rs, x3/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x4] && !IsSet(x4,IN_SET_W))
	    {
	      Y[x4] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x4, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x4%rs, (x4%ps)/rs, x4/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x5] && !IsSet(x5,IN_SET_W))
	    {
	      Y[x5] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x5, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x5%rs, (x5%ps)/rs, x5/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x6] && !IsSet(x6,IN_SET_W))
	    {
	      Y[x6] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x6, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x6%rs, (x6%ps)/rs, x6/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	    if (Y[x7] && !IsSet(x7,IN_SET_W))
	    {
	      Y[x7] = 0;
#ifdef CONJECTURE_1
	      for (k = 0; k < 26; k += 1)
	      {
		y = voisin26(x7, k, rs, ps, N);
		if ((y != -1) && (Y[y]) && (IsSet(y,IN_SET_W) && mctopo3d_simple26(Y, y, rs, ps, N)))
		  printf("CONJECTURE 1 FAUSSE: x=%d %d %d y=%d %d %d\n",
			 x7%rs, (x7%ps)/rs, x7/ps, y%rs, (y%ps)/rs, y/ps);		  
	      }
#endif
	    }
	  }
	  else
	  {
	    if (Y[x]) { Set(x,IN_SET_W); Y[x1] = Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x1]) { Set(x1,IN_SET_W); Y[x2] = Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x2]) { Set(x2,IN_SET_W); Y[x3] = Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x3]) { Set(x3,IN_SET_W); Y[x4] = Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x4]) { Set(x4,IN_SET_W); Y[x5] = Y[x6] = Y[x7] = 0; }
	    else if (Y[x5]) { Set(x5,IN_SET_W); Y[x6] = Y[x7] = 0; }
	    else if (Y[x6]) { Set(x6,IN_SET_W); Y[x7] = 0; }
	    else if (Y[x7]) Set(x7,IN_SET_W);
	  }
	 }
	 else // C inter Y est vide
	 {
	   Y[x] = CAN;
	   Set(x,IN_SET_W);
	 }
} // lskelCKSC3_aux0

static int32_t NKP_end(uint8_t *S, index_t p, index_t rs, index_t ps, index_t N)
{
  int32_t k, n;
  index_t y, q, r;
  for (n = k = 0; k < 26; k += 1)
  {
    y = voisin26(p, k, rs, ps, N);
    if ((y != -1) && S[y]) { n++; q = y; }
  } // for k
  if (n != 1) return 0;
  for (n = k = 0; k < 26; k += 1)
  {
    y = voisin26(q, k, rs, ps, N);
    if ((y != -1) && S[y] && (y != p)) { n++; r = y; }
  } // for k
  if (n == 0) return 1;
  if ((n == 1) &&
      mctopo3d_nbvoiso26(S, r, rs, ps, N) <= 2) return 1;
  return 0;
} //NKP_end()

/* ==================================== */
int32_t lskelCKSC3(
		   struct xvimage *image, 
		   struct xvimage *inhibit,
		   int32_t nsteps)
/* ==================================== */
/* 3D binary sequential curvilinear thinning
 EXPERIMENTAL - Ne pas utiliser dans des applications

VARIANTE_A
repeter
  Y = X ; W = {points non simples de X}
  pour dim = 2,1,0
    pour dir = a,b,c
      pour chaque clique critique C de X de dimension dim et de direction dir
        W = W u {points de C inter Y et non simples pour Y}
	si C inter Y non vide
          s'il existe des points dans C inter Y inter W alors
            pour tout x dans (C inter Y) \ W faire Y = Y \ {x}
          sinon
            choisir un point x dans C inter Y
            W = W u {x}
            retirer de Y les autres points de C inter Y      
	sinon
	  choisir un point x dans C
	  Y = Y u {x}
  X = W
*/
#undef F_NAME
#define F_NAME "lskelCKSC3"
{ 
  int32_t i, j, k, t, tb, stab, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* objet */
  struct xvimage *imageY = copyimage(image);
  uint8_t *Y = UCHARDATA(imageY);

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  

#ifndef lskelCKSC3_SANS_CONTRAINTE
  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }
#endif

  mctopo3d_init_topo3d();  
  IndicsInit(N);
  //  Y = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(Y != NULL);

  if (nsteps == -1) nsteps = 1000000000;

  for (k = 1; k < ds-1; k++) 
  for (j = 1; j < cs-1; j++) 
  for (i = 1; i < rs-1; i++) 
  {
    x = k*ps + j*rs + i;
    if (F[x] && ((k<2)||(j<2)||(i<2)||(k>ds-3)||(j>cs-3)||(i>rs-3))) 
      printf("%s: WARNING - points on extended border may not be treated\n", F_NAME);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  stab = 0;
  while (!stab && (nbiter < nsteps))
  {
    nbiter++;
#ifdef VERBOSE
    printf("%s: nbiter %d\n", F_NAME, nbiter);
#endif
    stab = 1;

    for (x = 0; x < N; x++) if (F[x]) F[x] = OBJ;
    memset(Y, 0, N * sizeof(uint8_t));

    // d�tection des isthmes 1D
#ifndef lskelCKSC3_SANS_CONTRAINTE
    for (x = 0; x < N; x++) 
      if (F[x])
      { 
#ifdef USE_NKP_END
	if (NKP_end(F, x, rs, ps, N)) Set(x,CONTRAINTE);
#else
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (!IsSet(x,CONTRAINTE))
	{
	  if (t > 1) Set(x,CONTRAINTE);
	}
	else
	{
	  if ((t == 1) && (mctopo3d_nbvoiso26(F, x, rs, ps, N) > 1)) 
	    UnSet(x,CONTRAINTE);
	}
#endif
      }
#endif

    for (k = 1; k < ds-1; k++) 
    for (j = 1; j < cs-1; j++) 
    for (i = 1; i < rs-1; i++) 
    {
      x = k*ps + j*rs + i;
      if (F[x])
      { 
	Y[x] = CAN;
	if (!mctopo3d_simple26(F, x, rs, ps, N) || IsSet(x,CONTRAINTE))
	  Set(x, IN_SET_W);
	else
	{
	  UnSet(x, IN_SET_W);
	  F[x] = CAN;
	}
      }
    }

    lskelCKSC3_process_cliques(image, imageY);

    for (x = 0; x < N; x++)
    {
      if (F[x] && !IsSet(x,IN_SET_W))
      {
        F[x] = 0;
	stab = 0;
      }
    }
  } // while (!stab && (nbiter < nsteps))

#ifdef CONJECTURE_2
  for (x = 0; x < N; x++) 
    if (F[x] && !IsSet(x,CONTRAINTE) && mctopo3d_simple26(F, x, rs, ps, N))
      printf("CONJECTURE 2 FAUSSE: x=%d %d %d\n", x%rs, (x%ps)/rs, x/ps);	
#endif

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  free(Y);
  return(1);
} /* lskelCKSC3() */
#endif

#ifdef lskelCKSC3_VARIANTE_C
static void lskelCKSC3_aux2(uint8_t *Y,  int32_t x, int32_t x1, index_t rs, index_t ps, index_t N)
{
  int32_t n = 0;
  Set(x,IN_SET_K); Set(x1,IN_SET_K);
  if (Y[x]) n++; if (Y[x1]) n++;
#ifdef DEBUG_resmatch
  printf("match2: %d %d %d - %d %d %d\n", x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps);
#endif
  if ((n == 1) && Y[x]) Set(x,IN_SET_W);
  if (Y[x] && !IsSet(x,IN_SET_W) && mctopo3d_simple26(Y, x, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(2) from Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
    Y[x] = 0; n--;
  }
  if ((n == 1) && Y[x1]) Set(x1,IN_SET_W);
  if (Y[x1] && !IsSet(x,IN_SET_W) && mctopo3d_simple26(Y, x1, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(2) from Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
    Y[x1] = 0;
  }
} // lskelCKSC3_aux2

static void lskelCKSC3_aux1(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, index_t rs, index_t ps, index_t N)
{
  int32_t n = 0;
  Set(x,IN_SET_K); Set(x1,IN_SET_K); Set(x2,IN_SET_K); Set(x3,IN_SET_K);
  if (Y[x]) n++; if (Y[x1]) n++; if (Y[x2]) n++; if (Y[x3]) n++;
#ifdef DEBUG_resmatch
  printf("match1: %d %d %d - %d %d %d - %d %d %d - %d %d %d ; Y: %d %d %d %d\n", 
	 x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps, x2%rs, (x2%ps)/rs, x2/ps, x3%rs, (x3%ps)/rs, x3/ps, Y[x], Y[x1], Y[x2], Y[x3]);
#endif
  if ((n == 1) && Y[x]) Set(x,IN_SET_W);
  if (Y[x] && !IsSet(x,IN_SET_W) && mctopo3d_simple26(Y, x, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(1) from Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
    Y[x] = 0; n--;
  }
  if ((n == 1) && Y[x1]) Set(x1,IN_SET_W);
  if (Y[x1] && !IsSet(x1,IN_SET_W) && mctopo3d_simple26(Y, x1, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(1) from Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
    Y[x1] = 0; n--;
  }
  if ((n == 1) && Y[x2]) Set(x2,IN_SET_W);
  if (Y[x2] && !IsSet(x2,IN_SET_W) && mctopo3d_simple26(Y, x2, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(1) from Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
    Y[x2] = 0; n--;
  }
  if ((n == 1) && Y[x3]) Set(x3,IN_SET_W);
  if (Y[x3] && !IsSet(x3,IN_SET_W) && mctopo3d_simple26(Y, x3, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(1) from Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
    Y[x3] = 0;
  }
} // lskelCKSC3_aux1

static void lskelCKSC3_aux0(uint8_t *Y,  int32_t x, int32_t x1, int32_t x2, int32_t x3, int32_t x4, int32_t x5, int32_t x6, int32_t x7, index_t rs, index_t ps, index_t N)
{
  int32_t n = 0;
  Set(x,IN_SET_K); Set(x1,IN_SET_K); Set(x2,IN_SET_K); Set(x3,IN_SET_K);
  Set(x4,IN_SET_K); Set(x5,IN_SET_K); Set(x6,IN_SET_K); Set(x7,IN_SET_K);
  if (Y[x]) n++; if (Y[x1]) n++; if (Y[x2]) n++; if (Y[x3]) n++;
  if (Y[4]) n++; if (Y[x5]) n++; if (Y[x6]) n++; if (Y[x7]) n++;
#ifdef DEBUG_resmatch
  printf("match0: %d %d %d - %d %d %d - %d %d %d - %d %d %d -  %d %d %d - %d %d %d - %d %d %d - %d %d %d\n Y: %d %d %d %d / %d %d %d %d\n", 
	 x%rs, (x%ps)/rs, x/ps, x1%rs, (x1%ps)/rs, x1/ps, x2%rs, (x2%ps)/rs, x2/ps, x3%rs, (x3%ps)/rs, x3/ps,
	 x4%rs, (x4%ps)/rs, x4/ps, x5%rs, (x5%ps)/rs, x5/ps, x6%rs, (x6%ps)/rs, x6/ps, x7%rs, (x7%ps)/rs, x7/ps, 
	 Y[x], Y[x1], Y[x2], Y[x3], Y[x4], Y[x5], Y[x6], Y[x7]);
#endif
  if ((n == 1) && Y[x]) Set(x,IN_SET_W);
  if (Y[x] && !IsSet(x,IN_SET_W) && mctopo3d_simple26(Y, x, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
    Y[x] = 0; n--;
  }
  if ((n == 1) && Y[x1]) Set(x1,IN_SET_W);
  if (Y[x1] && !IsSet(x1,IN_SET_W) && mctopo3d_simple26(Y, x1, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x1%rs, (x1%ps)/rs, x1/ps);
#endif
    Y[x1] = 0; n--;
  }
  if ((n == 1) && Y[x2]) Set(x2,IN_SET_W);
  if (Y[x2] && !IsSet(x2,IN_SET_W) && mctopo3d_simple26(Y, x2, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x2%rs, (x2%ps)/rs, x2/ps);
#endif
    Y[x2] = 0; n--;
  }
  if ((n == 1) && Y[x3]) Set(x3,IN_SET_W);
  if (Y[x3] && !IsSet(x3,IN_SET_W) && mctopo3d_simple26(Y, x3, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x3%rs, (x3%ps)/rs, x3/ps);
#endif
    Y[x3] = 0; n--;
  }
  if ((n == 1) && Y[x4]) Set(x4,IN_SET_W);
  if (Y[x4] && !IsSet(x4,IN_SET_W) && mctopo3d_simple26(Y, x4, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x4%rs, (x4%ps)/rs, x4/ps);
#endif
    Y[x4] = 0; n--;
  }
  if ((n == 1) && Y[x5]) Set(x5,IN_SET_W);
  if (Y[x5] && !IsSet(x5,IN_SET_W) && mctopo3d_simple26(Y, x5, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x5%rs, (x5%ps)/rs, x5/ps);
#endif
    Y[x5] = 0; n--;
  }
  if ((n == 1) && Y[x6]) Set(x6,IN_SET_W);
  if (Y[x6] && !IsSet(x6,IN_SET_W) && mctopo3d_simple26(Y, x6, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x6%rs, (x6%ps)/rs, x6/ps);
#endif
    Y[x6] = 0; n--;
  }
  if ((n == 1) && Y[x7]) Set(x7,IN_SET_W);
  if (Y[x7] && !IsSet(x7,IN_SET_W) && mctopo3d_simple26(Y, x7, rs, ps, N)) 
  {
#ifdef DEBUGlskelCKSC3
    printf("delete(0) from Y: %d %d %d\n", x7%rs, (x7%ps)/rs, x7/ps);
#endif
    Y[x7] = 0;
  }
} // lskelCKSC3_aux0

/* ==================================== */
int32_t lskelCKSC3(
		   struct xvimage *image, 
		   struct xvimage *inhibit,
		   int32_t nsteps)
/* ==================================== */
/* 3D binary sequential curvilinear thinning
 EXPERIMENTAL - Ne pas utiliser dans des applications

VARIANTE_C
repeter
  Y = X; 
  K = {};  // union des cliques critiques pour X
  W = {};  // marqueur des 'derniers points dans une clique critique'
  pour dim = 2,1,0
    pour dir = a,b,c
      pour chaque clique critique C de X de dimension dim et de direction dir
        pour tout x dans C
	  K = K u {x}
	  si |C inter Y| == 1 faire W = W u {x} 
	  si x hors de W et x simple pour Y faire Y = Y \ {x}
  pour chaque point de Y \ K simple pour X faire Y = Y \ {x}
  X = Y
*/
#undef F_NAME
#define F_NAME "lskelCKSC3"
{ 
  int32_t i, j, k, t, tb, stab, nbiter;
  index_t x;                       /* index de pixel */
  index_t rs = rowsize(image);     /* taille ligne */
  index_t cs = colsize(image);     /* taille colonne */
  index_t ps = rs * cs;            /* taille plan */
  index_t ds = depth(image);       /* nb plans */
  index_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);   /* objet */
  struct xvimage *imageY = copyimage(image);
  uint8_t *Y = UCHARDATA(imageY);

  ONLY_3D(image);
  ACCEPTED_TYPES1(image, VFF_TYP_1_BYTE);  

#ifndef lskelCKSC3_SANS_CONTRAINTE
  if (inhibit != NULL)
  {
    uint8_t *I;
    COMPARE_SIZE(image, inhibit);
    ACCEPTED_TYPES1(inhibit, VFF_TYP_1_BYTE);
    I = UCHARDATA(inhibit);
    for (x = 0; x < N; x++) if (I[x]) Set(x,CONTRAINTE);
  }
#endif

  mctopo3d_init_topo3d();  
  IndicsInit(N);
  //  Y = (uint8_t *)calloc(N, sizeof(uint8_t)); assert(Y != NULL);

  if (nsteps == -1) nsteps = 1000000000;

  for (k = 1; k < ds-1; k++) 
  for (j = 1; j < cs-1; j++) 
  for (i = 1; i < rs-1; i++) 
  {
    x = k*ps + j*rs + i;
    if (F[x] && ((k<2)||(j<2)||(i<2)||(k>ds-3)||(j>cs-3)||(i>rs-3))) 
      printf("%s: WARNING - points on extended border may not be treated\n", F_NAME);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  nbiter = 0;
  stab = 0;
  while (!stab && (nbiter < nsteps))
  {
    nbiter++;
#ifdef VERBOSE
    printf("%s: nbiter %d\n", F_NAME, nbiter);
#endif
    stab = 1;

    for (x = 0; x < N; x++) if (F[x]) F[x] = OBJ;
    memset(Y, 0, N * sizeof(uint8_t));

    // d�tection des isthmes 1D
#ifndef lskelCKSC3_SANS_CONTRAINTE
    for (x = 0; x < N; x++) 
      if (F[x])
      { 
	mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
	if (t > 1) Set(x,CONTRAINTE);
	if ((t == 1) && (tb == 1)) F[x] = CAN;
      }
#endif

    for (x = 0; x < N; x++) 
      if (F[x])
      { 
	Y[x] = OBJ;
	UnSet(x, IN_SET_K);
	UnSet(x, IN_SET_W);
	if (IsSet(x,CONTRAINTE)) F[x] = OBJ;
      }

    lskelCKSC3_process_cliques(image, imageY);

    //  pour chaque point de Y \ W simple pour X faire Y = Y \ {x}
    for (x = 0; x < N; x++)
      if (Y[x] && !IsSet(x,IN_SET_K) && 
	  !IsSet(x,CONTRAINTE) && mctopo3d_simple26(F, x, rs, ps, N))
      {
#ifdef DEBUGlskelCKSC3
	printf("delete(S) from Y: %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
#endif
        Y[x] = 0;
      }

    //  X = Y
    for (x = 0; x < N; x++)
      if (F[x] && !Y[x])
      {
	if (IsSet(x,CONTRAINTE))	
	  printf("BUG deleting constraint point %d %d %d\n", x%rs, (x%ps)/rs, x/ps);
	F[x] = 0;
	stab = 0;
      }

  } // while (!stab && (nbiter < nsteps))

#ifdef CONJECTURE_2
  for (x = 0; x < N; x++) 
    if (F[x] && !IsSet(x,CONTRAINTE) && mctopo3d_simple26(F, x, rs, ps, N))
      printf("CONJECTURE 2 FAUSSE: x=%d %d %d\n", x%rs, (x%ps)/rs, x/ps);	
#endif

  for (x = 0; x < N; x++) if (F[x]) F[x] = NDG_MAX;

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  mctopo3d_termine_topo3d();
  free(Y);
  return(1);
} /* lskelCKSC3() */
#endif
