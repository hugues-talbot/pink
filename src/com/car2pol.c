/* $Id: car2pol.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file car2pol.c

\brief converts a 2d image from cartesian to polar coordinates

<B>Usage:</B> in x_center y_center out 

<B>Description:</B>
Converts a 2d image from cartesian to polar coordinates.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/*
Michel Couprie  -  decembre 2002
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lgeo.h>

int main(int argc, char **argv)
{
  struct xvimage * image1;
  struct xvimage * image2;
  double x, y; 

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in x_center y_center out \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  x = atof(argv[2]);
  y = atof(argv[3]);

  image2 = lcar2pol(image1, x, y);
  if (! image2)
  {
    fprintf(stderr, "%s: function lcar2pol failed\n", argv[0]);
    exit(1);
  }

  writeimage(image2, argv[argc-1]);
  freeimage(image2);
  freeimage(image1);

  return 0;

}
