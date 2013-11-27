/* $Id: lresize.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* zoom par un facteur rationnel < 1 */
/* Michel Couprie - decembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>

#include "mccodimage.h"
#include "mcimage.h"
#include "mcutil.h"
#include "lzoom.h"

/* ==================================== */
int32_t lresize_lzoom(
  struct xvimage * in,
  struct xvimage ** out,
  double z)
/* ==================================== */
{
  int32_t x, y, x1, y1, xn, yn, xx, yy;
  uint8_t *ptin;
  uint8_t *ptout;
  int32_t rs, cs;
  int32_t rs2, cs2;
  double k, tmp, d, dx1, dxn, dy1, dyn, sigmad;

  if (depth(in) != 1) 
  {
    fprintf(stderr, "lzoom: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  rs = in->row_size;
  cs = in->col_size;
  ptin = UCHARDATA(in);

  if ((z >= 1.0) || (z <= 0.0))
  {   fprintf(stderr,"lzoom() : bad zoom factor : %g - must be < 1\n", z);
      return 0;
  }

  rs2 = (int32_t)(rs * z);
  cs2 = (int32_t)(cs * z);
  k = 1.0 / z;
  return 1;
}
