/* $Id: byte2float.c,v 1.3 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file byte2float.c

\brief converts a "byte" image to a "float" image

<B>Usage:</B> byte2float in out

<B>Description:</B> For each pixel x, out[x] = (float)in[x]

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
  struct xvimage * imagefloat;
  struct xvimage * imagebyte;
  float *L;
  uint8_t *B;
  int32_t x;

  int32_t rs, cs, d, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  imagebyte = readimage(argv[1]); 
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagebyte) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint8_t\n", argv[0]);
    exit(1);
  }

  rs = rowsize(imagebyte);
  cs = colsize(imagebyte);
  d = depth(imagebyte);
  N = rs * cs * d;
  B = UCHARDATA(imagebyte);
  
  imagefloat = allocimage(NULL, rs, cs, d, VFF_TYP_FLOAT);
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  L = FLOATDATA(imagefloat);
  imagefloat->xdim = imagebyte->xdim;
  imagefloat->ydim = imagebyte->ydim;
  imagefloat->zdim = imagebyte->zdim;

  for (x = 0; x < N; x++)
    L[x] = (float)B[x];

  writeimage(imagefloat, argv[2]);
  freeimage(imagefloat);
  freeimage(imagebyte);

  return 0;
} /* main */

