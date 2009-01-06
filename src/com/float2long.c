/* $Id: float2long.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file float2long.c

\brief converts a "float" image to a "int32_t" image

<B>Usage:</B> float2long in.pgm offset factor out.pgm

<B>Description:</B>

Applies the following tranformation to each value x in \b in.pgm :

X = round(offset + factor * x)

<B>Types supported:</B> float 2d, float 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/*
   Michel Couprie - novembre 2005
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lhisto.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagefloat;
  struct xvimage * imagelong;
  float *L;
  int32_t *B;
  int32_t x, i;
  int32_t rs, cs, d, N, tmp;
  double T, offset, factor;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.pgm offset factor out.pgm \n", argv[0]);
    exit(1);
  }

  imagefloat = readimage(argv[1]);  
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagefloat) != VFF_TYP_FLOAT)
  {
    fprintf(stderr, "%s: image type must be float\n", argv[0]);
    exit(1);
  }

  offset = atof(argv[2]);
  factor = atof(argv[3]);

  printf("%g %g\n", offset, factor);

  rs = rowsize(imagefloat);
  cs = colsize(imagefloat);
  d = depth(imagefloat);
  N = rs * cs * d;
  L = FLOATDATA(imagefloat);
  
  imagelong = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = (int32_t *)(UCHARDATA(imagelong));
  imagelong->xdim = imagefloat->xdim;
  imagelong->ydim = imagefloat->ydim;
  imagelong->zdim = imagefloat->zdim;

  for (x = 0; x < N; x++)
  {
    T = offset + (factor * L[x]);
    B[x] = (int32_t)arrondi(T);
  }

  writeimage(imagelong, argv[argc-1]);
  freeimage(imagefloat);
  freeimage(imagelong);

  return 0;
} /* main */
