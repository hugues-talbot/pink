/* $Id: line.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file line.c

\brief draws a straight line between the two first points found in image

<B>Usage:</B> line in.pgm out.pgm

<B>Description:</B>
Draws a straight line between the two first points found in image <B>in.pgm</B>.
Uses the Bresenham's algorithm.

<B>Types supported:</B> byte 2d

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "line: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if ((datatype(image1) == VFF_TYP_1_BYTE) || (depth(image1) > 1))
  {
    ldrawline2(image1);
  }
  else
  {
    fprintf(stderr, "line: bad data type or dimension\n", argv[0]);
    exit(1);
  }
  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  return 0;
} /* main */


