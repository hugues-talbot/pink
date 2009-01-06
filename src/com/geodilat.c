/* $Id: geodilat.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file geodilat.c

\brief geodesic (grayscale or binary) dilation

<B>Usage:</B> geodilat g.pgm f.pgm connex niter out.pgm

<B>Description:</B>
Geodesic dilation of <B>g.pgm</B> under <B>f.pgm</B>.
Let G and F be the two input images. If G is not under F, then 
G is replaced initially by min(G,F).
The structuring element is specified by the value of the parameter <B>connex</B>, 
which can be one of the following ones: 4, 8 in 2d, or 6, 18, 26 in 3d.
The parameter \b niter sets the number of iterations. If \b niter = -1,
then the iterations continue until stability.

<B>Types supported:</B> byte 2d, byte 3d.

<B>Category:</B> connect, morpho
\ingroup  connect, morpho

\author Michel Couprie
*/

/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t connex;
  int32_t niter;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s g.pgm f.pgm connex niter out.pgm \n", argv[0]);
    fprintf(stderr, "       connex = [4|8|6|18|26]\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  connex = atoi(argv[3]);
  if ((connex!=4)&&(connex!=8)&&(connex!=6)&&(connex!=18)&&(connex!=26))
  {
    fprintf(stderr, "%s: bad connexity - use one of the following:\n", argv[0]);
    fprintf(stderr, "4, 8 (in 2d), 6, 18, 26 (in 3d)\n");
    exit(1);
  }

  niter = atoi(argv[4]);

  if (depth(image1) == 1)
  {
    if (! lgeodilat(image1, image2, connex, niter))
    {
      fprintf(stderr, "%s: function lgeodilat failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lgeodilat3d(image1, image2, connex, niter))
    {
      fprintf(stderr, "%s: function lgeodilat3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
