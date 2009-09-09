/* $Id: openingfunction.c,v 1.1 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file openingfunction.c

\brief opening function

<B>Usage:</B> openingfunction in.pgm [dist] out.pgm

<B>Description:</B>
This operator associates, to each point x of the input object X, 
the radius of the biggest ball included in X that includes x.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance (truncated)
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2009

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  int32_t mode;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s f.pgm [dist] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4) mode = atoi(argv[2]); else mode = 0;
  if ((mode != 0) && (mode != 2) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  result = lopeningfunction(image, mode);
  if (result == NULL)
  {
    fprintf(stderr, "%s: function lopeningfunction failed\n", argv[0]);
    exit(1);
  }

  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */
