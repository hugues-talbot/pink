/* $Id: addconst.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file addconst.c

\brief adds a constant value to an image

<B>Usage:</B> addconst in.pgm const out.pgm

<B>Description:</B>
For each pixel x, out[x] = in[x] + const. If out[x] 
exceeds 255, then out[x] is set to 255.

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

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

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm constante out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  constante = atoi(argv[2]);

  if (! laddconst(image1, constante))
  {
    fprintf(stderr, "%s: function laddconst failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


