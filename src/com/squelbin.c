/* $Id: squelbin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file squelbin.c

\brief homotopic skeleton of a 2d binary image

<B>Usage:</B> squelbin in.pgm connex [n] out.pgm

<B>Description:</B>
Homotopic skeletonization by iterative removal of simple,
non-end points. Breadth-first strategy. 
During the first \b n iterations (default 0), the end points
are removed as well.
If \b n = -1, the end points are always removed.

<B>Types supported:</B> byte 2d

<B>Category:</B> topobin
\ingroup  topobin

\warning This operator is very sensitive to small contour irregularities. 
See skelcurv for a more robust operator.

\author Michel Couprie 1998
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsquelbin.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  int32_t niseuil;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [n] fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "squelbin: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  if (argc == 4)
    niseuil = 0;
  else
    niseuil = atoi(argv[3]);

  if (! lsquelbin(image, connex, niseuil))
  {
    fprintf(stderr, "squelbin: lsquelbin failed\n");
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */
