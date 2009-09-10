/* $Id: ldilateros3d.c,v 1.3 2009-01-07 12:46:35 mcouprie Exp $ */
/* operateurs de dilatation et d'erosion morphologique 3D */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <ldilateros3d.h>

/* ==================================== */
int32_t ldilatbin3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/*
   Operateur de dilatation binaire 3D par un element structurant 
   de taille quelconque

   ATTENTION: l'element structurant doit etre CONNEXE et REFLEXIF

   Michel Couprie - decembre 1999 
*/
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees de l'origine de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilatbin3d"
{
  register int32_t i, j, k, l;     /* index muet */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;         /* taille plan masque */
  int32_t Nm = psm * dsm;          /* taille masque */
  uint8_t *M = UCHARDATA(m);
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */

  if (!M[zc * psm + yc * rsm + xc]) /* l'element structurant N'est PAS reflexif */
  {
    fprintf(stderr, "%s: l'element structurant doit etre reflexif\n", F_NAME);
    return 0;
  }

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  l = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k * psm + j * rsm + i])
        {
          tab_es_x[l] = i;
          tab_es_y[l] = j;
          tab_es_z[l] = k;
          l += 1;
        }

  if (!ldilatbin3d2(f, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
  {
     fprintf(stderr,"%s() : ldilatbin3d2 failed\n", F_NAME);
     return(0);
  }

  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* ldilatbin3d() */

/* ==================================== */
int32_t ldilatbin3d2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, 
                 int32_t xc, int32_t yc, int32_t zc)
/*
   Operateur de dilatation binaire 3D par un element structurant 
   de taille quelconque
   ATTENTION: l'element structurant doit etre CONNEXE et REFLEXIF
   Michel Couprie - decembre 1999 
*/
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilatbin3d2"
{
  int32_t x, y, z, v, w;           /* index muet */
  register int32_t l, n, o;        /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  int32_t c;
  int32_t frontiere;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, N);

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    /* verifie si (x,y,z) est un point de la frontiere de l'objet */
    w = z * ps + y * rs + x;
    if (H[w])
    {
      frontiere = 0;
      for (l = 0; l <= 10; l += 2) /* parcourt les 6 voisins */
      {
        v = voisin6(w, l, rs, ps, N);
        if ((v != -1) && (!H[v])) { frontiere = 1; break; }
      } /* for l... */

      if (!frontiere) 
        F[w] = NDG_MAX;
      else
      {
        for (c = 0; c < nptb ; c += 1)
        {
          o = z + tab_es_z[c] - zc;
          n = y + tab_es_y[c] - yc;
          l = x + tab_es_x[c] - xc; 
          if ((o >= 0) && (o < ds) && 
              (n >= 0) && (n < cs) && 
              (l >= 0) && (l < rs))
	    F[o * ps + n * rs + l] = NDG_MAX;
        }
      }
    }
  }

  free(H);

  return 1;
} /* ldilatbin3d2() */

/* ==================================== */
int32_t lerosbin3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/*
   Operateur d'erosion binaire 3D par un element structurant 
   de taille quelconque

   ATTENTION: l'element structurant doit etre CONNEXE et REFLEXIF

   Michel Couprie - decembre 1999 
*/
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees de l'origine de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "lerosbin3d"
{
  register int32_t i, j, k, l;     /* index muet */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;         /* taille plan masque */
  int32_t Nm = psm * dsm;          /* taille masque */
  uint8_t *M = UCHARDATA(m);
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */

  if (!M[zc * psm + yc * rsm + xc]) /* l'element structurant N'est PAS reflexif */
  {
    fprintf(stderr, "%s: l'element structurant doit etre reflexif\n", F_NAME);
    return 0;
  }

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  l = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k * psm + j * rsm + i])
        {
          tab_es_x[l] = i;
          tab_es_y[l] = j;
          tab_es_z[l] = k;
          l += 1;
        }

  if (!lerosbin3d2(f, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
  {
     fprintf(stderr,"%s() : lerosbin3d2 failed\n", F_NAME);
     return(0);
  }

  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* lerosbin3d() */

/* ==================================== */
int32_t lerosbin3d2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, 
                 int32_t xc, int32_t yc, int32_t zc)
/*
   Operateur d'erosion binaire 3D par un element structurant 
   de taille quelconque

   ATTENTION: l'element structurant doit etre CONNEXE et REFLEXIF

   Michel Couprie - decembre 1999 
*/
/* ==================================== */
#undef F_NAME
#define F_NAME "lerosbin3d2"
{
  int32_t x, y, z, v, w;           /* index muet */
  register int32_t l, n, o;        /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  int32_t c;
  int32_t frontiere;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, N);

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    /* verifie si (x,y,z) est un point de la frontiere du fond */
    w = z * ps + y * rs + x;
    if (!H[w])
    {
      frontiere = 0;
      for (l = 0; l <= 10; l += 2) /* parcourt les 6 voisins */
      {
        v = voisin6(w, l, rs, ps, N);
        if ((v != -1) && (H[v])) { frontiere = 1; break; }
      } /* for l... */

      if (!frontiere) 
        F[w] = NDG_MIN;
      else
      {
        for (c = 0; c < nptb ; c += 1)
        {
          o = z + tab_es_z[c] - zc;
          n = y + tab_es_y[c] - yc;
          l = x + tab_es_x[c] - xc; 
          if ((o >= 0) && (o < ds) && 
              (n >= 0) && (n < cs) && 
              (l >= 0) && (l < rs))
	    F[o * ps + n * rs + l] = NDG_MIN;
        }
      }
    }
  }

  free(H);

  return 1;
} /* lerosbin3d2() */

/* ==================================== */
int32_t ldilatfast3d(struct xvimage *f, uint8_t *mask)
/* operateur de dilatation numerique 3d par un element structurant de taille inferieure a 3x3x3 */
/* Michel Couprie - juillet 1997 */
/* mask : masque du 26-voisinage representant l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilatfast3d"
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t d = depth(f);            /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                     /* image de travail */
  uint8_t sup;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
      return(0);
  }
  memcpy(H, F, N);

  for (x = 0; x < N; x++)
  {
    sup = H[x];
    for (k = 0; k < 26; k += 1)
    {
      if (mask[k])                      /* element structurant */
      {
        y = voisin26(x, k, rs, n, N);
        if ((y != -1) && (H[y] > sup)) sup = H[y];
      }
    } /* for k */
    F[x] = sup;
  }

  free(H);
  return 1;
} /* ldilatfast3d() */

/* ==================================== */
int32_t lerosfast3d(struct xvimage *f, uint8_t *mask)
/* operateur d'erosion numerique 3d par un element structurant de taille inferieure a 3x3x3 */
/* Michel Couprie - juillet 1997 */
/* mask : masque du 26-voisinage representant l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "lerosfast3d"
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t d = depth(f);            /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                     /* image de travail */
  uint8_t inf;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
      return(0);
  }
  memcpy(H, F, N);

  for (x = 0; x < N; x++)
  {
    inf = H[x];
    for (k = 0; k < 26; k += 1)
    {
      if (mask[k])                      /* element structurant */
      {
        y = voisin26(x, k, rs, n, N);
        if ((y != -1) && (H[y] < inf)) inf = H[y];
      }
    } /* for k */
    F[x] = inf;
  }

  free(H);
  return 1;
} /* lerosfast3d() */

/* ==================================== */
int32_t ldilatlong3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur de dilatation numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilatlong3d"
{
  register int32_t x, y, z;        /* index muet */
  register int32_t i, j, k, h;     /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;
  int32_t Nm = psm * dsm;
  uint8_t *M = UCHARDATA(m);
  uint32_t *F = ULONGDATA(f);
  uint32_t *H;                    /* image de travail */
  uint32_t sup;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;

  H = (uint32_t *)calloc(1,N*sizeof(int32_t));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, 4*N);

  nptb = 0;
  for (i = 0; i < Nm; i++) if (M[i]) nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  h = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k*psm + j*rsm + i])
        {
           tab_es_x[h] = rsm - 1 - i;  /* symetrique de l'e.s. */
           tab_es_y[h] = csm - 1 - j;
           tab_es_z[h] = dsm - 1 - k;
           h += 1;
        }
  xc = rsm - 1 - xc;
  yc = csm - 1 - yc;
  zc = dsm - 1 - zc;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    sup = 0;
    for (c = 0; c < nptb ; c += 1)
    {
      k = z + tab_es_z[c] - zc;
      j = y + tab_es_y[c] - yc;
      i = x + tab_es_x[c] - xc; 
      if ((k >= 0) && (k < ds) && (j >= 0) && (j < cs) && (i >= 0) && (i < rs) && 
          (H[k*ps + j*rs + i] > sup))
	sup = H[k*ps + j*rs + i];
    }
    F[z*ps + y*rs + x] = sup;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* ldilatlong3d() */

/* ==================================== */
int32_t leroslong3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur d'erosion numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "leroslong3d"
{
  register int32_t x, y, z;        /* index muet */
  register int32_t i, j, k, h;     /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;
  int32_t Nm = psm * dsm;
  uint8_t *M = UCHARDATA(m);
  uint32_t *F = ULONGDATA(f);
  uint32_t *H;                    /* image de travail */
  uint32_t inf;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;

  H = (uint32_t *)calloc(1,N*sizeof(int32_t));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, 4*N);

  nptb = 0;
  for (i = 0; i < Nm; i++) if (M[i]) nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  h = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k*psm + j*rsm + i])
        {
           tab_es_x[h] = rsm - 1 - i;  /* symetrique de l'e.s. */
           tab_es_y[h] = csm - 1 - j;
           tab_es_z[h] = dsm - 1 - k;
           h += 1;
        }
  xc = rsm - 1 - xc;
  yc = csm - 1 - yc;
  zc = dsm - 1 - zc;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    inf = UINT32_MAX;
    for (c = 0; c < nptb ; c += 1)
    {
      k = z + tab_es_z[c] - zc;
      j = y + tab_es_y[c] - yc;
      i = x + tab_es_x[c] - xc; 
      if ((k >= 0) && (k < ds) && (j >= 0) && (j < cs) && (i >= 0) && (i < rs) && 
          (H[k*ps + j*rs + i] < inf))
	inf = H[k*ps + j*rs + i];
    }
    F[z*ps + y*rs + x] = inf;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* leroslong3d() */

/* ==================================== */
int32_t ldilatbyte3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur de dilatation numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilatbyte3d"
{
  register int32_t x, y, z;        /* index muet */
  register int32_t i, j, k, h;     /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;
  int32_t Nm = psm * dsm;
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  uint8_t sup;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i++) if (M[i]) nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  h = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k*psm + j*rsm + i])
        {
           tab_es_x[h] = rsm - 1 - i;  /* symetrique de l'e.s. */
           tab_es_y[h] = csm - 1 - j;
           tab_es_z[h] = dsm - 1 - k;
           h += 1;
        }
  xc = rsm - 1 - xc;
  yc = csm - 1 - yc;
  zc = dsm - 1 - zc;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    sup = NDG_MIN;
    for (c = 0; c < nptb ; c += 1)
    {
      k = z + tab_es_z[c] - zc;
      j = y + tab_es_y[c] - yc;
      i = x + tab_es_x[c] - xc; 
      if ((k >= 0) && (k < ds) && (j >= 0) && (j < cs) && (i >= 0) && (i < rs) && 
          (H[k*ps + j*rs + i] > sup))
	sup = H[k*ps + j*rs + i];
    }
    F[z*ps + y*rs + x] = sup;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* ldilatbyte3d() */

/* ==================================== */
int32_t lerosbyte3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur d'erosion numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "lerosbyte3d"
{
  register int32_t x, y, z;        /* index muet */
  register int32_t i, j, k, h;     /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;
  int32_t Nm = psm * dsm;
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  uint8_t inf;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i++) if (M[i]) nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  h = 0;
  for (k = 0; k < dsm; k += 1)
    for (j = 0; j < csm; j += 1)
      for (i = 0; i < rsm; i += 1)
        if (M[k*psm + j*rsm + i])
        {
           tab_es_x[h] = rsm - 1 - i;  /* symetrique de l'e.s. */
           tab_es_y[h] = csm - 1 - j;
           tab_es_z[h] = dsm - 1 - k;
           h += 1;
        }
  xc = rsm - 1 - xc;
  yc = csm - 1 - yc;
  zc = dsm - 1 - zc;

  for (z = 0; z < ds; z++)
  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    inf = NDG_MAX;
    for (c = 0; c < nptb ; c += 1)
    {
      k = z + tab_es_z[c] - zc;
      j = y + tab_es_y[c] - yc;
      i = x + tab_es_x[c] - xc; 
      if ((k >= 0) && (k < ds) && (j >= 0) && (j < cs) && (i >= 0) && (i < rs) && 
          (H[k*ps + j*rs + i] < inf))
	inf = H[k*ps + j*rs + i];
    }
    F[z*ps + y*rs + x] = inf;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* lerosbyte3d() */

/* ==================================== */
int32_t ldilat3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur de dilatation numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldilat3d"
{
  if (datatype(f) == VFF_TYP_1_BYTE) 
    return ldilatbyte3d(f, m, xc, yc, zc);
  else if (datatype(f) == VFF_TYP_4_BYTE) 
    return ldilatlong3d(f, m, xc, yc, zc);
  else
  {
    fprintf(stderr, "%s: bad datatype\n", F_NAME);
    return 0;
  }
} // ldilat3d()

/* ==================================== */
int32_t leros3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/* operateur d'erosion numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - octobre 2002 */
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "leros3d"
{
  if (datatype(f) == VFF_TYP_1_BYTE) 
    return lerosbyte3d(f, m, xc, yc, zc);
  else if (datatype(f) == VFF_TYP_4_BYTE) 
    return leroslong3d(f, m, xc, yc, zc);
  else
  {
    fprintf(stderr, "%s: bad datatype\n", F_NAME);
    return 0;
  }
} // leros3d()

