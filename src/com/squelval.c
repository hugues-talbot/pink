/* $Id: squelval.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file squelval.c

\brief valuated skeleton of a binary image

<B>Usage:</B> squelval in.pgm dist.pgm connex [inhibit] out.pgm

<B>Description:</B>
Valuated skeleton (ref. CB03) of a binary image.
Requires a precomputed distance map \b dist.pgm .
If the parameter \b inhibit (an integer) is given and is different from -1,
then the points which correspond to this distance value will be left unchanged. 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topo
\ingroup  topo

\warning EXPERIMENTAL! DO NOT USE
\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsquelval.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * dist;
  struct xvimage * niv;
  struct xvimage * gra;
  int32_t connex;
  int32_t inhibit;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm dist.pgm connex [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  dist = readimage(argv[2]);
  if (dist == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if (argc == 6) inhibit = atoi(argv[4]); else inhibit = -1;

  if (depth(image) == 1)
  {
    if (! lsquelval(image, dist, connex, inhibit))
/*
    niv = allocimage(NULL, rowsize(image) * colsize(image), 1, 1, VFF_TYP_1_BYTE);
    gra = readimage("_g");
    if (! lsquelsmoothval(image, dist, niv, gra, connex, inhibit, 5))
*/
    {
      fprintf(stderr, "squelval: lsquelval failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lsquelval3d(image, dist, connex, inhibit))
    {
      fprintf(stderr, "squelval: lsquelval3d failed\n");
      exit(1);
    }
  }

  writeimage(dist, argv[argc-1]);
  freeimage(image);
  freeimage(dist);

  return 0;
} /* main */
