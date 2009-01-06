/* $Id: dilatbin3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file dilatbin3d.c

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

<B>dilatbin3d</B> - morphological dilation of a 3D binary image

SYNOPSIS

<B>dilatbin3d</B> in.pgm el.pgm x y z out.pgm

DESCRIPTION

The structuring element is given by the non-null values in <B>el.pgm</B>, 
its origin (wrt the point (0,0,0) of <B>el.pgm</B>) is given by the values of the parameters 
<B>x</B>, <B>y</B> and <B>z</B>.

Types supported: byte 3D.

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
#include <ldilateros3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y, z;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm x y z out.pgm \n", argv[0]);
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
  z = atoi(argv[5]);
  if (! ldilatbin3d(image, elem, x, y, z))
  {
    fprintf(stderr, "%s: function ldilatbin3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[6]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
