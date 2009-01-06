/* $Id: reconsdilat.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* 
obsolete - use geodilat

<B>reconsdilat</B> - morphological reconstruction by dilation

SYNOPSIS

<B>reconsdilat</B> g.pgm f.pgm connex out.pgm

DESCRIPTION

Reconstruction of <B>g.pgm</B> under <B>f.pgm</B>.
The structuring element is specified by the value of the parameter <B>connex</B>, 
which can be one of the following ones: vois4 (or 4), vois8 (or 8).

Types supported: byte 2D.

CLASS 

morph

*/

/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s g.pgm f.pgm connex out.pgm \n", argv[0]);
    fprintf(stderr, "       connex = [vois4|vois8|4|8]\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if (strcmp(argv[3], "vois8") == 0) connex = 8; else
  if (strcmp(argv[3], "vois4") == 0) connex = 4; else
  if (strcmp(argv[3], "8") == 0) connex = 8; else
  if (strcmp(argv[3], "4") == 0) connex = 4; else
  {
    fprintf(stderr, "%s: bad connexity - use one of the following:\n", argv[0]);
    fprintf(stderr, "vois8, vois4, 8, 4\n");
    exit(1);
  }

  if (! lreconsdilat(image1, image2, connex))
  {
    fprintf(stderr, "%s: function lreconsdilat failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[4]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
