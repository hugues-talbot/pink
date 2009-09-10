/* $Id: byte2long.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file byte2long.c

\brief converts a "byte" image to a "int32_t" image

<B>Usage:</B> byte2long in out

<B>Description:</B> For each pixel x, out[x] = (int32_t)in[x]

<B>Types supported:</B> byte 2d, byte 3d.

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - mai 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagelong;
  struct xvimage * imagebyte;
  int32_t *L;
  uint8_t *B;
  uint32_t x;

  int32_t rs, cs, d, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  imagebyte = readimage(argv[1]); 
  if (imagebyte == NULL)
  {
    fprintf(stderr, "byte2long: readimage failed\n");
    exit(1);
  }

  rs = rowsize(imagebyte);
  cs = colsize(imagebyte);
  d = depth(imagebyte);
  N = rs * cs * d;
  B = UCHARDATA(imagebyte);
  
  imagelong = allocimage(imagebyte->name, rs, cs, d, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "byte2long: allocimage failed\n");
    exit(1);
  }
  L = SLONGDATA(imagelong);
  imagelong->xdim = imagebyte->xdim;
  imagelong->ydim = imagebyte->ydim;
  imagelong->zdim = imagebyte->zdim;

  for (x = 0; x < N; x++)
    L[x] = (int32_t)B[x];

  writeimage(imagelong, argv[2]);
  freeimage(imagelong);
  freeimage(imagebyte);

  return 0;
} /* main */

