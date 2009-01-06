/* $Id: point.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file point.c

\brief modifies the value of a given point 

<B>Usage:</B> point in.pgm x y z v out.pgm

<B>Description:</B>
The value of the point (<B>x</B>,<B>y</B>,<B>z</B>)
is changed to <B>v</B>.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lpoint.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t x, y, z, v;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x y z v out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  x = atoi(argv[2]);
  y = atoi(argv[3]);
  z = atoi(argv[4]);
  v = atoi(argv[5]);
  
  if (! lpoint(image1, x, y, z, v))
  {
    fprintf(stderr, "%s: function lpoint failed\n", argv[0]);
    exit(1);
  }
  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  return 0;
} /* main */


