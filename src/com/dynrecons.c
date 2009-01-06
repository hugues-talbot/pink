/* $Id: dynrecons.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file dynrecons.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm imcond.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "dynrecons: readimage failed\n");
    exit(1);
  }

  imagecond = readimage(argv[2]);
  if (imagecond == NULL)
  {
    fprintf(stderr, "dynrecons: readimage failed\n");
    exit(1);
  }

  if (! ldynrecons(image, imagecond, connex))
  {
    fprintf(stderr, "dynrecons: ldynrecons failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
