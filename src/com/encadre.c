/* $Id: encadre.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file encadre.c

OBSOLETE - voir enframe.c

\brief adds a border with a given gray value to an image

<B>Usage:</B> encadre in.pgm grayvalue out.pgm

<B>Description:</B>
The result <B>out.pgm</B> is larger than <B>in.pgm</B> 
since two rows and two columns 
(and two planes for 3D images) are added. 
Its border is set to <B>grayvalue</B>, 
all other pixels are copied from <B>in.pgm</B>.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
*/

/* 
  Michel Couprie - aout 2000
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

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm grayvalue out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  grayval = atoi(argv[2]);
  imageout = lencadre(image, grayval);
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



