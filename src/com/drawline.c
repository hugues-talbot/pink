/* $Id: drawline.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file drawline.c

\brief draw a line segment in a 2D image

<B>Usage:</B> drawline in.pgm x1 y1 x2 y2 out.pgm

<B>Description:</B>

Draws a bresenham line segment between (x1,y1) and (x2,y2)

<B>Types supported:</B> byte 2D

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t x1, y1, x2, y2;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 x2 y2 out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  x2 = atoi(argv[4]);
  y2 = atoi(argv[5]);

  ldrawline(image, x1, y1, x2, y2);

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

