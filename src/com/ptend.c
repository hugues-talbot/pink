/* $Id: ptend.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file ptend.c

\brief detects end points in a binary image

<B>Usage:</B> ptend in.pgm connex out.pgm

<B>Description:</B>
An end point is a white point, which has exactly 1 white n-neighbour
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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

  if (! lptend(image, connex))
  {
    fprintf(stderr, "%s: function lptend failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
