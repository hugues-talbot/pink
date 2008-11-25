/* $Id: lambdamedialaxis.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file lambdamedialaxis.c

\brief discrete lambda-medial axis transform

<B>Usage:</B> lambdamedialaxis in.pgm [lambda] out.pgm

<B>Description:</B>
Discrete lambda-medial axis of the binary image \b X contained in \b in.pgm. 
If the parameter \b lambda is given, the output is a binary image. Otherwise, 
it is a float image representing the function x->lambda(x).

\warning Experimental.

<B>Types supported:</B> byte 2d, byte 3d

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
#include <lmedialaxis.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * distimage;
  struct xvimage * lambdaimage;
  double lambda;
  uint32_t rs, cs, ds, N, i;
  uint8_t *I;
  float *L;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [lambda] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;

  lambdaimage = allocimage(NULL, rs, cs, ds, VFF_TYP_FLOAT);
  if (lambdaimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  distimage = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if (distimage == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lsedt_meijster(image, distimage))
  {
    fprintf(stderr, "%s: lsedt_meijster failed\n", argv[0]);
    exit(1);
  }

  if (!llambdamedialaxis(distimage, lambdaimage))
  {
    fprintf(stderr, "%s: llambdamedialaxis failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    I = UCHARDATA(image);
    L = FLOATDATA(lambdaimage);
    lambda = atof(argv[2]);
    for (i = 0; i < N; i++)
      if (L[i] < lambda) 
	I[i] = NDG_MIN;
    writeimage(image, argv[argc - 1]);
  }
  else
    writeimage(lambdaimage, argv[argc - 1]);

  freeimage(lambdaimage);
  freeimage(distimage);
  freeimage(image);

  return 0;
} /* main */


