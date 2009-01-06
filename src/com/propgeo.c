/* $Id: propgeo.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file propgeo.c

\brief geodesic propagation of image values

<B>Usage:</B> propgeo in.pgm mask.pgm connex mode out.pgm

<B>Description:</B>
Propagates certain values of the original image <B>in.pgm</B>, 
geodesicaly wrt the connected components of the binary image <B>mask.pgm</B>.
The connexity is specified by the parameter <B>connex</B>.
The <B>mode</B> is one of the following ones:
\li    min   : propagates the minimal value of the component
\li    min1  : selects one point the value of which is equal to the min
\li    max   : propagates the maximal value of the component
\li    max1  : selects one point the value of which is equal to the max
\li    moy   : propagates the mean grayscale value of the component
\li    moy1  : selects one point the value of which is nearest to the mean
\li    minb  : propagates the minimal value of the external border of the component
\li    maxb  : propagates the maximal value of the external border of the component
\li    moyb  : propagates the mean value of the external border of the component
\li    randb : fills the component with random values, with a normal distribution
            centered around the value computed as for moyb

Only modes min, max and moy are available for int32_t images.

<B>Types supported:</B> byte 2d, int32_t 2d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lpropgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * masque;
  int32_t connex; 
  int32_t function;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s filein.pgm mask.pgm connex <min1|min|minb|max1|max|maxb|moy1|moy|moyb|randb> fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  masque = readimage(argv[2]);
  if (masque == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);  

  if (strcmp(argv[4], "min1") == 0) function = MIN1; else
  if (strcmp(argv[4], "min") == 0) function = MIN; else
  if (strcmp(argv[4], "minb") == 0) function = MINB; else
  if (strcmp(argv[4], "max1") == 0) function = MAX1; else
  if (strcmp(argv[4], "max") == 0) function = MAX; else
  if (strcmp(argv[4], "maxb") == 0) function = MAXB; else
  if (strcmp(argv[4], "moy1") == 0) function = MOY1; else
  if (strcmp(argv[4], "moy") == 0) function = MOY; else
  if (strcmp(argv[4], "moyb") == 0) function = MOYB; else
  if (strcmp(argv[4], "randb") == 0) function = RANDB; else
  {
    fprintf(stderr, "usage: %s filein.pgm mask.pgm connex <min1|min|minb|max1|max|maxb|moy1|moy|moyb|randb> fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (! lpropgeo(image, masque, connex, function))
  {
    fprintf(stderr, "%s: lpropgeo failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[5]);
  freeimage(image);
  freeimage(masque);

  return 0;
} /* main */
