/* $Id: quasishear.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file quasishear.c

\brief quasi shear rotation

<B>Usage:</B> quasishear in.pgm theta [x y] out.pgm

<B>Description:</B>
rotates the image of the angle \b theta (in degrees).
If a point (x,y) is given, then the result is croped to the original
image size, otherwise a new image with sufficient size is created.

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
#include <mccodimage.h>
#include <mcimage.h>
#include <lrotations.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  int32_t n, i;
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
    if (!lquasishear(image, theta, x, y))
    {
      fprintf(stderr, "%s: function lquasishear failed\n", argv[0]);
      exit(1);
    }
    writeimage(image, argv[argc-1]);
    freeimage(image);
    return 0;
  }
  else
  {
    image2 = lquasishear2(image, theta);
    if (! image2)
    {
      fprintf(stderr, "%s: function lquasishear2 failed\n", argv[0]);
      exit(1);
    }
    writeimage(image2, argv[argc-1]);
    freeimage(image);
    freeimage(image2);
    return 0;
  }

} /* main */


