/* $Id: ldilatfast.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur de dilatation numerique par un element structurant de taille inferieure a 3x3 */
/* UNIQUEMENT POUR DES ELEMENTS STRUCTURANTS REFLEXIFS */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t ldilatfast(struct xvimage *f, uint8_t *mask) 
/* mask : masque du 8-voisinage representant l'element structurant */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                     /* image de travail */
  uint8_t sup;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "ldilatfast: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"ldilatfast() : malloc failed for H\n");
      return(0);
  }
  for (x = 0; x < N; x++) H[x] = F[x];

  for (x = 0; x < N; x++)
  {
    sup = H[x];                         /* l'ES est reflexif */
    for (k = 0; k < 8; k += 1)
    {
      if (mask[k])                      /* element structurant */
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (H[y] > sup)) sup = H[y];
      }
    } /* for k */
    F[x] = sup;
  }

  free(H);
  return 1;
}
