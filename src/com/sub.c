/* $Id: sub.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file sub.c

\brief substracts an image from another one

<B>Usage:</B> sub in1.pgm in2.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = in1[x] - in2[x]. 
For byte and int32_t image types, if out[x] < 0, then out[x] is set to 0.
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
    fprintf(stderr, "sub: readimage failed\n");
    exit(1);
  }

  if (! convertgen(&image1, &image2))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }
  
  if (! lsub(image1, image2))
  {
    fprintf(stderr, "sub: function lsub failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
