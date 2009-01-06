/* $Id: integermedialaxis.c,v 1.1 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file integermedialaxis.c

\brief integer medial axis transform

<B>Usage:</B> integermedialaxis in.pgm gamma out.pgm

<B>Description:</B>
Integer medial axis of the binary image \b X, as defined in [HR05]. 

[HR05] "Euclidean Skeletons of 3D Data Sets in Linear Time 
by the Integer Medial Axis Transform", 
W.H. Hesselink and B.T.M. Roerdink,
Computational Imaging and Vision, Vol. 30, 
Mathematical Morphology: 40 Years On, Springer, 2005, pp. 259-268

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>
#include "lvoronoilabelling.h"

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * res;
  double gamma;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm gamma fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  gamma = atof(argv[2]);
  if (gamma < 1)
  {
    fprintf(stderr, "%s: gamma must be greater than 1\n", argv[0]);
    exit(1);
  }
  
  res = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  if (res == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  
  if (!lmedax_Hesselink(image, res, gamma))
  {
    fprintf(stderr, "%s: lmedialaxis failed\n", argv[0]);
    exit(1);
  }

  writeimage(res, argv[argc - 1]);
  freeimage(image);
  freeimage(res);

  return 0;
} /* main */
