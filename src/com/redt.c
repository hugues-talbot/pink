/* $Id: redt.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file redt.c

\brief reverse euclidean distance transform

<B>Usage:</B> redt in.pgm out.pgm

<B>Description:</B>
Reverse euclidean distance transform, by the Coeurjolly's algorithm [1].
The input \b in.pgm must be a quadratic euclidean distance map, or 
an image of type int32_t where each non-null value correspond to the quadratic
radius of a disc or ball. The output is a binary image.

[1] D. Coeurjolly,
"d-Dimensional Reverse Euclidean Distance Transformation
and Euclidean Medial Axis Extraction in Optimal Time", 
DGCI 2003, LNCS 2886, pp. 327-337, 2003.

<B>Types supported:</B> int32_t 2d, int32_t 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2004, Jean Cousty 2005 (3d)
*/

/*
%TEST redt %IMAGES/2dlong/l2fish1distc3.pgm %RESULTS/redt_l2fish1distc3.pgm
%TEST redt %IMAGES/2dlong/l2fish1distc3subset.pgm %RESULTS/redt_l2fish1distc3subset.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * res;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);

  if (depth(image) == 1) // 2D
  {
    if (!convertlong(&image))
    {
      fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
      exit(1);
    }
    if (! (res = lredt2d(image)))
    {
      fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
      exit(1);
    }
  }
  else // 3D
  {
    if (!convertlong(&image))
      {
	fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
	exit(1);
      }
    if (! (res = lredt3d(image)))
      {
	fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
	exit(1);
      }
    
    //   fprintf(stderr, "%s: 3d yet implemented\n", argv[0]);
    //   exit(1);
  }

  writeimage(res, argv[argc-1]);
  freeimage(res);
  freeimage(image);
  return 0;
} /* main */
