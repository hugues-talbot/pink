/* $Id: ldilatfast3d.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* operateur de dilatation numerique 3d par un element structurant de taille inferieure a 3x3x3 */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <ldilatfast3d.h>

/* ==================================== */
int32_t ldilatfast3d(struct xvimage *f, uint8_t *mask)
/* mask : masque du 26-voisinage representant l'element structurant */
/* ==================================== */
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
  {   fprintf(stderr,"ldilatfast3d() : malloc failed for H\n");
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
}
