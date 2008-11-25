/* $Id: section.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file section.c

\brief computes a 2d cross section from a 3d image

<B>Usage:</B> section in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm

<B>Description:</B> 
The points P0 ( \b x0, \b y0, \b z0 ), P1 ( \b x1, \b y1, \b z1 ) and
P2 ( \b x2, \b y2, \b z2 ) form a basis for a plane in the 3d space. 
The intersection of this plane with the domain of the 3d image \b in.pgm
form the domain of the 2d image \b out.pgm .
The grayscale values of \b out.pgm are obtained by trilinear interpolation from
the grayscale values of \b in.pgm .
The coordinates of the basis for the plane are expressed in "real world" dimensions.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeo.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * in;
  struct xvimage * out;
  double x0, y0, z0, x1, y1, z1, x2, y2, z2;

  if (argc != 12)
  {
    fprintf(stderr, "usage: %s in.pgm x0 y0 z0 x1 y1 z1 x2 y2 z2 out.pgm\n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  x0 = atof(argv[2]);
  y0 = atof(argv[3]);
  z0 = atof(argv[4]);
  x1 = atof(argv[5]);
  y1 = atof(argv[6]);
  z1 = atof(argv[7]);
  x2 = atof(argv[8]);
  y2 = atof(argv[9]);
  z2 = atof(argv[10]);

  out = lsection(in, x0, y0, z0, x1, y1, z1, x2, y2, z2);
  if (out == NULL)
  {
    fprintf(stderr, "%s: lsection failed\n", argv[0]);
    exit(1);
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
