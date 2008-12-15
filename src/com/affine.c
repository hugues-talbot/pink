/* $Id: affine.c,v 1.1 2008-12-15 06:38:50 mcouprie Exp $ */
/*! \file affine.c

\brief affine transformation of a 2D image

<B>Usage:</B> affine in.pgm <trans.lst | hx hy theta tx ty> out.pgm

<B>Description:</B>

Applies to in.pgm an affine tranformation composed of a zoom 
(factors \b hx, \b hy), a rotation (angle \bf theta, center 0,0), and a
translation (vector \b tx, \b ty). Alternatively, the transformation 
parameters may be given in a type "e" list file (see doc/formats.txt).

Method: interpolation.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcgeo.h>
#include <lrotations.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * image2;
  double theta, hx, hy, tx, ty;

  if ((argc != 4) && (argc != 8))
  {
    fprintf(stderr, "usage: %s in.pgm <trans.lst | hx hy theta tx ty> out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 8)
  {
    hx = atof(argv[2]);
    hy = atof(argv[3]);
    theta = atof(argv[4]);
    theta = (theta * M_PI) / 180;
    tx = atof(argv[5]);
    ty = atof(argv[6]);
  }
  else
  {
    FILE * fd = fopen(argv[2],"r");
    char type;
    int32_t n;

    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }

    fscanf(fd, "%c", &type);
    if (type != 'e')
    {
      fprintf(stderr, "usage: %s: bad file format for %s: %c \n", argv[0], argv[2], type);
      exit(1);
    }

    fscanf(fd, "%d\n", &n);
    if (n != 5)
    {
      fprintf(stderr, "usage: %s: wrong number of parameters in %s: %d \n", argv[0], argv[2], n);
      exit(1);
    }

    fscanf(fd, "%lf%lf%lf%lf%lf", &hx, &hy, &theta, &tx, &ty);
    theta = (theta * M_PI) / 180;
  } // else

  image2 = copyimage(image);
  if (! image2)
  {
    fprintf(stderr, "%s: copyimage failed\n", argv[0]);
    exit(1);
  }

  if (! laffinetransformation(image, hx, hy, theta, tx, ty, image2))
  {
    fprintf(stderr, "%s: function laffinetransformation failed\n", argv[0]);
    exit(1);
  }
  writeimage(image2, argv[argc-1]);
  freeimage(image);
  freeimage(image2);
  return 0;
} /* main */


