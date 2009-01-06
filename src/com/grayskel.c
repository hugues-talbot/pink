/* $Id: grayskel.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file grayskel.c

\brief grayscale homotopic skeleton

<B>Usage:</B> grayskel in.pgm <imcond.pgm|null> connex lambda out.pgm

<B>Description:</B>
Skeleton, either homotopic (<B>lambda</B> = 0) or non-homotopic, for 2D grayscale images
The skeleton is obtained by iterative lowering of lambda-destructible points
(see [CBB01]).

Reference:<BR> 
[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
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
  int32_t connex, lambda;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm <imcond.pgm|null> connex lambda out.pgm\n", argv[0]);
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

  if (! lgrayskel(image, imagecond, connex, lambda))
  {
    fprintf(stderr, "%s: lgrayskel failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */



