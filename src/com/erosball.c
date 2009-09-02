/* $Id: erosball.c,v 1.3 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file erosball.c

\brief morphological binary erosion by a ball

<B>Usage:</B> erosball in.pgm r [dist] out.pgm

<B>Description:</B>
Morphological binary erosion by a metric ball.
This erosion is the dual of the dilatball operator, 
i.e. erosball(X) = inverse(dilatball(inverse(X))).
The structuring element for this erosion is a ball (or a disc) of radius \b r.
The erosion is computed by thresholding a distance map.
The distance used depends on the optional parameter \b dist (default is 0) :
\li 0: approximate euclidean distance
\li 2: chamfer distance
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

erosball cells 3 cells_erosball

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_erosball.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_erosball</td>
  </tr>
</table>

*/

/*
%TEST erosball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 0 %RESULTS/erosball_b2hebreu_3_0.pgm
%TEST erosball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 2 %RESULTS/erosball_b2hebreu_3_2.pgm
%TEST erosball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 4 %RESULTS/erosball_b2hebreu_3_4.pgm
%TEST erosball %IMAGES/2dbyte/binary/b2hebreu.pgm 3 8 %RESULTS/erosball_b2hebreu_3_8.pgm
%TEST erosball %IMAGES/3dbyte/binary/b3a.pgm 3 0 %RESULTS/erosball_b3a_3_0.pgm
%TEST erosball %IMAGES/3dbyte/binary/b3a.pgm 3 2 %RESULTS/erosball_b3a_3_2.pgm
%TEST erosball %IMAGES/3dbyte/binary/b3a.pgm 3 6 %RESULTS/erosball_b3a_3_6.pgm
%TEST erosball %IMAGES/3dbyte/binary/b3a.pgm 3 18 %RESULTS/erosball_b3a_3_18.pgm
%TEST erosball %IMAGES/3dbyte/binary/b3a.pgm 3 26 %RESULTS/erosball_b3a_3_26.pgm
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
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
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
  if ((mode != 0) && (mode != 2) && (mode != 4) && 
      (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "%s: dist = [0|2|4|8|6|18|26] \n", argv[0]);
    exit(1);
  }

  if (! lerosball(image, r, mode))
  {
    fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */
