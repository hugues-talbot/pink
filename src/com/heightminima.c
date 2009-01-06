/* $Id: heightminima.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file heightminima.c

\brief h-minima operator

<B>Usage:</B> heightminima in.pgm connex height out.pgm

<B>Description:</B>
h-minima with connexity <B>connex</B> and depth <B>height</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribheight.h>

/* ==================================== */
static void inverse(struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt;
  for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    *pt = NDG_MAX - *pt;
} // inverse

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex height fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "heightminima: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  inverse(image);
  if (! lheightmaxima(image, connex, param))
  {
    fprintf(stderr, "heightminima: lheightmaxima failed\n");
    exit(1);
  }
  inverse(image);

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



