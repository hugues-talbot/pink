/* $Id: pgm2vskel.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file pgm2vskel.c

\brief decomposition of a valuated curvilinear skeleton into isolated points, end points, curves and junctions

<B>Usage:</B> pgm2vskel in.pgm val.pgm connex out.skel

<B>Description:</B>
The skeleton found in \b in.pgm is decomposed into isolated points, end points, curves and junctions ;
and its description is stored in \b out.skel . Each point of the skeleton is valuated by the corresponding value
found in \b val.pgm .
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * val;
  int32_t connex;
  skel * S;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm val.pgm  connex out.skel\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  val = readimage(argv[2]);
  if ((image == NULL) || (val == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);

  if (! (S = limage2skel(image, connex)))
  {
    fprintf(stderr, "%s: function limage2skel failed\n", argv[0]);
    exit(1);
  }

  //printskel(S);

  writevskel(S, argv[argc-1], val);
  termineskel(S);
  freeimage(image);

  return 0;
} /* main */
