/* $Id: split.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file split.c

\brief 

<B>Usage:</B> split in.pgm mode out.pgm

<B>Description:</B>

The distance d used depends on the parameter \b mode :
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup   morpho

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldistgeo.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t mode;
  struct xvimage * image;
  struct xvimage * seed;
  struct xvimage * restmp;
  struct xvimage * labels;
  struct xvimage * result;
  uint8_t *I;
  uint8_t *S;
  int32_t *T;
  int32_t *R;
  int32_t i, x, y, z, rs, cs, ds, ps, N, nbmax, connex;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm <4|8|6|18|26> out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  ds = depth(image);
  ps = rs * cs;          /* taille d'un plan */
  N = ps * ds;           /* taille de l'image */

  seed = copyimage(image);
  if (seed == NULL)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  if ((mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "usage: %s in.pgm <4|8|6|18|26> fileout.pgm\n", argv[0]);
    exit(1);
  }

  restmp = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  labels = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  result = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
  if ((restmp == NULL) || (labels == NULL) || (result == NULL))
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  I = UCHARDATA(image);
  S = UCHARDATA(seed);
  T = ULONGDATA(restmp);
  R = ULONGDATA(result);
  memset(S, 0, N);
  for (i = 0; i < N; i++) 
    if (I[i])
    {
      S[i] = 255;
      if (! ldistgeo(seed, image, mode, restmp))
      {
	fprintf(stderr, "%s: ldistgeo failed\n", argv[0]);
	exit(1);
      }
      if (! llabelextrema(restmp, mode, LABMAX, labels, &nbmax))
      {
	fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
	exit(1);
      }
      R[i] = nbmax - 1;
      S[i] = 0;
    }

  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(seed);
  freeimage(restmp);
  freeimage(labels);
  freeimage(result);

  return 0;
} /* main */


