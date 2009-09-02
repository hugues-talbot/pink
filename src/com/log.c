/* $Id: log.c,v 1.1 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file log.c

\brief pixelwise logarithm

<B>Usage:</B> log in.pgm out.pgm

<B>Description:</B>
For each pixel x, out[x] = log(in[x]).

<B>Types supported:</B> float 2d, float 3d

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

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! llog(image1))
  {
    fprintf(stderr, "%s: function llog failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */
