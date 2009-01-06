/* $Id: average.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file average.c

\brief pixelwise (weighted) average of two images

<B>Usage:</B> average in1.pgm in2.pgm [alpha] out.pgm

<B>Description:</B>
The default value for the optional parameter \b alpha is 0.5 . 
For each pixel x, out[x] = alpha * in1[x] + (1 - alpha) * in2[x]. 
If both images are byte images,
and if out[x] exceeds 255, then out[x] is set to 255.
Images must be of the same dimensions.

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
  double alpha;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm [alpha] out.pgm \n", argv[0]);
    exit(1);
  }

  if (argc == 4)
    alpha = 0.5;
  else
    alpha = atof(argv[3]);
  if ((alpha < 0.0) || (alpha > 1.0))
  {
    fprintf(stderr, "%s: parameter alpha must be between 0 and 1\n", argv[0]);
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

  if (! laverage(image1, image2, alpha))
  {
    fprintf(stderr, "%s: function laverage failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


