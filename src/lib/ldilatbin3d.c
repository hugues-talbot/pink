/* $Id: ldilatbin3d.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* operateurs de dilatation et d'erosion morphologique 3D */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mccodimage.h"

/* ==================================== */
int32_t ldilatbin3d_ldilatbin3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc)
/*
   Operateur de dilatation binaire 3D par un element structurant 
   de taille quelconque

   ATTENTION: l'element structurant doit etre CONNEXE et REFLEXIF

   Michel Couprie - decembre 1999 
*/
/* m : masque representant l'element structurant */
/* xc, yc, zc : coordonnees de l'origine de l'element structurant */
/* ==================================== */
{
  int32_t x, y, z, v, w;              /* index muet */
  register int32_t i, j, k, l, n, o;  /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;         /* taille plan masque */
  int32_t Nm = psm * dsm;          /* taille masque */
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;
  int32_t frontiere;

  if (!M[zc * psm + yc * rsm + xc]) /* l'element structurant N'est PAS reflexif */
  {
    fprintf(stderr, "ldilatbin3d: l'element structurant doit etre reflexif\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"ldilatbin3d() : malloc failed for H\n");
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"ldilatbin3d() : malloc failed for tab_es\n");
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
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* ldilatbin3d() */

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
{
  int32_t x, y, z, v, w;              /* index muet */
  register int32_t i, j, k, l, n, o;  /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t ds = depth(f);           /* nb plans */
  int32_t ps = rs * cs;            /* taille plan */
  int32_t N = ps * ds;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t dsm = depth(m);          /* nb plans masque */
  int32_t psm = rsm * csm;         /* taille plan masque */
  int32_t Nm = psm * dsm;          /* taille masque */
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  uint8_t sup;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t *tab_es_z;               /* liste des coord. z des points de l'e.s. */
  int32_t c;
  int32_t frontiere;

  if (!M[zc * psm + yc * rsm + xc]) /* l'element structurant N'est PAS reflexif */
  {
    fprintf(stderr, "lerosbin3d: l'element structurant doit etre reflexif\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"lerosbin3d() : malloc failed for H\n");
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {  
     fprintf(stderr,"lerosbin3d() : malloc failed for tab_es\n");
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
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);
  return 1;
} /* lerosbin3d() */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
