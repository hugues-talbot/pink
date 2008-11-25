/* $Id: byte2float.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
    fprintf(stderr, "byte2float: readimage failed\n");
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
    fprintf(stderr, "byte2float: allocimage failed\n");
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

