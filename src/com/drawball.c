/* $Id: drawball.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */

/*! \file drawball.c

\brief generates a binary euclidean ball

<B>Usage:</B> drawball in.pgm radius xc yc zc out.pgm

<B>Description:</B>
Draws a binary euclidean ball in the image \b in.pgm.
The parameter <B>radius</B> specifies the radius of the ball. 
The parameters <B>xc, yc, zc</B> specify the center of the ball. 

<B>Types supported:</B> byte 3d

<B>Category:</B> draw
\ingroup  draw

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
#include <mccodimage.h>
#include <mcimage.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  double xc, yc, zc;
  double radius;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm radius xc yx zc out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  radius = atof(argv[2]);
  
  xc = atof(argv[3]);
  yc = atof(argv[4]);
  zc = atof(argv[5]);

  ldrawball(image, radius, xc, yc, zc);

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

