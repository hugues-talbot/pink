/* $Id: erosion.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file erosion.c

\brief morphological erosion by a plane structuring element

<B>Usage:</B> erosion in.pgm se.pgm out.pgm

<B>Description:</B>
The (plane) structuring element is given by the non-null values in \b se.pgm, 
its origin (wrt the point (0,0) of \b se.pgm ) is given by a comment line in the file <B>se.pgm</B>. 

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

erosion cells ~/Pink/masks/carre5.pgm cells_eros

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_eros.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_eros</td>
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
    if (! leros(image, elem, x, y))
    {
      fprintf(stderr, "%s: function leros failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! leros3d(image, elem, x, y, z))
    {
      fprintf(stderr, "%s: function leros3d failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
