/* $Id: histo2.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file histo2.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

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
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  struct xvimage * mask = NULL;
  struct xvimage * histo;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in1.pgm in2.pgm [mask.pgm] histo.pgm\n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
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

  /* ---------------------------------------------------------- */
  /* alloc histo */
  /* ---------------------------------------------------------- */

  histo = allocimage(NULL, 256, 256, 1, VFF_TYP_4_BYTE);
  if (histo == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  /* ---------------------------------------------------------- */
  /* calcul histo */
  /* ---------------------------------------------------------- */

  if (! lhisto2(image1, image2, mask, histo))
  {
    fprintf(stderr, "%s: function lhisto2 failed\n", argv[0]);
    exit(1);
  }

  writeimage(histo, argv[argc - 1]);
  freeimage(histo);
  freeimage(image1);
  freeimage(image2);
  if (mask) freeimage(mask);

  return 0;
} /* main */
