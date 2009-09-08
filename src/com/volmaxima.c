/* $Id: volmaxima.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
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

/*
%TEST volmaxima %IMAGES/2dbyte/gray/g2gel.pgm 4 20 %RESULTS/volmaxima_g2gel_4_20.pgm
%TEST volmaxima %IMAGES/2dbyte/gray/g2gel.pgm 8 20 %RESULTS/volmaxima_g2gel_8_20.pgm
%TEST volmaxima %IMAGES/3dbyte/gray/g3a.pgm 6 20 %RESULTS/volmaxima_g3a_6_20.pgm
%TEST volmaxima %IMAGES/3dbyte/gray/g3a.pgm 18 20 %RESULTS/volmaxima_g3a_18_20.pgm
%TEST volmaxima %IMAGES/3dbyte/gray/g3a.pgm 26 20 %RESULTS/volmaxima_g3a_26_20.pgm
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



