/* $Id: localextrema.c,v 1.3 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file localextrema.c

\brief local extrema 

<B>Usage:</B> in.pgm connex minimum out.pgm

<B>Description:</B> 
Selects the local maxima or minima of a grayscale image with connexity <B>connex</B>.

<B>Types supported:</B> byte 2d - byte 3d

<B>Category:</B> connect, topogray
\ingroup  connect, topogray

\author Laurent Najman
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llocalextrema.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * maxima;
  int32_t connex, minim;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex minim fileout.pgm (connex = 4,8 (2D) 6,18,26(3D)) \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "localextrema: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  minim = atoi(argv[3]);

  maxima = allocimage(NULL, rowsize(image), colsize(image), depth(image), datatype(image));
  if (maxima == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  if (! llocalextrema(image, connex, minim, maxima)) {
    fprintf(stderr, "%s: llocalextrema failed\n", argv[0]);
    exit(1);
  }
  writeimage(maxima, argv[argc-1]);
  freeimage(maxima);
  freeimage(image);

  return 0;
} /* main */


