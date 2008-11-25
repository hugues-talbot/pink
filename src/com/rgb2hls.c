/* $Id: rgb2hls.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file rgb2hls.c

\brief converts a ppm color file to hls representation

<B>Usage:</B> in.ppm [mode] h.pgm l.pgm s.pgm

<B>Description:</B>
Constructs 3 pgm files from 1 ppm file:
\li <B>h.pgm</B> : Hue (int32_t - 0 to 359)
\li <B>h.pgm</B> : Luminance (byte)
\li <B>h.pgm</B> : Saturation (byte)

Different modes are available [default mode is 0]:
\li mode = 0: classical HLS coding, based on [1].
\li mode = 1: L1 norm (NYI) (see [2])
\li mode = 2: L1 norm with gamma correction (NYI) (see [2])

[1] Foley, Van Damm & al: "Computer Graphics", 2nd ed., p. 595

[2] J. Angulo and J. Serra. "Traitements des images de couleur 
en représentation luminance/saturation/teinte par norme L_1" (in French). 
Traitement du Signal, Vol. 21, No. 6, pp 583-604, December 2004.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/*
** Michel Couprie  -  avril 1999
**
** Quelques valeurs de HUE:
**     0  rouge
**    60  jaune
**   120  vert
**   180  cyan
**   240  bleu
**   300  magenta
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <mccolors.h>

int main(int argc, char **argv)
{
  struct xvimage * R;
  struct xvimage * G;
  struct xvimage * B;
  struct xvimage * H;
  struct xvimage * L;
  struct xvimage * S;
  uint8_t *r, *g, *b;
  uint32_t *h;
  uint8_t *l, *s;
  int32_t rs, cs, N;
  int32_t x;
  double th, tl, ts;
  int32_t mode;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.ppm [mode] h.pgm l.pgm s.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 6) mode = atoi(argv[2]); else mode = 0;

  if (readrgbimage(argv[1], &R, &G, &B) == 0)
  {
    fprintf(stderr, "%s: readrgbimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(R);
  cs = colsize(R);
  N = rs * cs;

  H = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  L = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  S = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if ((H == NULL) || (L == NULL) || (S == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  r = UCHARDATA(R);
  g = UCHARDATA(G);
  b = UCHARDATA(B);
  h = ULONGDATA(H);
  l = UCHARDATA(L);
  s = UCHARDATA(S);

  if (mode == 0)
  {
    for (x = 0; x < N; x++) 
    {
      rgb2hls(((double)(r[x]))/255, ((double)(g[x]))/255, ((double)(b[x]))/255, &th, &tl, &ts);
      h[x] = (int32_t)th;
      l[x] = (uint8_t)(tl * 255);
      s[x] = (uint8_t)(ts * 255);
    }
  } 
  else
  {
    fprintf(stderr, "%s: mode %d not implemented\n", argv[0], mode);
    exit(1);
  } 

  writeimage(H, argv[argc-3]);
  writeimage(L, argv[argc-2]);
  writeimage(S, argv[argc-1]);
  freeimage(R);
  freeimage(G);
  freeimage(B);
  freeimage(H);
  freeimage(L);
  freeimage(S);
  return 0;
}
