/* $Id: histolisse.c,v 1.0 2003/05/12 */
/*! \file histolisse.c

\brief smoothed histogram of the gray values

<B>Usage:</B> histolisse in.pgm L [mask.pgm] out.pgm

<B>Description:</B>
Generates a binary image which contains the graphical representation
of the smoothed histogram of \b in.pgm (masked by the binary image
\b mask.pgm, if given). \b L : smoothing coefficient, 0<L<255.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> histo
\ingroup  histo

\author Cedric Allene 2003
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imhist;
  struct xvimage * mask = NULL;
  uint32_t * histo;
  int32_t x, y, z, somme, lissage;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm L [mask.pgm] out.pgm\n", argv[0]);
    exit(1);
  }

  lissage = atoi (argv[2]);
  if ((lissage < 1) || (lissage > 254))
  {
    fprintf(stderr, "%s: 0<L<255 \n", argv[0]);
    exit(1);
  }
  
  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  histo = (uint32_t *)calloc((NDG_MAX - NDG_MIN + 1) * sizeof(int32_t), 1);
  if (histo == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5)
  {
    mask = readimage(argv[3]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lhisto(image, mask, histo))
  {
    fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
    exit(1);
  }

  
/* Lissage de l'histogramme */
  for (x=0; x<=NDG_MAX - NDG_MIN; x++)
  {
    somme = 0;
    z=0;
    for (y=-lissage/2; y<=lissage/2; y++)
    {
      z++;
      if ((x-y)>=0) somme = somme + (int32_t)histo[x-y];
    }
    histo [x]=(uint32_t)(somme/z);
  }


/* Ecriture de l'image histogramme */
  if (! lhisto2image(histo, NDG_MAX - NDG_MIN + 1, &imhist))
  {
    fprintf(stderr, "%s: function lhisto2image failed\n", argv[0]);
    exit(1);
  }

  writeimage(imhist, argv[argc - 1]);
  free(histo);
  freeimage(image);
  freeimage(imhist);
  if (mask) freeimage(mask);
  return 0;
} /* main */
