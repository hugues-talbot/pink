/* $Id: dup.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file dup.c

\brief duplicates an image

<B>Usage:</B> dup in.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = in[x]

<B>Types supported:</B> all types

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
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t constante;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


