/* $Id: histopgm.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file histopgm.c

\brief histogram of the gray values

<B>Usage:</B> histopgm in.pgm [mask.pgm] out.pgm

<B>Description:</B>
Generates a binary image which contains the graphical representation
of the histogram of \b in.pgm (masked by the binary image
\b mask.pgm, if given).

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

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

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imhist;
  struct xvimage * mask = NULL;
  uint32_t * histo;
  int32_t i,size;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [mask.pgm] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
    {
      mask = readimage(argv[2]);
      if (mask == NULL)
	{
	  fprintf(stderr, "%s: readimage failed\n", argv[0]);
	  exit(1);
	}
    }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    histo = (uint32_t *)calloc(1,(NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
    if (histo == NULL)
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }

    if (! lhisto(image, mask, histo))
    {
      fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
      exit(1);
    }

    if (! lhisto2image(histo, NDG_MAX - NDG_MIN + 1, &imhist))
      {
	fprintf(stderr, "%s: function lhisto2image failed\n", argv[0]);
	exit(1);
      }
  } 
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    if (! lhistolong(image, mask, &histo, &size))
    {
      fprintf(stderr, "histogram: function lhistolong failed\n");
      exit(1);
    }

    if (! lhisto2image(histo, size, &imhist))
      {
	fprintf(stderr, "%s: function lhisto2image failed\n", argv[0]);
	exit(1);
      }
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float w, smin, smax;
    if (! lhistofloat(image, mask, &histo, &size, &w, &smin, &smax))
    {
      fprintf(stderr, "%s: function lhistofloat failed\n", argv[0]);
      exit(1);
    }

    if (! lhisto2image(histo, size, &imhist))
      {
	fprintf(stderr, "%s: function lhisto2image failed\n", argv[0]);
	exit(1);
      }
  }

  writeimage(imhist, argv[argc - 1]);
  free(histo);
  freeimage(image);
  freeimage(imhist);
  if (mask) freeimage(mask);
  return 0;
} /* main */
