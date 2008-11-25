/* $Id: seuilhistoglobal.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file seuilhistoglobal.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  seuillage base sur l'histogramme
  on veut obtenir une proportion p de pixels blancs

  Michel Couprie - avril 1998
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * masque;
  uint32_t * histo;
  int32_t i, k, seuil, nbpts, n;
  int32_t rs, cs, d, N;
  uint8_t *I;
  uint8_t *M;
  double p;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm p [mask.pgm] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  p = 1.0 - atof(argv[2]);
  
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;
  I = UCHARDATA(image);

  histo = (uint32_t *)calloc((NDG_MAX - NDG_MIN + 1), sizeof(int32_t));
  if (histo == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    masque = readimage(argv[3]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    M = UCHARDATA(masque);
    nbpts = 0;
    for (i = 0; i < N; i++) if (M[i]) { histo[I[i]]++; nbpts++; }
  }
  else
  {
    for (i = 0; i < N; i++) histo[I[i]]++;
    nbpts = N;
  }
  
  nbpts = (int32_t)(p * nbpts);
  n = 0;
  for (i = NDG_MIN; i <= NDG_MAX; i++) 
  {
    n += histo[i];
    if (n >= nbpts) { seuil = i; break; }
  }

  for (i = 0; i < N; i++) if (I[i] >= seuil) I[i] = NDG_MAX; else I[i] = NDG_MIN;

  if (argc == 4) writeimage(image, argv[3]); else writeimage(image, argv[4]);
  free(histo);
  freeimage(image);
  if (argc == 5) freeimage(masque);

  return 0;
} /* main */
