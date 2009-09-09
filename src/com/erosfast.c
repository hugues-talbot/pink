/* $Id: erosfast.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file erosfast.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>erosfast</B> - morphological erosion by an elementary plane structuring element

SYNOPSIS

<B>erosfast</B> in.pgm el out.pgm

DESCRIPTION

The structuring element is specified by the value of the parameter <B>el</B>, 
which can be one of the following ones: vois4, vois8, croix, horiz, vert, mont, desc.

Types supported: byte 2D.

CLASS 

morph

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
  
  if (strcmp(argv[2], "vois8") == 0) mask = vois8; else
  if (strcmp(argv[2], "vois4") == 0) mask = vois4; else
  if (strcmp(argv[2], "croix") == 0) mask = croix; else
  if (strcmp(argv[2], "horiz") == 0) mask = horiz; else
  if (strcmp(argv[2], "vert") == 0) mask = vert; else
  if (strcmp(argv[2], "mont") == 0) mask = mont; else
  if (strcmp(argv[2], "desc") == 0) mask = desc; else
  {
    fprintf(stderr, "%s: bad maskname - use one of the following:\n", argv[0]);
    fprintf(stderr, "vois8, vois4, croix, horiz, vert, mont, desc\n");
    exit(1);
  }

  if (! lerosfast(image, mask))
  {
    fprintf(stderr, "%s: function lerosfast failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
