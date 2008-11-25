/* $Id: double2byte.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file double2byte.c

\brief converts a "double" image to a "byte" image

<B>Usage:</B> double2byte in.pgm [mode] out.pgm

<B>Description:</B>

Depending on the value given for the (optional) parameter <B>mode</B>:
\li   <B>mode</B> = 0 (default) : for all x, out[x] = min(255, arrondi(in[x])).
\li   <B>mode</B> = 1 : for all x, out[x] = arrondi(in[x]) modulo 256.
\li   <B>mode</B> = 2 : scales values in the range 0-255.
\li   <B>mode</B> = 4 : truncation of the square root in the range 0-255.
\li   <B>mode</B> = 5 : truncation of the log in the range 0-255.

<B>Types supported:</B> double 2d, double 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/*
   Michel Couprie - mai 1998

   Modif : decembre 1999 - mode 3 (trunchisto)
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lhisto.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * imagedouble;
  struct xvimage * imagebyte;
  double *L;
  uint8_t *B;
  int32_t x, i;
  int32_t mode = 0;
  double Min, Max, t;
  double T;
  int32_t rs, cs, d, N, tmp;

  if ((argc < 3) || (argc > 4))
  {
    fprintf(stderr, "usage: %s in1.pgm [mode] out.pgm \n", argv[0]);
    fprintf(stderr, "mode = 0 (trunc) | 1 (modulo) | 2 (scale) | \n");
    fprintf(stderr, "       4 (square root) | 5 (log)\n");
    exit(1);
  }

  imagedouble = readimage(argv[1]);  
  if (imagedouble == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagedouble) != VFF_TYP_DOUBLE)
  {
    fprintf(stderr, "%s: image type must be double\n", argv[0]);
    exit(1);
  }

  if (argc > 3) mode = atoi(argv[2]);

  rs = rowsize(imagedouble);
  cs = colsize(imagedouble);
  d = depth(imagedouble);
  N = rs * cs * d;
  L = DOUBLEDATA(imagedouble);
  
  imagebyte = allocimage(NULL, rs, cs, d, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = UCHARDATA(imagebyte);
  imagebyte->xdim = imagedouble->xdim;
  imagebyte->ydim = imagedouble->ydim;
  imagebyte->zdim = imagedouble->zdim;

  switch(mode)
  {
    case 0:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)min(tmp,255);
      }
      break;
    case 1:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)(tmp % 256);
      }
      break;
    case 2:
      Min = Max = L[0];
      for (x = 0; x < N; x++) 
        if (L[x] > Max) Max = L[x]; else if (L[x] < Min) Min = L[x];
      for (x = 0; x < N; x++) 
      {
        t = ((L[x]-Min) * 255.0) / (double)(Max-Min);
        tmp = arrondi(t);
        B[x] = (uint8_t)min(255,tmp);
      }
      break;
    case 4:
      for (x = 0; x < N; x++)
      {
        T = sqrt((double)(L[x]));
        tmp = arrondi(T);
        tmp = min(255,tmp);
        tmp = max(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    case 5:
      for (x = 0; x < N; x++)
      {
        T = log((double)(L[x]));
        tmp = arrondi(T);
        tmp = min(255,tmp);
        tmp = max(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    default:
      fprintf(stderr, "usage: %s in1.pgm [mode] out.pgm \n", argv[0]);
      fprintf(stderr, "mode = 0 (trunc) | 1 (modulo) | 2 (scale) | \n");
      fprintf(stderr, "       4 (square root) | 5 (log)\n");
      exit(1);
  } /* switch(mode) */

  writeimage(imagebyte, argv[argc-1]);
  freeimage(imagedouble);
  freeimage(imagebyte);

  return 0;
} /* main */
