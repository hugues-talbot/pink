/* $Id: barycentrelab.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file barycentrelab.c

\brief compute the center of gravity of each labeled region

<B>Usage:</B> barycentrelab in.pgm out.pgm

<B>Description:</B>
Compute the center of gravity of each labeled region.
Labels should be consecutive integers. 
Only strictly positive labels are considered. 

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* Calcule les isobarycentres des regions etiquetees d'une image. */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lbarycentre.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imagebyte;
  uint32_t *L;
  uint8_t *B;
  uint32_t x;
  int32_t rs, cs, d, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "barycentre: readimage failed\n");
    exit(1);
  }

  if (! lbarycentrelab(image))
  {
    fprintf(stderr, "%s: function lbarycentrelab failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;
  L = ULONGDATA(image);
  
  imagebyte = allocimage(image->name, rs, cs, d, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = UCHARDATA(imagebyte);

  for (x = 0; x < N; x++) B[x] = (uint8_t)min(L[x],255);

  writeimage(imagebyte, argv[argc-1]);
  freeimage(image);
  freeimage(imagebyte);

  return 0;
} /* main */
