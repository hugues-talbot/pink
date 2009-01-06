/* $Id: proj3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file proj3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* projette une image 3d binaire suivant une perspective cavaliere */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image3d;
  struct xvimage * image;
  int32_t i, j, k;
  uint8_t *Im3d;
  uint8_t *Im;
  int32_t rs, cs, d, n, N;
  float dx, dy;
  float x0, y0;
  float dndg, ndg;
  int32_t x, y, z, dz, nplans, nrs, ncs, NI;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in3d.pgm dx dy dz out.pgm \n", argv[0]);
    exit(1);
  }

  image3d = readimage(argv[1]);

  rs = rowsize(image3d);   /* taille ligne */
  cs = colsize(image3d);   /* taille colonne */
  d = depth(image3d);      /* nb plans */
  n = rs * cs;             /* taille plan */
  N = n * d;               /* taille image */
  Im3d = UCHARDATA(image3d);

  dx = atof(argv[2]);
  dy = atof(argv[3]);
  dz = atoi(argv[4]);

  nplans = d * dz;
  nrs = (int32_t)ceil(rs + (dx * nplans));
  ncs = (int32_t)ceil(cs + (dy * nplans));
  NI = nrs * ncs;
  dndg = (float)(7 * NDG_MAX) / (float)(8 * nplans);

  image = allocimage(NULL, nrs, ncs, 1, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "proj3d: allocimage failed\n");
    exit(1);
  }
  Im = UCHARDATA(image);
  for (i = 0; i < NI; i++) Im[i] = 0;

  ndg = (float)NDG_MAX / 8;
  for (k = d-1; k >= 0; k--)
  {
    for (z = dz-1; z >= 0; z--)
    {
      x0 = (nplans - ((k*dz)+z)) * dx;
      y0 = (nplans - ((k*dz)+z)) * dy;
      for (j = 0; j < cs; j++)
        for (i = 0; i < rs; i++)
	{
          x = (int32_t)(x0 + i);
          y = (int32_t)(y0 + j);
          if (Im3d[k * n + j * rs + i]) 
            Im[y * nrs + x] = (uint8_t)ndg;
	}
      ndg += dndg;
    } /* for (z = 0; z < dz; z++) */
  } /* for (k = d-1; k >= 0; k--) */

  writeimage(image, argv[5]);
  freeimage(image3d);
  freeimage(image);

  return 0;
} /* main */

