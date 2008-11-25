/* $Id: scale.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file scale.c

\brief product of an image by a scalar

<B>Usage:</B> scale in.pgm const out.pgm

<B>Description:</B>
For each pixel x, out[x] = in[x] * const. If the image is a byte image,
and if out[x] exceeds 255, then out[x] is set to 255.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  double scale;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm scale out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "scale: readimage failed\n");
    exit(1);
  }
  scale = atof(argv[2]);

  if (! lscale(image1, scale))
  {
    fprintf(stderr, "scale: function lscale failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */
