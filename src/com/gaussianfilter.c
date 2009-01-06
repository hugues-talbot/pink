/* $Id: gaussianfilter.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file gaussianfilter.c

\brief Gaussian filter operator

<B>Usage:</B> gaussianfilter in.pgm alpha out.pgm

<B>Description:</B>
Deriche's recursive implementation of the Gaussian filter
operator (convolution with a Gaussian kernel). The parameter 
\b alpha (double) controls the spatial extension of the
filter: 0 < alpha < infinity, typical value: alpha = 1

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lderiche.h>
#include <lderiche3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  double alpha;
  double dummy;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm alpha out.pgm \n", 
            argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  alpha = atof(argv[2]);

  if (depth(image1) == 1)
  {
    if (! lderiche(image1, alpha, 4, dummy))
    {
      fprintf(stderr, "%s: function lderiche failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lderiche3d(image1, alpha, 4, dummy))
    {
      fprintf(stderr, "%s: function lderiche3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


