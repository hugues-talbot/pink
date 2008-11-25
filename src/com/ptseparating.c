/* $Id: ptseparating.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file ptseparating.c

\brief detects separating points in a 2D or 3D binary image

<B>Usage:</B> ptseparating in.pgm connex out.pgm

<B>Description:</B>
A separating point is a white point x such that Tb_n[x] >= 2
(n = 4,8 in 2D or n = 6,18,26 in 3D, as set by the parameter \b connex)

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

  if (! lptseparating(image, connex))
  {
    fprintf(stderr, "%s: function lptseparating failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


