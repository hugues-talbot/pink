/* $Id: crestrestoration.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file crestrestoration.c

\brief crest restoration algorithm 

<B>Usage:</B> crestrestoration im.pgm <imcond.pgm|null> connex niter fileout.pgm [condout.pgm]

<B>Description:</B>
Crest restoration algorithm, as described in ref. CBB01.
The input image \b im.pgm must be a "thin" grayscale image, as
the output of the operator \b hthin.
The parameter \b connex indicates the connectivity used for the minimal 
regions.
The parameter \b niter gives the number of iterations.
The optional parameter \b imcond.pgm is a binary image (a set C) which indicates the points
in the neighborhood of which the extensible points will be searched.
The points which are modified by the algorithm will be dynamically added to C.
The optional parameter \b condout.pgm is an output file containing the final state of the 
set C.

Reference:<BR> 
[CBB01] M. Couprie, F.N. Bezerra, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/a2si/Ps/elimag01.ps.gz">"Topological operators for
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
  int32_t nitermax;
  int32_t connex;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connex niter fileout.pgm [condout.pgm]\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  nitermax = atoi(argv[4]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
  {
    if (argc == 7)
    {
      fprintf(stderr, "%s: cannot generate condout.pgm when imcond is null\n", argv[0]);
      exit(1);
    }
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

  if (! lcrestrestoration(image, imagecond, nitermax, connex))
  {
    fprintf(stderr, "crestrestoration: lcrestrestoration failed\n");
    exit(1);
  }

  if (argc == 6) 
    writeimage(image, argv[argc - 1]);
  else
  {
    writeimage(imagecond, argv[argc - 1]);
    writeimage(image, argv[argc - 2]);
  }
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
