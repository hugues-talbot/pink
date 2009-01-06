/* $Id: amont.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file amont.c

\brief amont of a binary marker in a grayscale image

<B>Usage:</B> amont in.pgm mark.pgm connex [s] out.pgm

<B>Description:</B>
Let F be the function defined by \b in.pgm, 
let X be the set defined by \b mark.pgm.
The amont of X in F is defined as the set of points x such that there exists
an ascending path from X to x for F with connectivity <B>connex</B>.
If the parameter <B>s</B> is given, then only strictly ascending paths are 
considered.

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>
#include <lwshedtopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marker;
  int32_t connex;
  int32_t strict = 0;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [s] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && strcmp(argv[4],"s"))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }
  if (argc == 6) strict = 1;

  image = readimage(argv[1]);
  marker = readimage(argv[2]);
  if ((image == NULL) || (marker == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  
  if (! lamont(marker, image, connex, strict))
  {
    fprintf(stderr, "%s: lamont failed\n", argv[0]);
    exit(1);
  }

  writeimage(marker, argv[argc - 1]);
  freeimage(image);
  freeimage(marker);

  return 0;
} /* main */



