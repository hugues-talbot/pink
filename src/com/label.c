/* $Id: label.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file label.c

\brief labeling of some flat zones of a grayscale or a binary image

<B>Usage:</B> label in.pgm connex <min|max|pla> out.pgm

<B>Description:</B>
The argument \b connex selects the connectivity (4, 8 in 2D; 6, 18, 26 in 3D).
The argument <B><min|max|pla></B> selects the kind of flat zone
to be labeled:
\li \b fgd selects foreground components for a binary image
\li \b bgd selects background components for a binary image
\li \b min selects regional minima
\li \b max selects regional maxima
\li \b pla selects all flat zones (plateaux).
The output image \b out.pgm has the type "int32_t".

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <llabelextrema.h>
#include <llabelplateaux.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, connex, i;
  struct xvimage * image;
  struct xvimage * result;
  int32_t function;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex <fgd|bgd|min|max|pla> fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (strcmp(argv[3], "fgd") == 0) function = LABFGD; else
  if (strcmp(argv[3], "bgd") == 0) function = LABBGD; else
  if (strcmp(argv[3], "min") == 0) function = LABMIN; else
  if (strcmp(argv[3], "max") == 0) function = LABMAX; else
  if (strcmp(argv[3], "pla") == 0) function = LABPLA; else
  {
    fprintf(stderr, "usage: %s filein.pgm connex <fgd|bgd|min|max|pla> fileout.pgm\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  switch (function)
  {
  case LABMIN:
  case LABMAX:
    if (! llabelextrema(image, connex, function, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
    break;
  case LABPLA:
    if (! llabelplateaux(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelplateaux failed\n", argv[0]);
      exit(1);
    }
    break;
  case LABFGD:
    if (! llabelbin(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelbin failed\n", argv[0]);
      exit(1);
    }    
    break;
  case LABBGD:
    (void)linverse(image);
    if (! llabelbin(image, connex, result, &nblabels))
    {
      fprintf(stderr, "%s: llabelbin failed\n", argv[0]);
      exit(1);
    }    
    break;
  } // switch (function)

  writelongimage(result, argv[argc-1]);
  freeimage(result);
  freeimage(image);

  return 0;
} /* main */


