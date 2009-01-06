/* $Id: ptseparatinggray.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ptseparatinggray.c

\brief detects separating points in a grayscale image

<B>Usage:</B> ptseparatinggray in.pgm connex out.pgm

<B>Description:</B>
A separating point is a point p, which is separating (in the binary sense)
for a section h of F, with h <= F(p).

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

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

  if (! lptseparatinggray(image, connex))
  {
    fprintf(stderr, "%s: function lptseparatinggray failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


