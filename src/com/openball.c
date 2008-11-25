/* $Id: openball.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file openball.c

\brief morphological binary closing by a ball

<B>Usage:</B> openball in.pgm r [dist] out.pgm

<B>Description:</B>
The structuring element for this opening is a ball (or a disc) of radius \b r.
The opening consists in a erosion (erosball) followed by an dilation (dilatball).
The erosion and dilation are computed by thresholding a distance map.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance
\li 2: chamfer distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

openball cells 3 cells_openball

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_openball.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_openball</td>
  </tr>
</table>

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t r, mode;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s f.pgm r [dist] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  r = atoi(argv[2]);

  if (argc == 5) mode = atoi(argv[3]); else mode = 0;
  if ((mode != 0) && (mode != 2) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1)
  {
    if (! lerosdisc(image, r, mode))
    {
      fprintf(stderr, "%s: function ldilatdisc failed\n", argv[0]);
      exit(1);
    }
    if (! ldilatdisc(image, r, mode))
    {
      fprintf(stderr, "%s: function lerosdisc failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lerosball(image, r, mode))
    {
      fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
      exit(1);
    }
    if (! ldilatball(image, r, mode))
    {
      fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
