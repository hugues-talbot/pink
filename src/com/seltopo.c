/* $Id: seltopo.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file seltopo.c

\brief selects points according to a local topological criterion 

<B>Usage:</B> seltopo in connex t- t+ tb- tb+ out

<B>Description:</B>
Let X be the set of points of the binary image \b in, let x be a point.
Let t(x) and tb(x) be the two connectivity numbers for x.
The result \b out is the set of points:
{ x in X, t- <= t(x) <= t+ and tb- <= tb(x) <= tb+ }.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
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
  int32_t connex, tm, tp, tbm, tbp;

  if (argc != 8)
  {
    fprintf(stderr, "usage: %s in connex t- t+ tb- tb+ out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  tm = atoi(argv[3]);
  tp = atoi(argv[4]);
  tbm = atoi(argv[5]);
  tbp = atoi(argv[6]);

  if (! lseltopo(image, connex, tm, tp, tbm, tbp))
  {
    fprintf(stderr, "%s: function lseltopo failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
