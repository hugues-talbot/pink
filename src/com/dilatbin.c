/* $Id: dilatbin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file dilatbin.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>dilatbin</B> - morphological dilation of a binary image

SYNOPSIS

<B>dilatbin</B> in.pgm el.pgm x y out.pgm

DESCRIPTION

The structuring element is given by the non-null values in <B>el.pgm</B>, 
its origin (wrt the point (0,0) of <B>el.pgm</B>) is given by the values of the parameters 
<B>x</B> and <B>y</B>. 

Types supported: byte 2D.

CLASS 

morph

*/

/* Michel Couprie - avril 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm x y out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readimage(argv[2]);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  if (! ldilatbin(image, elem, x, y))
  {
    fprintf(stderr, "%s: function ldilatbin failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[5]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
