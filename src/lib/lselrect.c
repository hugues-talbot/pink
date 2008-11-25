/* $Id: lselrect.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* selection d'une sous-image de forme rectangulaire - les autres pixels sont mis a NDG_MIN */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>

/* ==================================== */
int32_t lselrect(
  struct xvimage * image,
  int32_t x,
  int32_t y,
  int32_t w,
  int32_t h)
/* ==================================== */
{
  int32_t xx, yy;
  uint8_t *pt;
  int32_t rs, cs, N;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lhtkern: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  pt = UCHARDATA(image);

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  for (yy = 0; yy < cs; yy++)
    for (xx = 0; xx < rs; xx++)
      if ((yy < y) || (yy > y + h) || (xx < x) || (xx > x + w))
        pt[yy * rs + xx] = NDG_MIN;
  return 1;
}
