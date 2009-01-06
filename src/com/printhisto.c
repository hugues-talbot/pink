/* $Id: printhisto.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file printhisto.c

\brief prints the histogram of an image or a region

<B>Usage:</B> printhisto in.pgm [mask.pgm]

<B>Description:</B>
Calculates the histogram of \b im.pgm (masked by the binary image
\b mask.pgm, if given) and prints it on the screen.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> histo
\ingroup  histo

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

#define INDEX_FIRST
/*
*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  uint32_t * histo;
  int32_t i, k, s;


  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask.pgm]\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "histogram: readimage failed\n");
    exit(1);
  }

  if (argc == 3)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "histogram: readimage failed\n");
      exit(1);
    }
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    histo = (uint32_t *)calloc(1,(NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
    if (histo == NULL)
    {
      fprintf(stderr, "histogram: malloc failed\n");
      exit(1);
    }

    if (! lhisto(image, mask, histo))
    {
      fprintf(stderr, "histogram: function lhisto failed\n");
      exit(1);
    }

    k = 0;
    for (i = NDG_MIN; i <= NDG_MAX; i++) 
      if (histo[i] != 0)
      {
#ifdef INDEX_FIRST
        if (k % 10 == 0) printf("\n");
#endif
        k += 1;
#ifdef INDEX_FIRST
        printf("%3d:%4d ", i, histo[i]);
#else
        printf("%d %d\n", histo[i], i);
#endif
      }
    printf("\nNombre de niveaux differents = %d\n", k);
    free(histo);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    if (! lhistolong(image, mask, &histo, &s))
    {
      fprintf(stderr, "histogram: function lhistolong failed\n");
      exit(1);
    }

    k = 0;
    for (i = 0; i < s; i++) 
      if (histo[i] != 0)
      {
#ifdef INDEX_FIRST
        if (k % 10 == 0) printf("\n");
#endif
        k += 1;
#ifdef INDEX_FIRST
        printf("%3d:%4d ", i, histo[i]);
#else
        printf("%d %d\n", histo[i], i);
#endif
      }
    printf("\nNombre de niveaux differents = %d\n", k);
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float w, smin, smax;
    if (! lhistofloat(image, mask, &histo, &s, &w, &smin, &smax))
    {
      fprintf(stderr, "histogram: function lhistofloat failed\n");
      exit(1);
    }

    printf("bin width = %g ; min value = %g ; max value = %g\n", w, smin, smax);
    k = 0;
    for (i = 0; i < s; i++) 
      if (histo[i] != 0)
      {
#ifdef INDEX_FIRST
        if (k % 10 == 0) printf("\n");
#endif
        k += 1;
#ifdef INDEX_FIRST
        printf("%3d:%4d ", i, histo[i]);
#else
        printf("%d %d\n", histo[i], i);
#endif
      }
    printf("\nNombre de niveaux differents = %d\n", k);
  }

  freeimage(image);

  return 0;
} /* main */
