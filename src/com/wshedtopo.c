/* $Id: wshedtopo.c,v 1.2 2009-01-06 13:18:07 mcouprie Exp $ */
/*! \file wshedtopo.c

\brief topological grayscale watershed  (Couprie-Bertrand algorithm)

<B>Usage:</B> wshedtopo in.pgm connex [i] out.pgm

<B>Description:</B>
Topological grayscale watershed as defined in [CB97], preserves the minima - connectivity <B>connex</B>
If the parameter <B>i</B> is given, then the dual operator is applied.

[CB97] M. Couprie and G. Bertrand:
``Topological Grayscale Watershed Transformation'',
SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie

<B>Example:</B>

wshedtopo ur1 4 ur1_wshedtopo

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/ur1.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/ur1_wshedtopo.gif"></td>
  </tr>
  <tr><td align="center">ur1</td>
      <td align="center">ur1_wshedtopo</td>
  </tr>
</table>
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lwshedtopo.h>

/* ==================================== */
static void inverse(struct xvimage * image)
/* ==================================== */
{
  int32_t i, N = rowsize(image) * colsize(image) * depth(image);
  uint8_t *pt;
  for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    *pt = NDG_MAX - *pt;
} // inverse

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 5) && strcmp(argv[3],"i"))
  {
    fprintf(stderr, "usage: %s filein.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (argc == 4) inverse(image);
  if (! lwshedtopo(image, connex))
  {
    fprintf(stderr, "%s: lwshedtopo failed\n", argv[0]);
    exit(1);
  }
  if (argc == 4) inverse(image);

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */



