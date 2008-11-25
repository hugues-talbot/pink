/* $Id: maxdiameter.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file maxdiameter.c

\brief maximal diameter of a binary image

<B>Usage:</B> maxdiameter in connex out

<B>Description:</B>

Find two points x, y in a set X (binary image <B>in</B>) such that 
d(x,y) is less or equal to d(v,w) for any two points v, w in X.
The distance d is the Euclidean distance.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Gu Jun
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmaxdiameter.h>


/* =============================================================== */
int main(argc, argv)
/* =============================================================== */
  int argc; char **argv; 
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
    fprintf(stderr, "maxdiameter: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lmaxdiameter(image, connex))
  {
    fprintf(stderr, "maxdiameter: lmaxdiameter failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
