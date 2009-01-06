/* $Id: mult.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file mult.c

\brief pixelwise multiplication of two images

<B>Usage:</B> mult in1.pgm in2.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = in1[x] * in2[x]. If both images are byte images,
and if out[x] exceeds 255, then out[x] is set to 255.
Images must be of the same type and same dimensions.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }

  if (! lmult(image1, image2))
  {
    fprintf(stderr, "%s: function lmult failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


