/* $Id: ppm2pgm.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file ppm2pgm.c

\brief converts a color ppm image into 3 grayscale pgm images

<B>Usage:</B> ppm2pgm in.ppm r.pgm g.pgm b.pgm

<B>Description:</B>
Converts a color ppm image \b in.ppm into 3 grayscale pgm images
<B>r.pgm</B>, <B>g.pgm</B> and <B>b.pgm</B> which are the red, 
green and blue color planes of the original color image.

<B>Types supported:</B> byte color 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/* explose une image en couleurs pour donner trois images en ndg */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.ppm g1.pgm g2.pgm g3.pgm\n", argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &g1, &g2, &g3) == 0)
  {
    fprintf(stderr, "rgb2bw: readrgbimage failed\n");
    exit(1);
  }
  
  writeimage(g1, argv[2]);
  writeimage(g2, argv[3]);
  writeimage(g3, argv[4]);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
