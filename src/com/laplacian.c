/* $Id: laplacian.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file laplacian.c

\brief Laplacian operator

<B>Usage:</B> laplacian in.pgm alpha out.pgm

<B>Description:</B>
Deriche's recursive implementation of the Laplacian
operator. The parameter 
\b alpha (double) controls the spatial extension of the
filter: 0 < alpha < infinity, typical value: alpha = 1

<B>Types supported:</B> byte 2D

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
    fprintf(stderr, "laplacian: readimage failed\n");
    exit(1);
  }
  alpha = atof(argv[2]);

  if (! lderiche(image1, alpha, 2, dummy))
  {
    fprintf(stderr, "laplacian: function lderiche failed\n");
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


