/* $Id: gradinf.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file gradinf.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* gradient inf */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgradinf.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t i;
  uint8_t *pt;
  int32_t N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "gradinf: readimage failed\n");
    exit(1);
  }

  if (! lgradinf(image))
  {
    fprintf(stderr, "gradinf: function lgradinf failed\n");
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */
