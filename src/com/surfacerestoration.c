/* $Id: surfacerestoration.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file surfacerestoration.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t nitermax;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm connex niter fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 6) && (connex != 26))
  {
    fprintf(stderr, "%s : connexmin = <6|26>\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "surfacerestoration: readimage failed\n");
    exit(1);
  }

  if (! lsurfacerestoration(image, nitermax, connex))
  {
    fprintf(stderr, "surfacerestoration: lsurfacerestoration failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
