/* $Id: medianfilter.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file medianfilter.c

\brief median filter 

<B>Usage:</B> medianfilter in.pgm el.pgm out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element. 
then for each pixel p, G[p] is the median element of the sorted list (by 
increasing order) of the pixel values in the set { F[q], q in E[p] }.

The median filter is a particular case of rank filter (see rankfilter.c), 
with rank = 0.5 .

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie 1997
*/
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

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm el.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readse(argv[2], &x, &y, &z);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) == 1)
  {
    if (! lfiltreordre(image, elem, x, y, 0.5))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, 0.5))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
