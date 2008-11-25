/* $Id: barycentre.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file barycentre.c

\brief compute the center of gravity of each connected component

<B>Usage:</B> barycentre in.pgm connex out.pgm

<B>Description:</B> 
Compute the center of gravity of each connected component.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lbarycentre.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm connex out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "barycentre: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lbarycentre(image, connex))
  {
    fprintf(stderr, "%s: function lbarycentre failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
