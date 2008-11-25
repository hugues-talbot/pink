/* $Id: eros.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file eros.c

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

<B>eros</B> - morphological erosion by a plane structuring element

SYNOPSIS

<B>eros</B> in.pgm el.pgm x y out.pgm

DESCRIPTION

The (plane) structuring element is given by the non-null values in <B>el.pgm</B>, 
its origin (wrt the point (0,0) of <B>el.pgm</B>) is given by the values of the parameters 
<B>x</B> and <B>y</B>. 

Types supported: byte 2D.

CLASS 

morph

*/

/* Michel Couprie - mars 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
  if (! leros(image, elem, x, y))
  {
    fprintf(stderr, "%s: function leros failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[5]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
