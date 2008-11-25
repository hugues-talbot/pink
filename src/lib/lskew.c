/* $Id: lskew.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* transformation non-lineaire de l'echelle de gris d' une image */
/* Michel Couprie - juillet 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <math.h>
#include <lskew.h>

/* ==================================== */
int32_t lskew(
  struct xvimage * image,
  double skew)
/* ==================================== */
{
  double tmp;
  uint8_t t[256];
  int32_t i, N;
  uint8_t *F;

  N = rowsize(image) * colsize(image);
  F = UCHARDATA(image);

  for (i = 0; i < 256; i++) 
  {
    tmp = 256 * pow(((double)i)/256, skew);
    tmp = min(tmp,255);
    t[i] = (uint8_t)arrondi(tmp); 
  }
  
  for (i = 0; i < N; i++) F[i] = t[F[i]];
  return 1;
} /* lskew() */
