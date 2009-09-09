/* $Id: asftndg.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file asftndg.c

\brief alternate sequential filter controled by topology

<B>Usage:</B> asftndg in.pgm <c.pgm cc.pgm|null null> connex rmax out.pgm

<B>Description:</B>
Alternate sequential filter controled by topology for multilevel images.
Disc- or Ball-shaped structuring elements of increasing radius,
ranging from 1 to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I(0) = <B>in.pgm</B>, the ith intermediate result I(i) is obtained by the 
homotopic pseudo-closing of the homotopic pseudo-opening of I(i-1) by the structuring element Di. 
Two images \b c.pgm and \b cc.pgm can be given to serve as constraints. 
The image \b c.pgm must be 0 except for the points x that are to be preserved,
which can be set to 255 or to I[x].
The image \b cc.pgm must be 255 except for the points x that are to be preserved,
which can be set to 0 or to I[x].
The result <B>out.pgm</B> contains the final result I(n).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho, topogray
\ingroup  morpho, topogray

\author Michel Couprie
*/
/* M. Couprie -- juillet 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lasft.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagec = NULL;
  struct xvimage * imagecc = NULL;
  int32_t rayonmax;
  int32_t connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm <c.pgm cc.pgm|null null> connex rmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((strcmp(argv[2],"null") != 0) && (strcmp(argv[3],"null") != 0))
  {
    imagec = readimage(argv[2]);
    imagecc = readimage(argv[3]);
    if ((imagec == NULL) || (imagecc == NULL))
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  connex = atoi(argv[4]);
  rayonmax = atoi(argv[5]);

  if (depth(image) == 1)
  {
    if (! lasft_ndg(image, imagec, imagecc, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasft_ndg failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lasft_ndg3d(image, imagec, imagecc, connex, rayonmax))
    {
      fprintf(stderr, "%s: lasft_ndg3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  if (imagec) freeimage(imagec);
  if (imagecc) freeimage(imagecc);

  return 0;
} /* main */
