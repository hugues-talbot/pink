/* $Id: long2float.c,v 1.1 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file long2float.c

\brief converts a "long" image to a "float" image

<B>Usage:</B> long2float in out

<B>Description:</B> For each pixel x, out[x] = (float)in[x]

<B>Types supported:</B> long 2d, long 3d.

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
  struct xvimage * imagelong;
  float *L;
  uint32_t *B;
  int32_t x;

  int32_t rs, cs, d, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  imagelong = readimage(argv[1]); 
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagelong) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint32_t\n", argv[0]);
    exit(1);
  }

  rs = rowsize(imagelong);
  cs = colsize(imagelong);
  d = depth(imagelong);
  N = rs * cs * d;
  B = ULONGDATA(imagelong);
  
  imagefloat = allocimage(NULL, rs, cs, d, VFF_TYP_FLOAT);
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  L = FLOATDATA(imagefloat);
  imagefloat->xdim = imagelong->xdim;
  imagefloat->ydim = imagelong->ydim;
  imagefloat->zdim = imagelong->zdim;

  for (x = 0; x < N; x++)
    L[x] = (float)B[x];

  writeimage(imagefloat, argv[2]);
  freeimage(imagefloat);
  freeimage(imagelong);

  return 0;
} /* main */

