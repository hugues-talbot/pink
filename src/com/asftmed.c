/* $Id: asftmed.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file asftmed.c

\brief alternate sequential filter controled by topology

<B>Usage:</B> asftmed in.pgm connex rmax out.pgm

<B>Description:</B>
Alternate sequential filter controled by topology. 
Disc- or Ball-shaped structuring elements of increasing radius,
ranging from 1 to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I(0) = <B>in.pgm</B>, the ith intermediate result I(i) is obtained by the 
homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. 
Furthermore, the medial axes (distance 8 in 2D, 26 in 3D) of both the input image and its inverse 
are used as constraint sets.
The result <B>out.pgm</B> contains the final result I(n).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho, topobin
\ingroup  morpho, topobin

\author Michel Couprie
*/
/* M. Couprie -- mai 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lasft.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t rayonmax, rayon;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm connex rmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  rayonmax = atoi(argv[3]);

  if (depth(image) == 1)
  {
    if (! lasftmed(image, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasftmed failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lasftmed3d(image, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasftmed3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
