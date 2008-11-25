/* $Id: leros.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur d'erosion numerique par un element structurant de taille quelconque */
/* attention algo non optimal - voir l'implementation de dilatplan */
/* Michel Couprie - mars 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t leros( struct xvimage *f,
           struct xvimage *m,  
           int32_t xc,
           int32_t yc)
/* m : masque representant l'element structurant */
/* xc, yc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t i, j;                    /* index muet */
  int32_t k, l;                    /* index muet */
  register int32_t rs = rowsize(f);         /* taille ligne */
  register int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                     /* image de travail */
  int32_t inf;
  int32_t t;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "leros: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"leros() : malloc failed for H\n");
      return(0);
  }
  for (x = 0; x < N; x++) H[x] = F[x];

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    inf = NDG_MAX;
    for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
    {
      t = (int32_t)M[j * rsm + i];
      if (t)
      {
        l = y + j - yc;
        k = x + i - xc; 
        if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs) && ((int32_t)H[l * rs + k] - t < inf))
           inf = (int32_t)H[l * rs + k] - t;
      }
    } 
    F[y * rs + x] = (uint8_t)max(inf, NDG_MIN);
  }

  free(H);
  return 1;
}
