/* $Id: lambdaskel.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file lambdaskel.c

\brief grayscale filtered topological skeleton

<B>Usage:</B> lambdaskel in.pgm <imcond.pgm|null> connex lambda out.pgm

<B>Description:</B>
Filtered topological skeleton for 2D grayscale images. 
The parameter \b connex gives the connectivity used for the minima;
possible choices are 4 and 8.
The parameter \b lambda is a contrast parameter (positive integer).
Let F be the function corresponding to the input image \b in.pgm.
The parameter \b imcond.pgm is a constraint function G.
The algorithm is the following:

\verbatim
Repeat until stability:
    Select a point p which is lambda-destructible for F or a peak
        such that F(p) > G(p) and such that F(p) is minimal
    F(p) = alpha-(p,F)
Result: F
\endverbatim

Reference:<BR> 
M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.


<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llambdakern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t lambda;
  int32_t connex;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s filein.pgm <imcond.pgm|null> connex lambda fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  lambda = atoi(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! llambdakern(image, imagecond, connex, lambda))
  {
    fprintf(stderr, "%s: llambdakern failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
