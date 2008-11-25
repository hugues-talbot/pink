/* $Id: xor.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file xor.c

\brief pixelwise xor of two images

<B>Usage:</B> xor in1.pgm in2.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = in1[x] xor in2[x]. 
Images must be of the same type and same dimensions.

<B>Types supported:</B> byte 2d, byte 3d

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

  if (! lxor(image1, image2))
  {
    fprintf(stderr, "%s: function lxor failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


