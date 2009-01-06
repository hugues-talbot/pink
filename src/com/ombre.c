/* $Id: ombre.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ombre.c

\brief binary shadow of a grayscale function

<B>Usage:</B> ombre in.pgm out.pgm

<B>Description:</B>
The shadow of a 1D grayscale image is a 2D binary image.
The shadow of a 2D grayscale image is a 3D binary image.

<B>Types supported:</B> byte 1d, byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * ombre;
  int32_t rs, cs, ds, N, i, j, k;
  uint8_t *I;
  uint8_t *O;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: 3D images not allowed\n", argv[0]);
    exit(0);
  }
  N = rs * cs;

  if (cs == 1) { cs = 256; ds = 1; }
  else ds = 256;
  ombre = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (ombre == NULL)
  {   fprintf(stderr,"%s : allocimage failed\n", argv[0]);
      exit(0);
  }
  I = UCHARDATA(image);
  O = UCHARDATA(ombre);
  memset(O, NDG_MIN, rs*cs*ds);

  if (ds == 1)
    for (i = 0; i < rs; i++)
      for (j = 0; j <= I[i]; j++)
        O[(cs-j-1)*rs + i] = NDG_MAX;
  else
    for (j = 0; j < cs; j++)
      for (i = 0; i < rs; i++)
        for (k = 0; k < I[j*rs + i]; k++)
          O[(ds-k-1)*N + j*rs + i] = NDG_MAX;
    
  writeimage(ombre, argv[argc-1]);
  freeimage(image);
  freeimage(ombre);
  return 0;
} /* main */
