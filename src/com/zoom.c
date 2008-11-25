/* $Id: zoom.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file zoom.c

\brief zoom (shrink or expand) an image

<B>Usage:</B> zoom in.pgm <f | x rs | y cs | z ds | fx fy fz> out.pgm

<B>Description:</B> 
There are 3 modes, depending on the number of arguments.

1 argument: the same scale factor <B>f</B> is applied to both dimensions 
  x and y (and z in 3D)

2 arguments: if the parameter <B>x</B> is used, 
  followed by an integer number <B>rs</B>, the zoom factor <B>f</B> 
  is computed by dividing <B>rs</B> by the rowsize of <B>in.pgm</B>.
  If the parameter <B>y</B> is used, followed by an integer number <B>cs</B>, 
  <B>f</B> is computed by dividing <B>cs</B> by the colsize of <B>in.pgm</B>.
  If the parameter <B>z</B> is used, followed by an integer number <B>ds</B>, 
  <B>f</B> is computed by dividing <B>ds</B> by the depth of <B>in.pgm</B>.

3 arguments: different zoom factors <B>fx</B>, <B>fy</B>, <B>fz</B>
  are given for directions x, y, z.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

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
#include <lzoom.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * in;
  struct xvimage * out;
  int32_t rs, cs;
  double z, fx, fy, fz;
  int32_t newdim;

  if ((argc != 4) && (argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm <factor|x rs|y cs|fx fy fz> out.pgm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = in->row_size;
  cs = in->col_size;

  if (argc == 4) 
  { 
    z = atof(argv[2]);
    if (! lzoom(in, &out, z, z, z))
    {
      fprintf(stderr, "%s: function lzoom failed\n", argv[0]);
      exit(1);
    }
  }
  else if (argc == 5) 
  {
    newdim = atoi(argv[3]);
    if (! lzoom2(in, &out, newdim, argv[2][0]))
    {
      fprintf(stderr, "%s: function lzoom2 failed\n", argv[0]);
      exit(1);
    }
  }
  else
  { 
    fx = atof(argv[2]);
    fy = atof(argv[3]);
    fz = atof(argv[4]);
    if (! lzoom(in, &out, fx, fy, fz))
    {
      fprintf(stderr, "%s: function lzoom failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
