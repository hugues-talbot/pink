/* $Id: rankfilter.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file rankfilter.c

\brief rank filter 

<B>Usage:</B> rankfilter in.pgm el.pgm r out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element. Let n be the number of elements of E, and R be the product n.r,
then for each pixel p, G[p] is the Rth element of the sorted list (by 
increasing order) of the pixel values in the set { F[q], q in E[p] }.

Particular cases are the median filter (r = 0.5), the erosion (r = 0),
and the dilation (r = 1).

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie
*/
/* filtre d'ordre sur un voisinage quelconque */
/* Michel Couprie - decembre 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y, z;
  double r;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm el.pgm r out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readse(argv[2], &x, &y, &z);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  r = atof(argv[3]);
  if ((r < 0.0) || (r > 1.0))
  {
    fprintf(stderr, "%s: r = %g ; on doit avoir 0 <= r <= 1\n", argv[0], r);
    exit(1);
  }
  if (depth(image) == 1)
  {
    if (! lfiltreordre(image, elem, x, y, r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
