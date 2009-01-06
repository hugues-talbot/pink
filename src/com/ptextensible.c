/* $Id: ptextensible.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ptextensible.c

\brief detects extensible points in a grayscale image

<B>Usage:</B> ptextensible im.pgm <imcond.pgm|null> connex fileout.pgm

<B>Description:</B>
Extensible point detection.
The input image \b im.pgm must be a "thin" grayscale image, as
the output of the operator \b hthin.
The parameter \b connex indicates the cconnectivity used for the minimal 
regions.
The optional parameter \b imcond.pgm is a binary image which indicates the points
in the neighborhood of which the extensible points will be searched.

Reference:<BR> 
M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
grayscale image processing"</A>, <I> Journal of Electronic Imaging</I>, Vol.&nbsp;10, No.&nbsp;4, pp.&nbsp;1003-1015, 2001.

<B>Types supported:</B> byte 2D

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
#include <lhtkern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
  {
    imagecond = NULL;
  }
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lptextensible(image, imagecond, connex))
  {
    fprintf(stderr, "ptextensible: lptextensible failed\n");
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
