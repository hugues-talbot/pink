/* $Id: showpoint.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file showpoint.c

\brief prints the value of a given point 

<B>Usage:</B> showpoint in.pgm x y z

<B>Description:</B>
The value of the point (<B>x</B>,<B>y</B>,<B>z</B>)
is printed in the standard output stream.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> convert
\ingroup  convert

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t x, y, z, v;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm x y z\n", argv[0]);
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
  
  lshowpoint(image1, x, y, z);

  freeimage(image1);
  return 0;
} /* main */


