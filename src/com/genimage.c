/* $Id: genimage.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file genimage.c

\brief generates a monocolor image of given size

<B>Usage:</B> genimage <in.pgm|rs cs ds> col out.pgm

<B>Description:</B>
The size of the result image <B>out.pgm</B> is taken from image
<B>in.pgm</B>, or given by the parameters <B>rs</B>, <B>cs</B>, <B>ds</B>. 
For a 2D image, the parameter <B>ds</B> (depth size) is set to 1.
The color of the result image is given by the parameter <B>col</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> arith, geo
\ingroup  arith, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - mai 1998 
   revision decembre 1999 : 3D
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * header;
  struct xvimage * image;
  int32_t i;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  uint8_t col;
  double xdim, ydim, zdim;

  if ((argc != 4) && (argc != 6))
  {
    fprintf(stderr, "usage: %s <in.pgm|rowsize colsize depth> color out.pgm \n", argv[0]);
    exit(1);
  }

  if (argc == 6)
  {
    rs = atoi(argv[1]);
    cs = atoi(argv[2]);
    ds = atoi(argv[3]);
    col = (uint8_t)atoi(argv[4]);
    xdim = ydim = zdim = 1.0;
  }
  else
  {
    header = readheader(argv[1]);
    rs = rowsize(header);
    cs = colsize(header);
    ds = depth(header);
    col = (uint8_t)atoi(argv[2]);
    xdim = header->xdim;
    ydim = header->ydim;
    zdim = header->zdim;
  }
  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  N = rs * cs * ds;
  memset(Im, col, N);
  image->xdim = xdim;
  image->ydim = ydim;
  image->zdim = zdim;

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


