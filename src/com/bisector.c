/* $Id: bisector.c,v 1.3 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file bisector.c

\brief compute the bisector function of a binary image

<B>Usage:</B> bisector dist.pgm mask.pgm out.pgm

<B>Description:</B>

The bisector function is computed for the points which are non-null in the binary image
\b mask.pgm , from the exact quadratic euclidean distance map found in \b dist.pgm .

\warning If the image \b dist.pgm is not an exact quadratic euclidean distance map, 
then the result of this operator will make no sense. 

References:<BR> 
[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: <A HREF="http://www.esiee.fr/~coupriem/Pdf/ccz07.pdf">"Discrete bisector function and Euclidean skeleton in 2D and 3D"</A>, <I>Image and Vision Computing</I>, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.<BR>

<B>Types supported:</B> byte 2d, byte 3d 

<B>Category:</B> morpho
\ingroup  morpho


\author Rita Zrour, Michel Couprie
*/

/*
%TEST bisector %IMAGES/2dlong/l2fish1.pgm %IMAGES/2dbyte/binary/b2fish1.pgm %RESULTS/bisector_b2fish1.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * dist;
  struct xvimage * mask;
  struct xvimage * angle;
  int32_t N, i,j, y;
  uint8_t *D, *M, *A;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s dist.pgm mask.pgm out.pgm\n", argv[0]);
    exit(1);
  }

  dist = readimage(argv[1]);
  mask = readimage(argv[2]);
  if ((dist == NULL) || (mask == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  angle = allocimage(NULL, rowsize(dist), colsize(dist), depth(dist), VFF_TYP_FLOAT);
  if (angle == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lbisector(dist, mask, angle))
    //  if (! lprintdownstream(dist))      pour les tests
  {
    fprintf(stderr, "%s: lbisector failed\n", argv[0]);
    exit(1);
  }

  writeimage(angle, argv[argc-1]);
  freeimage(dist);
  freeimage(mask);
  freeimage(angle);

  return 0;
} /* main */
