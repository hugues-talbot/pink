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
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcrbt.h>
#include <mctopo3d.h>
#include <mckhalimsky3d.h>
#include <mcfifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

#define EN_RBT        0
#define EN_FIFO       1
#define CONTRAINTE    2
#define PARANO
//#define DEBUG1
//#define DEBUG

//#define PRIODIR

//#define TEST_SIMPLE_PAR_COLLAPSE

/* ==================================== */
static int32_t typedir3d(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t y, n, s, e, o, a, d, sum;
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
static int32_t testabaisse4bin(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple4(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse4bin() */

/* ==================================== */
static int32_t testabaisse8bin(uint8_t *F, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple8(F, x, rs, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse8bin() */

/* ==================================== */
static int32_t testabaisse6bin(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple6(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse6bin() */

/* ==================================== */
static int32_t testabaisse18bin(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
  if (simple18(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
  return modifie;
} /* testabaisse18bin() */

/* ==================================== */
static int32_t testabaisse26bin(uint8_t *F, int32_t x, int32_t rs, int32_t ps, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;
#ifdef TEST_SIMPLE_PAR_COLLAPSE
  if (simple_26_att(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
#else
  if (simple26(F, x, rs, ps, N)) { modifie = 1; F[x] = NDG_MIN; }
#endif
  return modifie;
} /* testabaisse26bin() */

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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
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
  RbtTermine(RBT);
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  int32_t taillemaxrbt;

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
        case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
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
	      case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
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
	      case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
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
  RbtTermine(RBT);
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  init_topo3d();

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
    if (F[x] && (P[x] < val_inhibit) && bordext26(F, x, rs, ps, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      if (testabaisse6bin(F, x, rs, ps, N))      /* modifie l'image le cas echeant */
      {
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && (P[y] < val_inhibit) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  int32_t *P = NULL;  /* l'image de priorites (cas int32) */
  uint8_t *PB = NULL;  /* l'image de priorites (cas uint8) */
  float   *PF = NULL;  /* l'image de priorites (cas float) */
  double  *PD = NULL;  /* l'image de priorites (cas double) */
  Rbt * RBT;
  int32_t taillemaxrbt;

  IndicsInit(N);

  init_topo3d();

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
    if (F[x] && (!I || !I[x]) && bordext26(F, x, rs, ps, N))
    {
      switch(datatype(imageprio))
      {
        case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[x], x); break;
        case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[x], x); break;
        case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[x], x); break;
        case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[x], x); break;
      }
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
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
	      case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
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
	      case VFF_TYP_4_BYTE: RbtInsert(&RBT, P[y], y); break;
	      case VFF_TYP_1_BYTE: RbtInsert(&RBT, PB[y], y); break;
	      case VFF_TYP_FLOAT : RbtInsert(&RBT, PF[y], y); break;
	      case VFF_TYP_DOUBLE: RbtInsert(&RBT, PD[y], y); break;
	    }
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse26bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
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
C = empty image
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  Fifo * FIFO1;
  Fifo * FIFO2;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
        RbtInsert(&RBT, P[x]*10 + typedir(F, x, rs, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
#ifdef DEBUG
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
        RbtInsert(&RBT, P[x]*10 + typedir(F, x, rs, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
#ifdef DEBUG
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
    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
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
                RbtInsert(&RBT, P[y]*10 + typedir(F, y, rs, N), y);
#else
                RbtInsert(&RBT, P[y], y);
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
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else if (connex == 8)
  {
    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
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
                RbtInsert(&RBT, P[y]*10 + typedir(F, y, rs, N), y);
#else
                RbtInsert(&RBT, P[y], y);
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
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  RbtTermine(RBT);
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
C = empty image
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

  IndicsInit(N);
  init_topo3d();

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      if (F[x] && bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
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

    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
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
                RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
                RbtInsert(&RBT, P[y], y);
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
          top6(F, x, rs, ps, N, &t, &tb);
          if (t > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!RbtVide(RBT))
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
              top26(F, y, rs, ps, N, &t, &tb);
              if (t > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
              RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
              RbtInsert(&RBT, P[y], y);
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
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
on definit l'operateur Surfacic Thinning CT(F,P) : 
C = empty image
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t prio, oldprio;
  int32_t taillemaxrbt;

  IndicsInit(N);
  init_topo3d();

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      if (F[x] && bordext6(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
#endif
        Set(x, EN_RBT);
      }
    }
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
    {
      if (F[x] && bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
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

    while (!RbtVide(RBT))
    {
      prio = (int32_t)RbtMinLevel(RBT) / 10;
      oldprio = prio;

      while (!RbtVide(RBT) && (prio == oldprio)) 
      {
        x = RbtPopMin(RBT);
        FifoPush(FIFO1, x);
        if (!RbtVide(RBT)) prio = (int32_t)RbtMinLevel(RBT) / 10;
      } 

      while (!FifoVide(FIFO1))
      {
        x = FifoPop(FIFO1);
        UnSet(x,EN_RBT);

        if ((! IsSet(x, CONTRAINTE)) && testabaisse6bin(F, x, rs, ps, N))
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
                RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
                RbtInsert(&RBT, P[y], y);
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
          top6(F, x, rs, ps, N, &t, &tb);
          if (tb > 1) Set(x, CONTRAINTE);
        }
      } // while (!FifoVide(FIFO2))
    } /* while (!RbtVide(RBT)) */
    FifoTermine(FIFO1);
    FifoTermine(FIFO2);
  } /* if (connex == 6) */
  else if (connex == 26)  // NOTE : en 26 connexite pas besoin de la strategie a 2 passes (FIFO)
  {           
    while (!RbtVide(RBT))
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
              top26(F, y, rs, ps, N, &t, &tb);
              if (tb > 1) 
	      {
                Set(y, CONTRAINTE);
	      }
	    }
            if ((! IsSet(y, CONTRAINTE)) && (! IsSet(y, EN_RBT)))
	    {
#ifdef PRIODIR
              RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
              RbtInsert(&RBT, P[y], y);
#endif
              Set(y, EN_RBT);
	    }
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;
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
  else 
  {
    fprintf(stderr, "%s: datatype(imageprio) must be int32_t\n", F_NAME);
    return(0);
  }
  taillemaxrbt = 2 * rs +  2 * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse4bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */
  else
  if (connex == 8)
  {
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
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
  RbtTermine(RBT);
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t d = depth(image);
  int32_t N = d * ps;              /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;  /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;
  int32_t t, tb;

  IndicsInit(N);

  init_topo3d();

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
    return(0);
  }
  taillemaxrbt = 2 * rs * cs +  2 * rs * d +  2 * d * cs;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
    if (F[x] && !IsSet(x, CONTRAINTE) && bordext26(F, x, rs, ps, N))
    {
      RbtInsert(&RBT, P[x], x);
      Set(x, EN_RBT);
    }
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 6)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top6(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else
  if (connex == 18)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top18(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 18) */
  else
  if (connex == 26)
  {
    while (!RbtVide(RBT))
    {
      x = RbtPopMin(RBT);
      UnSet(x, EN_RBT);
      top26(F, x, rs, ps, N, &t, &tb);
      if ((tmin <= t) && (t <= tmax) && (tbmin <= tb) && (tb <= tbmax))
      {
        F[x] = 0;
        for (k = 0; k < 26; k += 1)        /* parcourt les voisins en 26-connexite */
        {                                              /* pour empiler les voisins */
          y = voisin26(x, k, rs, ps, N);                       /* non deja empiles */
          if ((y != -1) && (F[y]) && !IsSet(y, CONTRAINTE) && (! IsSet(y, EN_RBT)))
          {
            RbtInsert(&RBT, P[y], y);
            Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse6bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */
  else
  {
    fprintf(stderr, "%s: bad value for connex\n", F_NAME);
    return(0);
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  termine_topo3d();
  IndicsTermine();
  RbtTermine(RBT);
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
  int32_t x, y;                 /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t ds = depth(image);       /* nb plans */
  int32_t N = ds * ps;             /* taille image */
  struct xvimage *imageprio;
  struct xvimage *imagedist;
  double *P = NULL;     /* l'image de priorites */
  double *D = NULL;     /* l'image de distances */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I = NULL; /* l'image d'inhibition */
  Rbt *RBT;
  int32_t taillemaxrbt;
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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
	RbtInsert(&RBT, D[x], x);
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
	    RbtInsert(&RBT, prio, y);
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
      if (F[x] && (!I[x]) && bordext26(F, x, rs, ps, N))
      {
	RbtInsert(&RBT, D[x], x);
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
	    RbtInsert(&RBT, prio, y);
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

    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, D[y], y);
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
	      RbtInsert(&RBT, prio, y);
	      P[y] = prio;
	    }
	  } // if y
        } // for k
      } // else if (D[x] >= r)
    finwhile:;
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 8) */
  else
  if (connex == 26)
  {
    init_topo3d();
    while (!RbtVide(RBT))
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
            RbtInsert(&RBT, D[y], y);
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
	      RbtInsert(&RBT, prio, y);
	      P[y] = prio;
	    }
	  } // if y
        } // for k
      } // else
    finwhile26:;
    } /* while (!RbtVide(RBT)) */
    termine_topo3d();
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
  RbtTermine(RBT);
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
uint32_t encodevois(int32_t p, uint8_t *F, int32_t rs, int32_t ps, int32_t N)
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
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  int32_t *P = NULL;     /* l'image de priorites (ndg) */
  Rbt * RBT;
  int32_t taillemaxrbt;
  uint32_t config;

//#define VERIF
#ifdef VERIF
{ int32_t i, s = 1 << 27;
  for (i = 0; i < s; i++)
    if (IsEnd(i)) printf("Is End: %x\n", i);
}
#endif

  IndicsInit(N);
  init_topo3d();

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
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      if (F[x] && bordext26(F, x, rs, ps, N))
      {
#ifdef PRIODIR
        RbtInsert(&RBT, P[x]*10 + typedir3d(F, x, rs, ps, N), x);
#else
        RbtInsert(&RBT, P[x], x);
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
    while (!RbtVide(RBT))
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
	    RbtInsert(&RBT, P[y]*10 + typedir3d(F, y, rs, ps, N), y);
#else
	    RbtInsert(&RBT, P[y], y);
#endif
	    Set(y, EN_RBT);
          } /* if y */
        } /* for k */      
      } /* if (testabaisse8bin(F, x, rs, N)) */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
  return(1);
} /* lskelend3d_sav() */

/* ==================================== */
int32_t lskelend3d(struct xvimage *image, 
	       int32_t connex, 
	       uint8_t *endpoint)
/* ==================================== */
/* 
Squelette curviligne ou surfacique 3D binaire.

Le pr�dicat "endpoint" est d�fini par un tableau de 2^27 bool�ens 
(cod�s sur 2^24 octets) pass� en param�tre.
*/
#undef F_NAME
#define F_NAME "lskelend3d"
{ 
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  Rbt * RBT;
  int32_t taillemaxrbt;
  uint32_t config;

//#define VERIF
#ifdef VERIF
{ int32_t i, s = 1 << 27;
  for (i = 0; i < s; i++)
    if (IsEnd(i)) printf("Is End: %x\n", i);
}
#endif

  IndicsInit(N);
  init_topo3d();

  taillemaxrbt = 2 * rs +  2 * cs + 2 * ds;
  /* cette taille est indicative, le RBT est realloue en cas de depassement */
  RBT = CreeRbtVide(taillemaxrbt);
  if (RBT == NULL)
  {
    fprintf(stderr, "%s: CreeRbtVide failed\n", F_NAME);
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
      if (F[x] && simple6(F, x, rs, ps, N))
        RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
  }
  else if (connex == 18)
  {
    for (x = 0; x < N; x++)
      if (F[x] && simple18(F, x, rs, ps, N))
        RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
  }
  else if (connex == 26)
  {
    for (x = 0; x < N; x++)
      if (F[x] && simple26(F, x, rs, ps, N))
        RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
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
      while (!RbtVide(RBT))
      {
	x = RbtPopMin(RBT);
	config = encodevois(x, F, rs, ps, N);
	if ((!IsEnd(config)) && testabaisse6bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && simple6(F, x, rs, ps, N))
          RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 6) */
  else if (connex == 18)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!RbtVide(RBT))
      {
	x = RbtPopMin(RBT);
	config = encodevois(x, F, rs, ps, N);
	if ((!IsEnd(config)) && testabaisse18bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && simple18(F, x, rs, ps, N))
          RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 18) */
  else if (connex == 26)
  {           
    int32_t nbdel = 1; 
    int32_t nbiter = 0; 
    while (nbdel)
    {
      nbdel = 0; 
      nbiter++;
      while (!RbtVide(RBT))
      {
#ifdef DEBUG
	{ int32_t lev = RbtMinLevel(RBT);
	printf("pop: prio %d ", lev); }
#endif	
	x = RbtPopMin(RBT);
#ifdef DEBUG
	printf("; point %d (%d,%d,%d)\n", x, x % rs, (x % ps) / rs, x / ps);
#endif
	config = encodevois(x, F, rs, ps, N);
	if ((!IsEnd(config)) && testabaisse26bin(F, x, rs, ps, N)) nbdel++;
      } /* while (!RbtVide(RBT)) */
      for (x = 0; x < N; x++)
        if (F[x] && simple26(F, x, rs, ps, N))
          RbtInsert(&RBT, typedir3d(F, x, rs, ps, N), x);
      printf("nbiter : %d ; nbdel : %d\n", nbiter, nbdel);
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 26) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  termine_topo3d();
  RbtTermine(RBT);
  return(1);
} /* lskelend3d() */
