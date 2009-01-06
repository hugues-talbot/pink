/* $Id: hthiniso.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file hthiniso.c

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
#include <lhthiniso.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t nitermax;
  int32_t connex;
  double dmax;
  double pixwhratio = 1.0;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm connex dmax fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  dmax = atof(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lhthiniso(image, dmax, connex, pixwhratio))
  {
    fprintf(stderr, "%s: lhthiniso failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
