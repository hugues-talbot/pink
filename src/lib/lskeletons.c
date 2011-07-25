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
#include <mcrlifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <pinktypes.h>
#include <ldist.h>
#include <lskeletons.h>

#define EN_RBT        0
#define EN_FIFO       1
#define CONTRAINTE    2
#define CONTRAINTE1   2
#define CONTRAINTE2   3
#define PARANO

//#define VERBOSE
//#define DEBUG

//#define PRIODIR

//#define TEST_SIMPLE_PAR_COLLAPSE

/* ==================================== */
static int32_t typedir3d(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
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
} /* typedir3d() */

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
} /* typedir3d() */

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
static int32_t lskeletons_testabaisse6bin(uint8_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple6(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* lskeletons_testabaisse6bin() */

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
static int32_t lskeletons_testabaisse6lab(int32_t *F, index_t x, index_t rs, index_t ps, index_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (mctopo3d_simple6lab(F, x, rs, ps, N)) { modifie = 1; F[x] = 0; }
  return modifie;
} /* lskeletons_testabaisse6lab() */

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
Les points ayant une valeur supérieure ou égale à une valeur 
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
      x = RbtPopMin(RBT);
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
      x = RbtPopMin(RBT);
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
      x = RbtPopMin(RBT);
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
      x = RbtPopMin(RBT);
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
Les points ayant une valeur supérieure ou égale à une valeur 
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (lskeletons_testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
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
      } /* if (lskeletons_testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
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
  index_t d = depth(image);
  index_t N = d * ps;              /* taille image */
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
  if (imageinhib != NULL) I = UCHARDATA(imageinhib);

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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (lskeletons_testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
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
      } /* if (lskeletons_testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
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
int32_t lskelcurv(struct xvimage *image, 
              struct xvimage *imageprio, 
              struct xvimage *inhibit, 
              int32_t connex)
/* ==================================== */
/* 
Squelette curviligne 2D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

On definit l'operateur Curvilinear Thinning CT(F,P) : 
C = {y in F | T(y) > 1}
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C[y] = 1
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
	if (t > 1) Set(x, CONTRAINTE);
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
	if (t > 1) Set(x, CONTRAINTE);
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
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*10+typedir(F,x,rs,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*10+typedir(F,x,rs,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*10+typedir(F,x,rs,N),x); break;
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
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir(F, x, rs, N));
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
        case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[x]*10+typedir(F,x,rs,N),x); break;
        case VFF_TYP_1_BYTE: assert(0); break;
        case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[x]*10+typedir(F,x,rs,N),x); break;
        case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[x]*10+typedir(F,x,rs,N),x); break;
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
printf("init: push %d,%d (%d)\n", x%rs, x/rs, P[x]*10 + typedir(F, x, rs, N));
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
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse4bin(F, x, rs, N))
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
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*10+typedir(F,y,rs,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*10+typedir(F,y,rs,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*10+typedir(F,y,rs,N),y); break;
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
        if ((! IsSet(x, CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top4(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 4) */
  else if (connex == 8)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse8bin(F, x, rs, N))
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
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	      {
#ifdef PRIODIR
		switch(datatype(imageprio))
		{
		case VFF_TYP_4_BYTE: mcrbt_RbtInsert(&RBT,P[y]*10+typedir(F,y,rs,N),y); break;
		case VFF_TYP_1_BYTE: assert(0); break;
		case VFF_TYP_FLOAT : mcrbt_RbtInsert(&RBT,PF[y]*10+typedir(F,y,rs,N),y); break;
		case VFF_TYP_DOUBLE: mcrbt_RbtInsert(&RBT,PD[y]*10+typedir(F,y,rs,N),y); break;
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
        if ((! IsSet(x, CONTRAINTE)) && (nonbord(x,rs,N)))
        {
          top8(F, x, rs, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
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
Squelette curviligne 3D binaire guide par une image de priorites.
Les valeurs les plus basses correspondent a la plus grande priorite.

De facon tres schematique,
on definit l'operateur Curvilinear Thinning CT(F,P) : 
C = {y in F | T(y) > 1}
repeter jusqu'a stabilite
  choisir un point x de F, simple pour F, tel que C[x] == 0 
    et de priorite maximale (valeur de P minimale)
  F = F \ {x}
  pour tout y dans gamma(x)
    si T(y) > 1 alors C[y] = 1
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
	if (t > 1) Set(x, CONTRAINTE);
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
	if (t > 1) Set(x, CONTRAINTE);
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
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && lskeletons_testabaisse6bin(F, x, rs, ps, N))
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
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
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
        if ((! IsSet(x, CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
          if (t > 1) Set(x, CONTRAINTE);
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x, CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y, CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              mctopo3d_top26(F, y, rs, ps, N, &t, &tb);
              if (t > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
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
	if (tb > 1) Set(x, CONTRAINTE);
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
	if (tb > 1) Set(x, CONTRAINTE);
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
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!mcrbt_RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!mcrbt_RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && lskeletons_testabaisse6bin(F, x, rs, ps, N))
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
              if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
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
        if ((! IsSet(x, CONTRAINTE)) && (nonbord3d(x,rs,ps,N)))
        {
          mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if ((! IsSet(x, CONTRAINTE)) && testabaisse26bin(F, x, rs, ps, N))
      {
        for (k = 0; k < 26; k += 1)         /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]))
          {
            if ((! IsSet(y, CONTRAINTE)) && (nonbord3d(y,rs,ps,N)))
	    {
              mctopo3d_top26(F, y, rs, ps, N, &t, &tb);
              if (tb > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
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
    for (x = 0; x < N; x++) if (I[x]) Set(x, CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x, CONTRAINTE) && bordext8(F, x, rs, N))
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top4(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top8(F, x, rs, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 8; k += 1)          /* parcourt les voisins en 8-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin(x, k, rs, N);                             /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
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
    for (x = 0; x < N; x++) if (I[x]) Set(x, CONTRAINTE);
  }

  /* ================================================ */
  /*               DEBUT ALGO                         */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DU RBT */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (F[x] && !IsSet(x, CONTRAINTE) && mctopo3d_bordext26(F, x, rs, ps, N))
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
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top6(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
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
      } /* if (lskeletons_testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 18)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top18(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
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
      } /* if (lskeletons_testabaisse6bin(F, x, rs, N)) */
    } /* while (!mcrbt_RbtVide(RBT)) */
  } /* if (connex == 18) */
  else
  if (connex == 26)
  {
    while (!mcrbt_RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      mctopo3d_top26(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
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
      } /* if (lskeletons_testabaisse6bin(F, x, rs, N)) */
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
      prio = RbtMinLevel(RBT); 
      x = RbtPopMin(RBT);
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
        I[x] = 1; // le point x ne sera plus considéré, il fait partie du squelette
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
      prio = RbtMinLevel(RBT); 
      x = RbtPopMin(RBT);
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
        I[x] = 1; // le point x ne sera plus considéré, il fait partie du squelette
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

Le prédicat "endpoint" est défini par un tableau de 2^27 booléens 
(codés sur 2^24 octets) passé en paramètre.

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
      x = RbtPopMin(RBT);
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
int32_t lskelend3d(struct xvimage *image, 
		   int32_t connex, 
		   uint8_t *endpoint,
		   int32_t niseuil)
/* ==================================== */
/* 
Squelette curviligne ou surfacique 3D binaire.

Algo par passes directionnelles.

Le prédicat "endpoint" est défini par un tableau de 2^27 booléens 
(codés sur 2^24 octets) passé en paramètre.
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
	x = RbtPopMin(RBT);
	config = encodevois(x, F, rs, ps, N);
	if (((nbiter < niseuil) || (!IsEnd(config))) && lskeletons_testabaisse6bin(F, x, rs, ps, N)) nbdel++;
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
	x = RbtPopMin(RBT);
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
	{ int32_t lev = RbtMinLevel(RBT);
	printf("pop: prio %d ", lev); }
#endif	
	x = RbtPopMin(RBT);
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
int32_t lskelendcurvlab3d(struct xvimage *image, 
			  int32_t connex, 
			  int32_t niseuil)
/* ==================================== */
/* 
Squelette curviligne 3D sur une image de labels (chaque label est traité comme une image binaire).

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
	x = RbtPopMin(RBT);
	if (((nbiter < niseuil) || (mctopo3d_nbvoislab6(F, x, rs, ps, N) > 1)) && lskeletons_testabaisse6lab(F, x, rs, ps, N)) nbdel++;
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
	x = RbtPopMin(RBT);
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
	x = RbtPopMin(RBT);
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
// SQUELETTES GUIDES PARALLELES - POINTS P_SIMPLES
// ========================================================
// ========================================================

/* ==================================== */
int32_t lskelPSG(struct xvimage *image, 
		 struct xvimage *imageprio, 
		 double val)
/* ==================================== */
/*
Squelette symétrique guidé ultime basé sur les points P-simples

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
    curprio = RbtMinLevel(RBT);
#define DEBUG_lskelPSG2
#ifdef DEBUG_lskelPSG2
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = RbtPopMin(RBT);
#ifdef DEBUG_lskelPSG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	C[x] = 1;
      }
    } while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == curprio));

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
    curprio = RbtMinLevel(RBT);
    if (curprio >= val) break;
    do
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (mctopo3d_simple26(F, x, rs, ps, N))
      {
	RlifoPush(&RLIFO, x);
	C[x] = 1;
      }
    } while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++)
    {
      x = RLIFO->Pts[i];
      if (P_simple26(F, C, x, rs, ps, N))
      {
	Set(x, PSIMPLE); // marque le point pour effacement ulterieur
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
Squelette symétrique guidé ultime basé sur les noyaux critiques (cruciaux)

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
Carte topologique par squelettisation symétrique guidée ultime basée sur les noyaux critiques (cruciaux)

Attention : l'objet ne doit pas toucher le bord de l'image

EXPERIMENTAL - Ne pas utiliser dans des applications
*/
{ 
  if (depth(image) == 1)
    return lskelCKG2map(imageprio, image);

  fprintf(stderr, "lskelCKGmap: 3D not yet implemented\n");
  return(0);
    //    return lskelCKG3map(imageprio, image);
} // lskelCKGmap()

/* ==================================== */
static void extract_vois(uint8_t *img, int32_t p, int32_t rs, int32_t N, uint8_t *vois)
/*
  retourne dans "vois" les valeurs des 8 voisins de p, dans l'ordre suivant:

		3	2	1
		4	p	0
		5	6	7
  le point p ne doit pas être un point de bord de l'image (test effectué)
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
static void rotate90_vois(uint8_t *vois)
/*
   effectue une rotation du voisinage "vois" de 90 degres dans le sens
   trigonométrique
*/
/* ==================================== */
{
  uint8_t tmp;
  tmp = vois[0]; vois[0] = vois[6]; vois[6] = vois[4]; vois[4] = vois[2]; vois[2] = tmp;
  tmp = vois[7]; vois[7] = vois[5]; vois[5] = vois[3]; vois[3] = vois[1]; vois[1] = tmp;
} /* rotate90_vois() */

// Etiquetage des points objet
// Tout point objet a une étiquette >= OBJ
// Tout point candidat (donc simple) a une étiquette >= CAN
#define OBJ 1
#define CAN 2
#define CR2 3
#define CR1 4
#define CR0 5

/* ==================================== */
static void CrucialPass1( /* pour un objet en 8-connexite */
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  index_t N)       /* taille image */
/*
  Repère et marque CR1 les cliques 1-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir préalablement été étiquetés CAN.
  Il est supposé que X[p] == CAN (pas de test).
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
static void CrucialPass2( /* pour un objet en 8-connexite */
  uint8_t *X,      /* pointeur base image */
  index_t p,       /* index du point */
  index_t rs,      /* taille rangee */
  index_t N)       /* taille image */
/*
  Repère et marque CR0 les cliques 0-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir préalablement été étiquetés CAN.
  Il est supposé que X[p] == CAN (pas de test).
  Les cliques 1-cruciales doivent avoir préalablement été étiquetées CR1 (voir CrucialPass1).
*/
/* ==================================== */
{
  int32_t i;
  uint8_t v[8];

  extract_vois(X, p, rs, N, v);

/*
x b a  on doit avoir le point 'a' ou les 2 points 'b' dans X (ou les deux), 
x C b  et tous les points 'a,b' de X marqués CAN,
x x x  pour que le point central soit étiqueté CR0
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
} // CrucialPass2()


#define NONCRUCIAL 1

/* ==================================== */
int32_t lskelCKG2(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
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
    curprio = RbtMinLevel(RBT);
//#define DEBUG_lskelCKG2
#ifdef DEBUG_lskelCKG2
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass1(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass2(F, x, rs, N); }
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
    curprio = RbtMinLevel(RBT);
    if (curprio > incrprio) incrprio = curprio;
//#define DEBUG_lskelCKG2
#ifdef DEBUG_lskelCKG2
    printf("entering loop, curprio = %g, incrprio = %g\n", curprio, incrprio);
#endif
    do
    {
      x = RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG2
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && simple8(F, x, rs, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == curprio));

    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass1(F, x, rs, N); }
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; if (F[x] != CAN) F[x] = OBJ; }    
    for (i = 0; i < RLIFO->Sp; i++) { x = RLIFO->Pts[i]; CrucialPass2(F, x, rs, N); }
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


#ifdef _3D_A_FINIR
int32_t lskelCKG3(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
{}
#else
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

  le point p ne doit pas être un point de bord de l'image
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
  le point p ne doit pas être un point de bord de l'image
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
static void isometrieXZ_vois(uint8_t *vois) 
// effectue une isométrie du voisinage "vois" par échange des axes X et Z (+ symétries)
// cette isométrie est de plus une involution
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
// effectue une isométrie du voisinage "vois" par échange des axes Y et Z (+ symétries)  
// cette isométrie est de plus une involution
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
Teste si les conditions suivantes sont réunies:
1: v[8] et v[26] doivent être marqués CAN
2: for i = 0 to 7 do w[i] = v[i] || v[i+9] ; w[0...7] doit être non 2D-simple
Si le test réussit, les points 8, 26 sont marqués CR2
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
// Teste si les trois conditions suivantes sont réunies:
// 1: (P1 et P4) ou (P2 et P3)
// 2: tous les points Pi non nuls doivent être simples et non marqués CR2
// 3: A et B sont tous nuls ou [au moins un A non nul et au moins un B non nul]
// Si le test réussit, les points Pi non nuls sont marqués CR1
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
  if ((v[2]  && (v[2] != CAN)) ||
      (v[1]  && (v[1] != CAN)) ||
      (v[0]  && (v[0] != CAN)) ||
      (v[26]  && (v[26] != CAN))) goto next2;
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

Teste si les conditions suivantes sont réunies:
1: au moins un des ensembles {12,26}, {11,4}, {13,2}, {8,3} est inclus dans l'objet, et
2: les points non nuls sont tous simples, non marqués 2M_CRUCIAL et non marqués 1M_CRUCIAL
Si le test réussit, les points non nuls sont marqués 0M_CRUCIAL
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
  Repère et marque CR2 les cliques 2-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir préalablement été étiquetés CAN.
  Il est supposé que X[p] == CAN (pas de test).
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
  Repère et marque CR1 les cliques 1-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir préalablement été étiquetés CAN.
  Il est supposé que X[p] == CAN (pas de test).
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
  Repère et marque CR0 les cliques 0-cruciales pour <X,C> contenant le point p.
  Les points simples candidats de l'image X doivent avoir préalablement été étiquetés CAN.
  Il est supposé que X[p] == CAN (pas de test).
*/
/* ==================================== */
{
  uint8_t v[27];
  extract_vois3d(X, p, rs, ps, N, v);
  (void)match_vois0(v);
  insert_vois3d(v, X, p, rs, ps, N);
} // CrucialPass3d0()

/* ==================================== */
int32_t lskelCKG3(struct xvimage *image,
		  struct xvimage *imageprio, 
		  double val)
/* ==================================== */
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
    curprio = RbtMinLevel(RBT);
//#define DEBUG_lskelCKG3
#ifdef DEBUG_lskelCKG3
      printf("entering loop, curprio: %g\n", curprio);
#endif
    if (curprio >= val) break;
    do
    {
      x = RbtPopMin(RBT);
#ifdef DEBUG_lskelCKG3
      printf("pop: %d\n", x);
#endif
      UnSet(x, EN_RBT);
      if (F[x] && mctopo3d_simple26(F, x, rs, ps, N))
      {
	RlifoPush(&RLIFO, x);
	F[x] = CAN;
      }
    } while (!mcrbt_RbtVide(RBT) && (RbtMinLevel(RBT) == curprio));

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
        for (k = 0; k < 26; k += 1) // parcourt les voisins en 8-connexite
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
#endif
