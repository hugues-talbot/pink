/* $Id: ball.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */

OBSOLETE - VOIR DRAWBALL

/* \file ball.c

\brief generates a binary euclidean ball

<B>Usage:</B> ball radius x_size y_size z_size out.pgm

<B>Description:</B>
Generates a binary euclidean ball.
The parameter <B>radius</B> specifies the radius of the ball. 
The parameters <B>x_size</B>, <B>y_size</B> and <B>z_size</B>
indicate the dimensions of the voxel in the "real" world.

<B>Types supported:</B> byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1999 */

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i, j, k, xc, yc, zc;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  char name[512];
  double x_size, y_size, z_size, x, y, z;
  double radius, r2;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s radius x_size y_size z_size out.pgm \n", argv[0]);
    exit(1);
  }

  radius = atof(argv[1]);
  x_size = atof(argv[2]);
  y_size = atof(argv[3]);
  z_size = atof(argv[4]);
  xc = (int32_t)ceil(radius / x_size);
  yc = (int32_t)ceil(radius / y_size);
  zc = (int32_t)ceil(radius / z_size);
  rs = 2 * xc + 1;
  cs = 2 * yc + 1;
  ds = 2 * zc + 1;
  ps = rs * cs;
  N = ps * ds;
  sprintf(name, "ball(%lf,%lf,%lf,%lf)", radius, x_size, y_size, z_size);
  image = allocimage(name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "ball: allocimage failed\n");
    exit(1);
  }
  Im = UCHARDATA(image);
  memset(Im, NDG_MIN, N);

  r2 = radius * radius;
  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = abs(i-xc) * x_size;
    y = abs(j-yc) * y_size;
    z = abs(k-zc) * z_size;
    if (x * x + y * y + z * z <= r2)
      Im[k * ps + j * rs + i] = NDG_MAX;
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

