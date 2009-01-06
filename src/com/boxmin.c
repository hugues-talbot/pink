/* $Id: boxmin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file boxmin.c

\brief minimal box including a given set of points

<B>Usage:</B> boxmin in.pgm out.pgm

<B>Description:</B>
The result out.pgm has the same size as in.pgm. 
It contains the minimal box including the set of points in \b in.pgm 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie 2002
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lboxmin(image))
  {
    fprintf(stderr, "%s: function lboxmin failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);
  return 0;
} /* main */



