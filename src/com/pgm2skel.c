/* $Id: pgm2skel.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file pgm2skel.c

\brief decomposition of a curvilinear skeleton into isolated points, end points, curves and junctions

<B>Usage:</B> pgm2skel in.pgm connex out.skel

<B>Description:</B>
The skeleton found in \b in.pgm is decomposed into isolated points, end points, curves and junctions ;
and its description is stored in \b out.skel .
The parameter \b connex sets the adjacency relation used for the object
(4, 8 (2d) or 6, 18, 26 (3d)).

\warning No verification is done to check that the input image \b in.pgm contains indeed a curvilinear skeleton.
In the contrary case, the result would be meaningless.

\warning IMPORTANT LIMITATION: different junctions in the original image must not be in direct contact with each other
(i.e., connected) otherwise they will be considered as a single junction. To prevent this to occur, one can 
increase image resolution. 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2004
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcskelcurv.h>
#include <lskelcurv.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  skel * S;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.skel\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! (S = limage2skel(image, connex)))
  {
    fprintf(stderr, "%s: function limage2skel failed\n", argv[0]);
    exit(1);
  }

  //printskel(S);

  writeskel(S, argv[argc-1]);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
