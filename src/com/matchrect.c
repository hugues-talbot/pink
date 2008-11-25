/* $Id: matchrect.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file matchrect.c

\brief matches (rounded) rectangles to the contours of objects

<B>Usage:</B> matchrect in.pgm connex [rounded|axis] out.pgm

<B>Description:</B>
Matches (rounded) rectangles to the contours of objects.
Objects are connected components according to the connectivity
given by \b connex .
Option \b rounded is used to match rounded rectangles.
Option \b axis is used to output only the big axis of the matched rectangle.

<B>Types supported:</B> byte 2D

<B>Category:</B>  geo
\ingroup  geo

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeo.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t connex;
  int32_t mode = 0;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm connex [rounded|axis] out.pgm \n", argv[0]);
    exit(1);
  }
  if (argc == 5)
  {
    if (strcmp(argv[3], "rounded") == 0)
      mode = 1;
    else if (strcmp(argv[3], "axis") == 0)
      mode = 2;
    else
    {
      fprintf(stderr, "usage: %s in.pgm connex [rounded|axis] out.pgm \n", argv[0]);
      exit(1);
    }
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);

  if (! lmatchrect(image1, connex, mode))
  {
    fprintf(stderr, "%s: function lmatchrect failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


