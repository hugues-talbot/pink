/* $Id: isnull.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file isnull.c

\brief nullity test

<B>Usage:</B> isnull in.pgm

<B>Description:</B>
This function returns 1 (no error) if for each pixel x, in[x] == 0; 
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
  int32_t ret;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s in.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  ret = lnull(image1);

  freeimage(image1);

  return ret;
} /* main */
