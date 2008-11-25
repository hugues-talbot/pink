/* $Id: enframe.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file enframe.c

\brief adds a border with a given gray value to an image

<B>Usage:</B> enframe in.pgm grayvalue [width] out.pgm

<B>Description:</B>
Adds a border to the input image, filled with the value
\b grayvalue. The width of the border may be given as parameter \b width, 
otherwise its value is 1.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imageout;
  int32_t grayval;
  int32_t width;  

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm grayvalue [width] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  grayval = atoi(argv[2]);
  if (argc == 5) width = atoi(argv[3]); else width = 1;
  imageout = lenframe(image, grayval, width);
  if (imageout == NULL)
  {
    fprintf(stderr, "%s: lencadre failed\n", argv[0]);
    exit(1);
  }

  writeimage(imageout, argv[argc-1]);
  freeimage(imageout);
  freeimage(image);

  return 0;
} /* main */



