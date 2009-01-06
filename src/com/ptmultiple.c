/* $Id: ptmultiple.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ptmultiple.c

\brief detects multiple points in 2d or 3d binary images

<B>Usage:</B> ptmultiple in.pgm connex out.pgm

<B>Description:</B>
Detects multiple points in the 2d or 3d binary image \b in.pgm .
A multiple point is a white point x such that Tn[x] > 2
(n = 4 in 2D or n = 6 in 3D, as set by the parameter \b connex).
For the 8-connectivity in 2D, a multiple point is chacacterized by 
a transition number > 2.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2002
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseltopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lptmultiple(image, connex))
  {
    fprintf(stderr, "%s: function lptmultiple failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
