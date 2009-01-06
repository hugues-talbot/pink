/* $Id: rotatebin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file rotatebin.c

\brief rotation of an image

<B>Usage:</B> rotatebin in.pgm theta [x y] out.pgm

<B>Description:</B>
rotates the image \b in.pgm of the angle \b theta (in degrees).
Method: truncated real rotation.
If \b x and \b y are given, then the center of the rotation is the point 
(x,y) and the image size is left unchanged (hence parts of object 
may be lost). 
Otherwise, the center of the rotation is the point (0,0) and the resulting 
image size is computed such that no loss of information occur.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcgeo.h>
#include <lrotations.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  double theta, x, y;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm theta [x y] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  theta = atof(argv[2]);
  theta = (theta * M_PI) / 180;

  if (argc == 6)
  {
    x = atof(argv[3]);
    y = atof(argv[4]);
    image2 = lrotationRT(image, theta, x, y, 0);
  }
  else
    image2 = lrotationRT(image, theta, 0.0, 0.0, 1);

  if (! image2)
  {
    fprintf(stderr, "%s: function lrotationRT failed\n", argv[0]);
    exit(1);
  }
  writeimage(image2, argv[argc-1]);
  freeimage(image);
  freeimage(image2);
  return 0;
} /* main */


