/* $Id: stretch.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file stretch.c

OBSOLETE - use normalize

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* etire l'echelle de gris d'une image */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lstretch.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "stretch: readimage failed\n");
    exit(1);
  }

  if (! lstretch(image))
  {
    fprintf(stderr, "stretch: function lstretch failed\n");
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */
