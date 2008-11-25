/* $Id: drawtorus.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */

/*! \file drawtorus.c

\brief generates a binary torus

<B>Usage:</B> drawtorus <in.pgm|null> r1 r2 xc yc zc out.pgm

<B>Description:</B>
Draws a binary torus in the image \b in.pgm.
The parameters <B>r1, r2</B> specify respectively the big 
and the small radius of the torus. 
The parameters <B>xc, yc, zc</B> specify the center of the torus. 
If the first parameter is "null", then an image with the
appropriate size is created.

<B>Types supported:</B> byte 3d

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ldraw.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  double xc, yc, zc;
  double r1, r2;

  if (argc != 8)
  {
    fprintf(stderr, "usage: %s <in.pgm|null> r1 r2 xc yx zc out.pgm \n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[1], "null") == 0)
  {
  }
  else 
  {
    image = readimage(argv[1]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  r1 = atof(argv[2]);
  r2 = atof(argv[3]);
  
  xc = atof(argv[4]);
  yc = atof(argv[5]);
  zc = atof(argv[6]);

  if (strcmp(argv[1], "null") == 0)
  {
    int32_t rs = xc + (int32_t)(r1 + r2) + 1;
    int32_t cs = yc + (int32_t)(r1 + r2) + 1;
    int32_t ds = zc + (int32_t)r2 + 1;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
  }
  else 
  {
    image = readimage(argv[1]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  ldrawtorus(image, r1, r2, xc, yc, zc);

  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */

