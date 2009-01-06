/* $Id: */
/*! \file deframe.c

\brief suppress the frame from an image

<B>Usage:</B> deframe in.pgm out.pgm

<B>Description:</B>
suppress the frame from an image.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - juillet 2007 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcrop.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * out;
  int32_t rs, cs, ds, ps, n;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm \n", argv[0]);
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
  ds = depth(in);
  ps = rs * cs;
  n = ps * ds;
  
  if ((ds == 1)) /* 2D case */
  {
    out = lcrop(in, 1, 1, rs-2, cs-2);
  }
  else  /* 3D case */
  {
    out = lcrop3d(in, 1, 1, 1, rs-2, cs-2, ds-2);
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
