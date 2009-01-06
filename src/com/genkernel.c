/* $Id: genkernel.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file genkernel.c

\brief generates a kernel for convolution

<B>Usage:</B> genkernel rs cs out.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/

/* 
   Michel Couprie - mai 1998 
   revision decembre 1999 : 3D
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  float *F;
  int32_t rs, cs, N;
  int32_t i, j, x, y, x0, y0, xr, yr;
  double sigma, lambda, width, length, theta;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s rs cs out.pgm \n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);

  image = allocimage(NULL, rs, cs, 1, VFF_TYP_FLOAT);
  if (image == NULL)
  {
    fprintf(stderr, "genkernel: allocimage failed\n");
    exit(1);
  }
  F = FLOATDATA(image);
  N = rs * cs;

  x0 = rs / 2;
  y0 = cs / 2;

  width = rs / 40.0;
  length = rs / 5.0;
  sigma = 1.0 / (2*width*width);
  lambda = 1.0 / (2*length*length);
  theta = 0.0;

  for (j = 0; j < cs; j++)
  {
    for (i = 0; i < rs; i++)
    {
      x = i - x0;
      y = j - y0;
      xr = cos(theta) * x - sin(theta) * y;
      yr = sin(theta) * x + cos(theta) * y;
      //      F[j * rs + i] = (float)((127.0 / (2*sigma)) * exp(-lambda*yr*yr) *
      //                        (4*sigma*sigma*xr*xr - 2*sigma) * exp(-sigma*xr*xr)); 
      F[j * rs + i] = (float)( (127.0 / (2*sigma)) *
          (4*sigma*sigma*(xr*xr+yr*yr) - 2*sigma) * exp(-sigma*(xr*xr+yr*yr)) ); 
    }
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


