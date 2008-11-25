/* $Id: reconsdilat3d.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file reconsdilat3d.c

obsolete - use geodilat

\brief morphological reconstruction by dilation

<B>Usage:</B> reconsdilat3d g.pgm f.pgm el out.pgm

<B>Description:</B>
Reconstruction of <B>g.pgm</B> under <B>f.pgm</B>.
The structuring element is specified by the value of the parameter <B>el</B>, 
which can be one of the following ones: vois6 (or 6), vois18 (or 18), vois26 (or 26).

<B>Types supported:</B> byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lreconsdilat3d.h>

/* les elements structurants */
/*                        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 */
uint8_t vois26[26]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
uint8_t vois18[26]={0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0};
uint8_t vois6 [26]={0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;
  uint8_t *mask = NULL;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s g.pgm f.pgm maskname out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if (strcmp(argv[3], "vois26") == 0) mask = vois26; else
  if (strcmp(argv[3], "vois18") == 0) mask = vois18; else
  if (strcmp(argv[3], "vois6") == 0) mask = vois6; else
  if (strcmp(argv[3], "26") == 0) mask = vois26; else
  if (strcmp(argv[3], "18") == 0) mask = vois18; else
  if (strcmp(argv[3], "6") == 0) mask = vois6; else
  {
    fprintf(stderr, "%s: bad maskname - use one of the following:\n", argv[0]);
    fprintf(stderr, "vois26, vois18, vois6, 26, 18, 6\n");
    exit(1);
  }

  if (! lreconsdilat3d(image1, image2, mask))
  {
    fprintf(stderr, "%s: function lreconsdilat3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[4]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */

