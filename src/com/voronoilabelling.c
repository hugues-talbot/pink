/* $Id: voronoilabelling.c,v 1.1 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file voronoilabelling.c

\brief voronoi labelling (and squared Euclidean distance transform)

<B>Usage:</B> voronoilabelling in.pgm [dist.pgm] vor.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d,  byte 3d

<B>Category:</B> morpho
\ingroup  morpho

*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lvoronoilabelling.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * dist;
  struct xvimage * vor;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [dist.pgm] vor.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }


  dist = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  vor = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if ((dist == NULL) || (vor == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lvoronoilabelling(image, dist, vor))
  {
    fprintf(stderr, "%s: lvoronoilabelling failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
    writeimage(dist, argv[argc-2]);
  writeimage(vor, argv[argc-1]);
  freeimage(image);
  freeimage(dist);
  freeimage(vor);

  return 0;
} /* main */


