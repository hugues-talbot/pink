/* $Id: directionalfilter.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file directionalfilter.c

\brief directional filter for curvilinear feature extraction

<B>Usage:</B> directionalfilter.c in.pgm width length ndir out.pgm

<B>Description:</B>
Let F be the original image from \b in.pgm .
This operator computes the supremum of the convolutions of F
by a series of kernels K0, ... Kn where n = \b ndir - 1, which are defined
by, for each (x,y) and each i in [0...n]: 

\verbatim
sigma = 1 / (2*width*width);
lambda = 1 / (2*length*length);
theta = i * PI / n;
xr = cos(theta) * x - sin(theta) * y;
yr = sin(theta) * x + cos(theta) * y;
Ki(x,y) = exp(-lambda*yr*yr) *
          (4*sigma*sigma*xr*xr - 2*sigma) * 
          exp(-sigma*xr*xr) 
\endverbatim

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie 2003
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvol.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  double width, length;  
  int32_t ndir;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm width length ndir out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  width = atof(argv[2]);
  length = atof(argv[3]);
  ndir = atoi(argv[4]);

  if (! convertfloat(&image))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }
  
  if (! ldirectionalfilter(image, width, length, ndir))
  {
    fprintf(stderr, "%s: function ldirectionalfilter failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
} /* main */

