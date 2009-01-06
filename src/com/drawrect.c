/* $Id: drawrect.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file drawrect.c

\brief Draws a rectangle with sides parallel to the main axes

<B>Usage:</B> drawrect in.pgm x1 y1 z1 x2 y2 z2 out.pgm

<B>Description:</B>
Draws a rectangle (a box in 3D) with sides parallel to the main axes. 
Two diagonally opposed corners are specified by 
<B>x1</B>, <B>y1</B>, <B>z1</B> and <B>x2</B>, <B>y2</B>, <B>z2</B>.
The rectangle is surimposed to the contents of <B>in.pgm</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> draw
\ingroup  draw

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
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  char name[128];
  int32_t x, y, z, x1, y1, z1, x2, y2, z2;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 z1 x2 y2 z2 out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  rs = image->row_size;
  cs = image->col_size;
  ps = rs * cs;
  ds = depth(image);
  N = ds * ps;

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  z1 = atoi(argv[4]);

  x2 = atoi(argv[5]);
  y2 = atoi(argv[6]);
  z2 = atoi(argv[7]);

  if ((x1 < 0) || (x1 >= rs) || (x2 < 0) || (x2 >= rs) ||
      (y1 < 0) || (y1 >= cs) || (y2 < 0) || (y2 >= cs) ||
      (z1 < 0) || (z1 >= ds) || (z2 < 0) || (z2 >= ds)
      )
  {
    fprintf(stderr, "%s: bad coordinates\n", argv[0]);
    exit(1);
  }

  if (x1 > x2) {x = x1; x1 = x2; x2 = x;} 
  if (y1 > y2) {y = y1; y1 = y2; y2 = y;} 
  if (z1 > z2) {z = z1; z1 = z2; z2 = z;} 
  for (z = z1; z <= z2; z++)
    for (y = y1; y <= y2; y++)
      for (x = x1; x <= x2; x++)
	Im[z * ps + y * rs + x] = NDG_MAX;

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

