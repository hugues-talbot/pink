/* $Id: remspnoise.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file remspnoise.c

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
#include <lremspnoise.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t g, k;
  struct xvimage * image;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm g k fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "remspnoise: readimage failed\n");
    exit(1);
  }

  g = atoi(argv[2]);
  k = atoi(argv[3]);

  if (! lremspnoise(image, g, k))
  {
    fprintf(stderr, "remspnoise: function lremspnoise failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
