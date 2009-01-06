/* $Id: dilation.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file dilation.c

\brief morphological dilation by a plane structuring element

<B>Usage:</B> dilation in.pgm se.pgm out.pgm

<B>Description:</B>
The (plane) structuring element is given by the non-null values in \b se.pgm, 
its origin (wrt the point (0,0) of \b se.pgm ) is given by a comment line in the file <B>se.pgm</B>. 

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, long3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

dilation cells ~/Pink/masks/carre5.pgm cells_dilat

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_dilat.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_dilat</td>
  </tr>
</table>

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>
#include <ldilateros3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y, z;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readse(argv[2], &x, &y, &z);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "%s: readse failed\n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1)
  {
    if (! ldilat(image, elem, x, y))
    {
      fprintf(stderr, "%s: function ldilat failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! ldilat3d(image, elem, x, y, z))
    {
      fprintf(stderr, "%s: function ldilat3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
