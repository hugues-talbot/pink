/* $Id: projsphere.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file projsphere.c

\brief projection on a sphere

<B>Usage:</B> projsphere in.pgm out.pgm

<B>Description:</B> Projection on a sphere.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - décembre 2004 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * result;
  struct xvimage * image;
  uint8_t *I;
  uint8_t *R;
  double zd;
  int32_t x, y, z, xc, yc, r;
  int32_t rs, cs, ds, ps, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s : input image must be 2D \n", argv[0]);
    exit(1);
  }

  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s : input image type must be byte \n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);   /* taille ligne */
  cs = colsize(image);   /* taille colonne */
  I = UCHARDATA(image);

  xc = rs / 2;
  yc = cs / 2;
  r = max(xc,yc);

  ds = r+1;     /* nb plans */
  ps = rs * cs; /* taille plan */
  N = ps * ds;  /* taille image */

  result = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (result == NULL)
  {
    fprintf(stderr, "%s : allocimage failed \n", argv[0]);
    exit(1);
  }
  R = UCHARDATA(result);
  memset(R, 0, N);

  for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
      if (I[y*rs +x])
      {
	zd = sqrt(r*r - ((x-xc)*(x-xc) + (y-yc)*(y-yc)));
	for (z = 0; z <= (int32_t)zd; z++)
	  R[z*ps + y*rs +x] = NDG_MAX;
      }

  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */

