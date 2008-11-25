/* $Id: lstretch.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* etire l'echelle de gris d' une image */
/* Michel Couprie - juillet 1996 */

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <mccodimage.h>

/* ==================================== */
int32_t lstretch(struct xvimage * image)
/* ==================================== */
{
  int32_t x;
  uint8_t *Im;
  int32_t N;
  uint8_t ndgmin, ndgmax;

  N = rowsize(image) * colsize(image) * depth(image);
  Im = UCHARDATA(image);

  ndgmin = NDG_MAX;
  ndgmax = NDG_MIN;
  for (x = 0; x < N; x++)
  {
    if (Im[x] < ndgmin) ndgmin = Im[x];
    if (Im[x] > ndgmax) ndgmax = Im[x];
  }

  ndgmax = ndgmax - ndgmin;

  for (x = 0; x < N; x++)
  {
    Im[x] = ((Im[x] - ndgmin) * NDG_MAX) / ndgmax;
  }
  return 1;
}

