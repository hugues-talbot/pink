/* $Id: dilatball.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file dilatball.c

\brief morphological binary dilation by a ball

<B>Usage:</B> dilatball in.pgm r [dist] out.pgm

<B>Description:</B>
Morphological binary dilation by a metric ball.
If the parameter \b r is a positive integer, then
the structuring element for this dilation is a ball (or a disc) of radius \b r.
In this case the result of the dilation is defined by Y = {x; d(x,X) <= r} where X
represents the original point set (non-null pixels in the original image).
If the parameter \b r is -1, then 
the structuring element for each point x is a ball (or a disc) of radius \b F(x),
where F is the greyscale image  \b in.pgm .

The dilation is computed by thresholding a distance map.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance
\li 1: approximate quadratic euclidean distance
\li 2: chamfer distance
\li 3: exact quadratic euclidean distance
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

\warning The input image \b in.pgm must be a binary image. No test is done.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie 2002

<B>Example:</B>

dilatball cells 4 cells_dilatball

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_dilatball.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_dilatball</td>
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
  struct xvimage * res;
  int32_t r;
  int32_t mode;

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
  if ((mode != 0) && (mode != 1) && (mode != 2) && (mode != 3) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|1|2|3|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  if (depth(image) == 1) // 2D
  {
    if (r >= 0)
    {
      if (! ldilatdisc(image, r, mode))
      {
        fprintf(stderr, "%s: function ldilatdisc failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      if (mode == 3)
      {
      	if (!convertlong(&image))
        {
	  fprintf(stderr, "%s: function convertlong failed\n", argv[0]);
	  exit(1);
	}
	if (! (res = lredt2d(image)))
        {
	  fprintf(stderr, "%s: function lredt2d failed\n", argv[0]);
	  exit(1);
	}
      }
      else
      {
	if (! (res = ldilatdiscloc(image, mode)))
        {
	  fprintf(stderr, "%s: function ldilatdiscloc failed\n", argv[0]);
	  exit(1);
	}
      }
    }
  }
  else // 3D
  {
    if (r >= 0)
    {
      if (! ldilatball(image, r, mode))
      {
        fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      if (! (res = ldilatballloc(image, mode)))
      {
        fprintf(stderr, "%s: function ldilatballloc failed\n", argv[0]);
        exit(1);
      }
    }
  }

  if (r >= 0) 
  { 
    writeimage(image, argv[argc-1]); 
  }
  else 
  { 
    writeimage(res, argv[argc-1]);
    freeimage(res);
  }
  freeimage(image);
  return 0;
} /* main */
