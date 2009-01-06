/* $Id: normalize.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file normalize.c

\brief normalization of grayscale values

<B>Usage:</B> normalize in.pgm [nmin nmax] out.pgm

<B>Description:</B>
For byte and int32_t images, grayscale values are normalized to span the range of 
[nmin...nmax].
The parameters \b nmin and \b nmax are optional.
For byte and int32_t images, the default values are \b nmin = 0 and \b nmax = 255.
For float images, the default values are \b nmin = 0 and \b nmax = 1.

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
  float nmin, nmax;

  if ((argc != 3) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [nmin nmax] out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    nmin = (float)(atof(argv[2]));
    nmax = (float)(atof(argv[3]));
  }
  else
  {
    nmin = (float)0;
    if (datatype(image1) == VFF_TYP_FLOAT)
      nmax = (float)1;
    else
      nmax = (float)255;
  }

  if (! lnormalize(image1, nmin, nmax))
  {
    fprintf(stderr, "%s: function lnormalize failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */
