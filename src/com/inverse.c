/* $Id: inverse.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file inverse.c

\brief complements a byte image to 255

<B>Usage:</B> inverse in.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = 255 - in[x].

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! linverse(image))
  {
    fprintf(stderr, "%s: function linverse failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */
