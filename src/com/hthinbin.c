/* $Id: hthinbin.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file hthinbin.c

OBSOLETE - see skeleton, skelcurv

\brief homotopic skeleton of a 2d binary image

<B>Usage:</B> hthinbin in <constr|null> connex ne ni out

<B>Description:</B>
Homotopic curvilinear thinning by iterative removal of simple,
non-end points. Breadth-first strategy. 
If given, the binary image \b constr indicates the points that must not
be removed during the thinning.
The number of iterations is set to \b ni . It \b ni = -1, 
the iterations will continue until stability.
During the first \b ne iterations (default 0), the end points
are removed as well.
If \b ne = -1, the end points are always removed.

<B>Types supported:</B> byte 2d

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie 1998
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsquelbin.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * contr = NULL;
  int32_t connex;
  int32_t niseuil;
  int32_t niter;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in <constr|null> connex ne ni out\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "hthinbin: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[3]);
  niseuil = atoi(argv[4]);
  niter = atoi(argv[5]);

  if (strcmp(argv[2],"null") == 0) 
  {
    if (! lhthinbin(image, connex, niseuil, niter))
    {
      fprintf(stderr, "hthinbin: lhthinbin failed\n");
      exit(1);
    }
  }
  else
  {
    contr = readimage(argv[2]);
    if (contr == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    if (! lhthinbincontr(image, contr, connex, niseuil, niter))
    {
      fprintf(stderr, "hthinbin: lhthinbincontr failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (contr) freeimage(contr);

  return 0;
} /* main */
