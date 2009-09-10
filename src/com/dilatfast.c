/* $Id: dilatfast.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file dilatfast.c

\brief morphological dilation by an elementary structuring element

<B>Usage:</B> dilatfast in.pgm el out.pgm

<B>Description:</B>

The structuring element is specified by the value of the parameter <B>el</B>, 
which can be one of the following ones: 4, 8, croix, horiz, vert, mont, desc in 2D, 
or 26, 18, 6 in 3D. 

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho 
\ingroup  morpho

\author Michel Couprie
*/

/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <ldilateros3d.h>

/* les elements structurants 3-D */
/*                               0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 */
static uint8_t vois26[26]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static uint8_t vois18[26]={0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0};
static uint8_t vois6 [26]={0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
/* les elements structurants 2-D */
static uint8_t vois8[8] = {1, 1, 1, 1, 1, 1, 1, 1};
static uint8_t vois4[8] = {1, 0, 1, 0, 1, 0, 1, 0};
static uint8_t croix[8] = {0, 1, 0, 1, 0, 1, 0, 1};
/* les elements structurants 1-D */
static uint8_t horiz[8] = {1, 0, 0, 0, 1, 0, 0, 0};
static uint8_t vert[8]  = {0, 0, 1, 0, 0, 0, 1, 0};
static uint8_t mont[8]  = {0, 1, 0, 0, 0, 1, 0, 0};
static uint8_t desc[8]  = {0, 0, 0, 1, 0, 0, 0, 1};

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *mask = NULL;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm maskname out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if ((strcmp(argv[2], "vois6") == 0) || (strcmp(argv[2], "6") == 0)) mask = vois6; else
  if ((strcmp(argv[2], "vois18") == 0) || (strcmp(argv[2], "18") == 0)) mask = vois18; else
  if ((strcmp(argv[2], "vois26") == 0) || (strcmp(argv[2], "26") == 0)) mask = vois26; else
  if ((strcmp(argv[2], "vois4") == 0) || (strcmp(argv[2], "4") == 0)) mask = vois4; else
  if ((strcmp(argv[2], "vois8") == 0) || (strcmp(argv[2], "4") == 0)) mask = vois8; else
  if (strcmp(argv[2], "croix") == 0) mask = croix; else
  if (strcmp(argv[2], "horiz") == 0) mask = horiz; else
  if (strcmp(argv[2], "vert") == 0) mask = vert; else
  if (strcmp(argv[2], "mont") == 0) mask = mont; else
  if (strcmp(argv[2], "desc") == 0) mask = desc; else
  {
    fprintf(stderr, "%s: bad maskname - use one of the following:\n", argv[0]);
    fprintf(stderr, "6, 18, 26, 8, 4, croix, horiz, vert, mont, desc\n");
    exit(1);
  }

  if ((mask == vois6)  || (mask == vois18) || (mask == vois26))
  {
    if (! ldilatfast3d(image, mask))
    {
      fprintf(stderr, "%s: function ldilatfast3d failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! ldilatfast(image, mask))
    {
      fprintf(stderr, "%s: function ldilatfast failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
