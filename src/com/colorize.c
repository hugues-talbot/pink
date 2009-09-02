/* $Id: colorize.c,v 1.3 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file colorize.c

\brief generates a color image from a grayscale image and a lookup table

<B>Usage:</B> colorize in.pgm lut.ppm out.ppm

<B>Description:</B>
Generates a color image from a grayscale image and a lookup table (see genlut.c).

<B>Types supported:</B> byte 2D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * in;
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;
  uint8_t r[256]; /* la LookUp Table */
  uint8_t g[256];
  uint8_t b[256];
  int32_t rs, cs, N;
  int32_t i;
  uint8_t v;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm lut.ppm out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);

  if (in == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  assert(datatype(in) == VFF_TYP_1_BYTE);
  rs = rowsize(in);
  cs = colsize(in);
  N = rs * cs;

  // Charge la LUT
  if (!readrgbimage(argv[2], &g1, &g2, &g3))
  {
    fprintf(stderr, "%s: readrgbimage failed\n", argv[0]);
    exit(1);
  }
  assert(rowsize(g1) == 256);
  assert(colsize(g1) == 1);
  assert(depth(g1) == 1);
  assert(datatype(g1) == VFF_TYP_1_BYTE);
  for (i = 0; i < 256; i++)
  {  
    r[i] = (UCHARDATA(g1))[i];
    g[i] = (UCHARDATA(g2))[i];
    b[i] = (UCHARDATA(g3))[i];
  }

  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  g1 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g1 != NULL);
  g2 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g2 != NULL);
  g3 = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE); assert(g3 != NULL);

  // Applique la LUT
  for (i = 0; i < N; i++)
  {
    v = (UCHARDATA(in))[i];
    (UCHARDATA(g1))[i] = r[v];
    (UCHARDATA(g2))[i] = g[v];
    (UCHARDATA(g3))[i] = b[v];
  }
  
  writergbimage(g1, g2, g3, argv[argc-1]);
  freeimage(in);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
