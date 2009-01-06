/* $Id: seuil2.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file seuil2.c

\brief double threshold

<B>Usage:</B> seuil2 in.pgm smin smax vmin vmax out.pgm

<B>Description:</B>
Every pixel < \b smin gets value \b vmin 
Every pixel >= \b smax gets value \b vmax 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseuil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  uint8_t seuilmin, seuilmax, valmin, valmax;
  struct xvimage * image;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s filein.pgm smin smax vmin vmax fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  seuilmin = (uint8_t)atoi(argv[2]);
  seuilmax = (uint8_t)atoi(argv[3]);
  valmin = (uint8_t)atoi(argv[4]);
  valmax = (uint8_t)atoi(argv[5]);

  if (! lseuil2(image, seuilmin, seuilmax, valmin, valmax))
  {
    fprintf(stderr, "%s: function lseuil failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
