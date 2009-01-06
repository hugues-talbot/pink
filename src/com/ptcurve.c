/* $Id: ptcurve.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ptcurve.c

\brief detects curve points in a binary image

<B>Usage:</B> ptcurve in.pgm connex out.pgm

<B>Description:</B>
An curve point is a white point, which has exactly 2 white n-neighbours 
that are not n-neighbours to each other. In other words, Tn = 2 and 
each neighboring n-connected component is made of exactly one point
(n = 4, 8 (2d) or 6, 18, 26 (3d), as set by the parameter \b connex)

<B>Types supported:</B> byte 2d, byte 3d

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

  if (! lptcurve(image, connex))
  {
    fprintf(stderr, "%s: function lptcurve failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
