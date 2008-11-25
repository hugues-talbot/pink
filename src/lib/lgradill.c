/* $Id: lgradill.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* introduit un gradient d'illumination */
/* valeurs normales en haut - multipliees par un facteur alpha <= 1 en bas */
/* Michel Couprie - fevrier 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>

/* ==================================== */
int32_t lgradill(
  struct xvimage * image1,
  double alpha)
/* ==================================== */
{
  int32_t i;
  uint8_t *pt1;
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  pt1 = UCHARDATA(image1);
  for (i = 0; i < N; i++)
  {
    pt1[i] = (uint8_t)(pt1[i] * (1.0 - (alpha * (i/rs) / (double)(N/rs))));
  }
  return 1;
}





