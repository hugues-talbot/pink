/* $Id: genlut.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file genlut.c

\brief generates a lookup table

<B>Usage:</B> genlut ncol minhue maxhue out.lut

<B>Description:</B> Generates a lookup table with a continuous color gradient.

A lookup table is represented by a special ppm image with 1 row. 

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mccolors.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * lut_r;
  struct xvimage * lut_g;
  struct xvimage * lut_b;
  int32_t i, ncol;
  double minhue, maxhue, minlum, maxlum, minsat, maxsat;
  double r, g, b, hue, lum, sat;
  uint8_t *R, *G, *B;
  int32_t rint, gint, bint;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s ncol minhue maxhue minlum maxlum minsat maxsat  out.lut\n", argv[0]);
    exit(1);
  }

  ncol = atoi(argv[1]);
  minhue = atof(argv[2]);
  maxhue = atof(argv[3]);
  minlum = atof(argv[4]);
  maxlum = atof(argv[5]);
  minsat = atof(argv[6]);
  maxsat = atof(argv[7]);
  lut_r = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  lut_g = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  lut_b = allocimage(NULL, ncol, 1, 1, VFF_TYP_1_BYTE);
  if ((lut_r == NULL) || (lut_g == NULL) || (lut_b == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  R = UCHARDATA(lut_r);
  G = UCHARDATA(lut_g);
  B = UCHARDATA(lut_b);

  for (i = 0; i < ncol; i++)
  {
    hue = (double)(minhue + ((maxhue-minhue)*i) / (double)ncol);
    while (hue < 0.0) hue += 360.0;
    while (hue >= 360.0) hue -= 360.0;
    lum = (double)(minlum + ((maxlum-minlum)*i) / (double)ncol);
    sat = (double)(minsat + ((maxsat-minsat)*i) / (double)ncol);
    hls2rgb(&r, &g, &b, hue, lum, sat);
    rint = (int32_t)(r*256);
    gint = (int32_t)(g*256);
    bint = (int32_t)(b*256);
    if (rint>255) R[i]=255; else R[i]=(uint8_t)rint;
    if (gint>255) G[i]=255; else G[i]=(uint8_t)gint;
    if (bint>255) B[i]=255; else B[i]=(uint8_t)bint;
  }

  writergbimage(lut_r, lut_g, lut_b, argv[argc-1]);
  freeimage(lut_r);
  freeimage(lut_g);
  freeimage(lut_b);

  return 0;
} /* main */
