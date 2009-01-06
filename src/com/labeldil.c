/* $Id: labeldil.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file labeldil.c

\brief labeling of the connected components of a binary image

<B>Usage:</B> labeldil in.pgm se.pgm out.pgm

<B>Description:</B>
The argument \b se.pgm is a structuring element which must be reflexive and symmetric. 
Connected components are defined by this structuring element.
The output image \b out.pgm has the type "int32_t".

\warning No test is done to check properties of the structuring element.

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup  connect

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelextrema.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, i;
  struct xvimage * image;
  struct xvimage * result;
  struct xvimage * se;
  int32_t x, y, z;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm se.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  se = readse(argv[2], &x, &y, &z);
  if (se == NULL)
  {
    fprintf(stderr, "%s: readse failed\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! llabeldil(image, se, x, y, result, &nblabels))
  {
    fprintf(stderr, "%s: llabeldil failed\n", argv[0]);
    exit(1);
  }

  writelongimage(result, argv[argc-1]);
  freeimage(result);
  freeimage(image);
  freeimage(se);

  return 0;
} /* main */


