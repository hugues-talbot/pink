/* $Id: volmaxima.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file volmaxima.c

\brief volume-based filtering

<B>Usage:</B> volmaxima in.pgm connex vol out.pgm

<B>Description:</B>
Volume-based filtering with connexity <B>connex</B> and volume <B>vol</B>.

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
#include <lattribvol.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex vol fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "volmaxima: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lvolmaxima(image, connex, param))
  {
    fprintf(stderr, "volmaxima: lvolmaxima failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



