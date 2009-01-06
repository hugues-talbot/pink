/* $Id: autocrop.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file autocrop.c

\brief eliminates null regions at the periphery of an image

<B>Usage:</B> autocrop in.pgm out.pgm

<B>Description:</B>
Selects the minimum rectangle (cuboid in 3d) that contain all non-null information 
of the image \b in.pgm .

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * temp1;
  int32_t i, j;
  uint8_t *I;
  uint8_t *T1;
  int32_t x, y, z, w, h, p;
  int32_t xmin, xmax, ymin, ymax, zmin, zmax;
  int32_t rs, cs, n, N, d;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm\n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  rs = rowsize(in);
  cs = colsize(in);
  N = rs * cs;
  d = depth(in);
  I = UCHARDATA(in);
  xmin = rs; xmax = 0; 
  ymin = cs; ymax = 0; 
  zmin = d; zmax = 0; 
  for (z = 0; z < d; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (I[z * N + y * rs + x])
        {
          if (z < zmin) zmin = z; if (z > zmax) zmax = z; 
          if (y < ymin) ymin = y; if (y > ymax) ymax = y; 
          if (x < xmin) xmin = x; if (x > xmax) xmax = x; 
        }

  w = xmax - xmin + 1;
  h = ymax - ymin + 1;
  p = zmax - zmin + 1;

#ifdef VERBOSE
  printf("Crop: xmin=%d, ymin=%d, zmin=%d, w=%d, h=%d, p=%d\n",
	 xmin, ymin, zmin, w, h, p);
#endif

  temp1 = allocimage(NULL, w, h, p, VFF_TYP_1_BYTE);
  if (temp1 == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
 
  T1 = UCHARDATA(temp1);
  n = w * h;
  for (z = 0; z < p; z++)
    for (y = 0; y < h; y++)
      for (x = 0; x < w; x++)
        T1[z*n + y*w + x] = I[((zmin+z)*N) + ((ymin+y)*rs) + xmin+x];

  writeimage(temp1, argv[2]);
  freeimage(in);
  freeimage(temp1);

  return 0;
} /* main */
