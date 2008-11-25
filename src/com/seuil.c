/* $Id: seuil.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file seuil.c

\brief simple threshold

<B>Usage:</B> seuil in.pgm n [n2] out.pgm

<B>Description:</B>
If n2 is not present, for each pixel x, out[x] = if (in[x] < n) then 0 else 255

If n2 is present, for each pixel x, out[x] = if (n <= in[x] < n2) then 255 else 0

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie 1997
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseuil.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  double seuil, seuil2;
  struct xvimage * image;
  struct xvimage * imagebin;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm seuil [seuil2] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  seuil = atof(argv[2]);
  if (argc==5)
    seuil2 = atof(argv[3]);
  else seuil2 = 0;

  if (argc == 4)
  {
    if (! lseuil(image, seuil))
      {
	fprintf(stderr, "%s: function lseuil failed\n", argv[0]);
	exit(1);
      }
  }
  else
  {
    if (! lseuil3(image, seuil, seuil2))
      {
	fprintf(stderr, "%s: function lseuil3 failed\n", argv[0]);
	exit(1);
      }
  }

  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t rs, cs, ds, N, x;
    uint8_t *F;
    uint32_t *FL;
    rs = rowsize(image); cs = colsize(image); ds = depth(image); N = rs * cs * ds; 
    imagebin = allocimage(image->name, rs, cs, ds, VFF_TYP_1_BYTE);
    if (imagebin == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(imagebin); FL = ULONGDATA(image);
    for (x = 0; x < N; x++) F[x] = (uint8_t)FL[x];
    writeimage(imagebin, argv[argc-1]);
    freeimage(imagebin);
    freeimage(image);
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    int32_t rs, cs, ds, N, x;
    uint8_t *F;
    float *FF;
    rs = rowsize(image); cs = colsize(image); ds = depth(image); N = rs * cs * ds; 
    imagebin = allocimage(image->name, rs, cs, ds, VFF_TYP_1_BYTE);
    if (imagebin == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(imagebin); FF = FLOATDATA(image);
    for (x = 0; x < N; x++) if (FF[x] == 0.0) F[x] = NDG_MIN; else F[x] = NDG_MAX;
    writeimage(imagebin, argv[argc-1]);
    freeimage(imagebin);
    freeimage(image);
  }
  else
  {
    writeimage(image, argv[argc-1]);
    freeimage(image);
  }
  return 0;
} /* main */
