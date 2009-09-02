/* $Id: pow.c,v 1.1 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file pow.c

\brief raises the value of each pixel to the pth power

<B>Usage:</B> pow in.pgm p out.pgm

<B>Description:</B>
For each pixel x, out[x] = in[x]^p. If the image is a byte image,
and if out[x] exceeds 255, then out[x] is set to 255.

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
  double p;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm p out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  p = atof(argv[2]);

  if (! lpow(image1, p))
  {
    fprintf(stderr, "%s: function lpow failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */
