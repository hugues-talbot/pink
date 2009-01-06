/* $Id: equal.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file equal.c

\brief equality test

<B>Usage:</B> equal in1.pgm in2.pgm

<B>Description:</B>
This function returns 1 (no error) if for each pixel x, in1[x] == in2[x]; 
otherwise it returns 0.
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
  int32_t ret;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm \n", argv[0]);
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
  
  ret = lequal(image1, image2);

  freeimage(image1);
  freeimage(image2);

  return ret;
} /* main */
