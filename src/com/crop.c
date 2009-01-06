/* $Id: crop.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file crop.c

\brief extracts a rectangular (cuboidal in 3d) area from an image

<B>Usage:</B> crop in.pgm x y [z] w h [d] out.pgm

<B>Description:</B>
For a 2d image, extracts the rectangle with upper left corner (<B>x</B>,<B>y</B>),
of width <B>w</B> and height <B>h</B> from the image in.pgm .
For a 3d image, extracts the cuboid with upper left corner (<B>x</B>,<B>y</B>, <B>z</B>),
of width <B>w</B>, height <B>h</B> and depth <B>d</B> from the image in.pgm .

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d.

<B>Category:</B> convert, geo
\ingroup  convert, geo

\author Michel Couprie
*/

/* 
   Michel Couprie - juillet 1996 

   update aout 2000 : int32_t et 3d
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
  int32_t i, j, k;
  uint8_t *I;
  uint8_t *T1;
  uint32_t *IL;
  uint32_t *T1L;
  int32_t x, y, z, w, h, d;
  int32_t rs, n, p;

  if ((argc != 7) && (argc != 9))
  {
    fprintf(stderr, "usage: %s in.pgm x y [z] w h [d] out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if ((argc == 7)) /* 2D case */
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    w = atoi(argv[4]);
    h = atoi(argv[5]);

    out = lcrop(in, x, y, w, h);
  }
  else  /* 3D case */
  {
    x = atoi(argv[2]);
    y = atoi(argv[3]);
    z = atoi(argv[4]);
    w = atoi(argv[5]);
    h = atoi(argv[6]);
    d = atoi(argv[7]);
    out = lcrop3d(in, x, y, z, w, h, d);
  }

  writeimage(out, argv[argc-1]);
  freeimage(in);
  freeimage(out);

  return 0;
} /* main */
