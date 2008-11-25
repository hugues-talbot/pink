/* $Id: insert.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*!\file insert.c

\brief inserts an image into another image

<B>Usage:</B> insert a.pgm b.pgm x y z out.ppm

<B>Description:</B> Inserts image \b a.pgm into image \b b.pgm at position \b x,y,z

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup convert, geo

\author Michel Couprie
*/
/* 
   Michel Couprie - octobre 1997 
   Revision decembre 1999 (3D)
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
  struct xvimage * a;
  struct xvimage * b;
  int32_t x, y, z;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s a.pgm b.pgm x y z out.ppm \n", argv[0]);
    exit(1);
  }

  a = readimage(argv[1]);
  if (a == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  b = readimage(argv[2]);
  if (b == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  z = atoi(argv[5]);

  if (! convertgen(&a, &b))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }

  if (!linsert(a, b, x, y, z))
  {
    fprintf(stderr, "%s: linsert failed\n", argv[0]);
    exit(1);
  }
 
  writeimage(b, argv[argc-1]);
  freeimage(a);
  freeimage(b);

  return 0;
} /* main */
