/* $Id: rankmaxopening.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file rankmaxopening.c

\brief rank-max opening 

<B>Usage:</B> rankmaxopening in.pgm el.pgm r out.pgm

<B>Description:</B>
Let F be the input image, G be the output image, and E the structuring
element.

The rank-max opening [Ron86]
of structuring element $E$ and parameter \b r \in [0...1]
may be defined by G = min(F,dilation(rankfilter(F,E,1-r),E))

[Ron86] C. Ronse:
"Erosion of narrow image features by combination of local low rank and max filters",
Proc. 2nd Int. Conf. on Image Processing and int32_t Applications,
pages 77-81, 1986.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup morpho

\author Michel Couprie
*/
/* Michel Couprie - janvier 2005 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>
#include <ldilateros.h>
#include <ldilateros3d.h>
#include <larith.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * orig;
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

  orig = copyimage(image);

  if (depth(image) == 1)
  {
    if (! lfiltreordre(image, elem, x, y, 1.0 - r))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    if (! ldilat(image, elem, x, y))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    (void)lmin(image, orig);
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, 1.0 - r))
    {
      fprintf(stderr, "%s: function lfiltreordre3d failed\n", argv[0]);
      exit(1);
    }
    if (! ldilat3d(image, elem, x, y, z))
    {
      fprintf(stderr, "%s: function lfiltreordre failed\n", argv[0]);
      exit(1);
    }
    (void)lmin(image, orig);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(orig);

  return 0;
} /* main */
