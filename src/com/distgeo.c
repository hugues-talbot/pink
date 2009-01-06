/* $Id: distgeo.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file distgeo.c

\brief geodesic distance

<B>Usage:</B> distgeo in.pgm mask.pgm mode out.pgm

<B>Description:</B>
Geodesic distance to the object X defined by the binary image \b in.pgm ,
in the object Y defined by the binary image \b mask.pgm .

The distance d used depends on the parameter \b mode :
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> morpho
\ingroup   morpho

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldistgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t mode;
  struct xvimage * image;
  struct xvimage * mask;
  struct xvimage * result;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm mask.pgm mode out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mask = readimage(argv[2]);
  if (mask == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[3]);
  if ((mode != 4) && (mode != 8) && (mode != 6) && (mode != 18) && (mode != 26))
  {
    fprintf(stderr, "usage: %s in.pgm mask.pgm <4|8|6|18|26> fileout.pgm\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! ldistgeo(image, mask, mode, result))
  {
    fprintf(stderr, "%s: ldistgeo failed\n", argv[0]);
    exit(1);
  }
  result->name = NULL;
  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(mask);
  freeimage(result);

  return 0;
} /* main */


