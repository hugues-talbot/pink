/* $Id: medialaxis.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file medialaxis.c

\brief medial axis transform

<B>Usage:</B> medialaxis in.pgm distance out.pgm

<B>Description:</B>
Medial axis of the binary image \b X. If x is the center of a maximal ball
included in X, then the result R(x) is equal to the smallest distance between x 
and a point outside X, otherwise it is equal to 0 .
The distance is indicated by the parameter <B>distance</B> :
\li 0: approximate euclidean distance
\li 1: exact quadratic euclidean distance (Saito-Toriwaki's skeleton)
\li 2: exact quadratic euclidean distance (Coeurjolly's reduced axis)
\li 3: exact quadratic euclidean distance (Rémy-Thiel)
\li 4: 4-distance in 2d
\li 6: 6-distance in 3d
\li 8: 8-distance in 2d
\li 26: 26-distance in 3d

The usual discrete distances are denoted by 4, 8 (2D) and 6, 26 (3D).
For the euclidean distance, set <B>distance</B> to 0.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * res;
  int32_t distance;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm distance fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  distance = atoi(argv[2]);
  
  if (! (res = lmedialaxis(image, distance)))
  {
    fprintf(stderr, "%s: lmedialaxis failed\n", argv[0]);
    exit(1);
  }

  writeimage(res, argv[argc - 1]);
  freeimage(image);
  freeimage(res);

  return 0;
} /* main */
