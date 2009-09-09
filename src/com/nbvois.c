/* $Id: nbvois.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file nbvois.c

\brief number of neighbors for each object point

<B>Usage:</B> nbvois in connex out

<B>Description:</B>
Let X be the set of object points of the binary image \b in .
For each point x of X,
\b out (x) = card( gamma(x) inter X ). 

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lnbvois.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "nbvois: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lnbvois(image, connex))
  {
    fprintf(stderr, "nbvois: lnbvois failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
