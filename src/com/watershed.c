/* $Id: watershed.c,v 1.2 2009-01-06 13:18:07 mcouprie Exp $ */
/*! \file watershed.c

\brief topological binary watershed (Couprie-Bertrand algorithm)

<B>Usage:</B> watershed in.pgm mark.pgm connex [i] out.pgm

<B>Description:</B>
Topological watershed as defined in [CB97] - connectivity <B>connex</B>.
A marker image is supplied in <B>mark.pgm</B>. The result is a binary image.
If the parameter <B>i</B> is given, then the dual operator is applied.

[CB97] M. Couprie and G. Bertrand:
``Topological Grayscale Watershed Transformation'',
SPIE Vision Geometry V Proceedings, 3168 (136--146), 1997.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
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
  struct xvimage * marker;
  int32_t connex;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 6) && strcmp(argv[4],"i"))
  {
    fprintf(stderr, "usage: %s filein.pgm mark.pgm connex [i] fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  marker = readimage(argv[2]);
  if ((image == NULL) || (marker == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  
  if (argc == 5) inverse(image);
  if (! lwshedtopobin(image, marker, connex))
  {
    fprintf(stderr, "%s: lwshedtopobin failed\n", argv[0]);
    exit(1);
  }
  if (argc == 5) inverse(image);

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marker);

  return 0;
} /* main */



