/* $Id: hitormiss.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file hitormiss.c

\brief morphological hit or miss transformation for binay images

<B>Usage:</B> hitormiss in.pgm e1.pgm e2.pgm out.pgm

<B>Description:</B>
Let X be the input set, E1, E2 be the two structuring elements, the result Y is given by:<br>
Y = (X - E1) inter (Xb - E2)<br>
where '-' denotes the erosion operator, and 'Xb' denotes the complementary of X.  

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, long3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2003

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <ldilateros3d.h>
#include <larith.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * tmp;
  struct xvimage * elem1;
  struct xvimage * elem2;
  int32_t x1, y1, z1;
  int32_t x2, y2, z2;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm e1.pgm e2.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem1 = readse(argv[2], &x1, &y1, &z1);
  elem2 = readse(argv[3], &x2, &y2, &z2);
  if ((image == NULL) || (elem1 == NULL) || (elem2 == NULL))
  {
    fprintf(stderr, "%s: readimage or readse failed\n", argv[0]);
    exit(1);
  }

  tmp = copyimage(image);
  linverse(tmp);
  if (depth(image) == 1)
  {
    if (! leros(image, elem1, x1, y1))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
writeimage(image, "_t1");
    if (! leros(tmp, elem2, x2, y2))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
writeimage(tmp, "_t2");
    lmin(image,tmp);
  }
  else
  {
    if (! leros3d(image, elem1, x1, y1, z1))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
    if (! leros3d(tmp, elem2, x2, y2, z2))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
    lmin(image,tmp);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(elem1);
  freeimage(elem2);
  freeimage(tmp);

  return 0;
} /* main */
