/* $Id: printstats.c,v 1.4 2009-01-07 12:46:35 mcouprie Exp $ */
/*! \file printstats.c

\brief prints some stats of an image or a region

<B>Usage:</B> printstats in.pgm [mask.pgm]

<B>Description:</B>
Calculates the histogram of \b im.pgm (masked by the binary image
\b mask.pgm, if given) and prints it on the screen.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> histo
\ingroup  histo

\author Laurent Najman
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>

#define INDEX_FIRST
/*
*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
#undef F_NAME
#define F_NAME "printstats"
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  int32_t rs;     /* taille ligne */
  int32_t cs;     /* taille colonne */
  int32_t d;        /* nombre plans */
  int32_t N;         /* taille image */
  int32_t nbPoints=0;
  int32_t min, max;
  double mean;
  int32_t i;
  uint8_t *M;
  uint8_t *SOURCEc;      /* l'image de depart */
  uint32_t *SOURCEi;      /* l'image de depart */
  float *SOURCEf;      /* l'image de depart */
  int32_t pixel;


  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask.pgm]\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", F_NAME);
    exit(1);
  }
  rs = rowsize(image);     /* taille ligne */
  cs = colsize(image);     /* taille colonne */
  d = depth(image);        /* nombre plans */
  N = rs * cs * d;         /* taille image */

  switch (datatype(image)) {
  case VFF_TYP_1_BYTE:   SOURCEc = UCHARDATA(image); break;
  case VFF_TYP_4_BYTE:   SOURCEi = ULONGDATA(image); break;
  case VFF_TYP_FLOAT:    SOURCEf = FLOATDATA(image); break;
  default:
    fprintf(stderr, "%s: cette version ne traite que les images BYTE, LONG et FLOAT\n", F_NAME);
    exit(1);
  }

  M=NULL;
  if (argc == 3)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", F_NAME);
      exit(1);
    }
    M = UCHARDATA(mask);
  }

  if (datatype(image) != VFF_TYP_FLOAT) {
    min = INT32_MAX;
    max = INT32_MIN;
    mean = 0.;
    if (M == NULL) {
      for (i=0; i<N; i++) {
	switch (datatype(image)) {
	case VFF_TYP_1_BYTE:   pixel = SOURCEc[i]; break;
	case VFF_TYP_4_BYTE:   pixel = SOURCEi[i]; break;
	}
	
	if (min > pixel) min = pixel;
	if (max < pixel) max = pixel;
      mean += (double)pixel;
      nbPoints++;
      }
    } else {
      for (i=0; i<N; i++) {
	switch (datatype(image)) {
	case VFF_TYP_1_BYTE:   pixel = SOURCEc[i]; break;
	case VFF_TYP_4_BYTE:   pixel = SOURCEi[i]; break;
	}
	if (M[i]) {
	  if (min > pixel) min = pixel;
	  if (max < pixel) max = pixel;
	  mean += (double)pixel;
	  nbPoints++;
	}
      }
    }
    printf("Maximum : %d\n", max);
    printf("Minimum : %d\n", min);
    printf("Mean    : %lg\n", mean/(double)nbPoints);
  } else {
    // VFF_TYP_FLOAT
    float minf = SOURCEf[0];
    float maxf = SOURCEf[0];
    float meanf = 0;
    float pixelf;
    if (M == NULL) {
      for (i=0; i<N; i++) {
	pixelf = SOURCEf[i];
	
	if (minf > pixelf) minf = pixelf;
	if (maxf < pixelf) maxf = pixelf;
	meanf += pixelf;
	nbPoints++;
      }
    } else {
      for (i=0; i<N; i++) {
	if (M[i]) {
	  pixelf = SOURCEf[i];
	  
	  if (minf > pixelf) minf = pixelf;
	  if (maxf < pixelf) maxf = pixelf;
	  meanf += pixelf;
	  nbPoints++;
	}
      }
    }
    printf("Maximum : %lg\n", maxf);
    printf("Minimum : %lg\n", minf);
    printf("Mean    : %lg\n", (double)meanf/(double)nbPoints);
  }

  freeimage(image);

  return 0;
} /* main */
