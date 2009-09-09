/* $Id: crestheight.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file crestheight.c

\brief measures the height of crests in a thinned grayscale image

<B>Usage:</B> crestheight skel.pgm connex out.pgm

<B>Description:</B>
Let F be the input image (parameter <B>skel.pgm</B>), and
let R be the output image (parameter <B>out.pgm</B>).
For each point p which is separating for F (connexity
given by the parameter <B>connex</B>), the output value R(p) is
equal to calpha8m(F,S,p).

References:<BR> 
"Topological operators for the detection of
curvilinar structures in grayscale images", in preparation, 2003.

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>

#define NORMALIZE

/* ==================================== */
int32_t lcrestheight(
  struct xvimage *skel, 
  int32_t connex)
/* ==================================== */
#undef F_NAME
#define F_NAME "lcrestheight"
{ 
  int32_t p;
  int32_t rs = rowsize(skel);     /* taille ligne */
  int32_t cs = colsize(skel);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *S = UCHARDATA(skel);      /* l'image de squelette */
  struct xvimage *sp;
  uint8_t *SP;
  struct xvimage *r;
  uint8_t *R;
  uint8_t vmax = 0;

  if (depth(skel) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  sp = copyimage(skel);
  r = copyimage(skel);
  if ((sp == NULL) || (r == NULL))
  {
    fprintf(stderr, "%s: copyimage failed\n", F_NAME);
    return(0);
  }
  SP = UCHARDATA(sp);
  R = UCHARDATA(r);

  for (p = 0; p < N; p++) if (separant4(S, p, rs, N)) SP[p] = 255; else SP[p] = 0;

  for (p = 0; p < N; p++)
  {
    if (SP[p])
    {
      R[p] = S[p] - calpha8m(S, SP, p, rs, N);
#ifdef NORMALIZE
      if (R[p] > vmax) vmax = R[p];
#endif
    }
    else
      R[p] = 0;
  }

#ifdef NORMALIZE
  for (p = 0; p < N; p++)
    R[p] = (255 * R[p]) / vmax;
#endif

  memcpy(S, R, N);

  freeimage(sp);
  freeimage(r);
  return(1);
} /* lcrestheight() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * skel;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s skel.pgm connex out.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  skel = readimage(argv[1]);
  if (skel == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lcrestheight(skel, connex))
  {
    fprintf(stderr, "%s: lcrestheight failed\n", argv[0]);
    exit(1);
  }

  writeimage(skel, argv[argc-1]);
  freeimage(skel);

  return 0;
} /* main */
