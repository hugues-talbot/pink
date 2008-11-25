/* $Id: warp.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file warp.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* warp */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <lwarp.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  uint8_t *pt;
  int32_t N;
  triangle ts, td;
  float xa, ya, xb, yb, xc, yc;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "warp: readimage failed\n");
    exit(1);
  }

  printf("triangle source: saisissez xa, ya, xb, yb, xc, yc : \n");
  scanf("%f %f %f %f %f %f", &xa, &ya, &xb, &yb, &xc, &yc);
  ts.xa = (double)xa;  ts.ya = (double)ya;
  ts.xb = (double)xb;  ts.yb = (double)yb;
  ts.xc = (double)xc;  ts.yc = (double)yc;
  printf("triangle destination: saisissez xa, ya, xb, yb, xc, yc : \n");
  scanf("%f %f %f %f %f %f", &xa, &ya, &xb, &yb, &xc, &yc);
  td.xa = (double)xa;  td.ya = (double)ya;
  td.xb = (double)xb;  td.yb = (double)yb;
  td.xc = (double)xc;  td.yc = (double)yc;

  if (! lwarp(image, &ts, &td, 0))
  {
    fprintf(stderr, "warp: function lwarp failed\n");
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */
