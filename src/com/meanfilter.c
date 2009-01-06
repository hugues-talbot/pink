/* $Id: meanfilter.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file meanfilter.c

\brief Mean filter operator

<B>Usage:</B> meanfilter in.pgm roi.pgm connex niter [inhibit.pgm] out.pgm

<B>Description:</B>
Let F be the function stored in image \b in.pgm.
Let R be the function stored in image \b roi.pgm (region of interest).
Let I be the function stored in image \b inhibit.pgm (if any).
The algorithm is the following:

\verbatim
Repeat niter times
  G = F
  For each image point x
    If R[x] and not I[x] then
      S = G[x]; C = 1;
      For each connex-neighbour y of x
        If R[y] then S = S + G[y]; C = C + 1;
      F[x] = S / C;
Result: F
\endverbatim

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d, byte 3d, int32_t 3d, float 3d

<B>Category:</B> signal
\ingroup  signal

\author Marcin Janaszewski, Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmean.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * roi;
  struct xvimage * inhibit = NULL;
  int32_t connex;
  int32_t niter;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm roi.pgm connex niter [inhibit.pgm] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  roi = readimage(argv[2]);
  if ((image == NULL) || (mask == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[3]);
  niter = atoi(argv[4]);

  if (argc == 7) 
  {
    inhibit = readimage(argv[5]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }    
  }

  if (! convertfloat(&image))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }
  
  if (! lmeanfilter(image, roi, inhibit, connex, niter))
  {
    fprintf(stderr, "%s: function lmeanfilter failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(roi);
  if (inhibit) freeimage(inhibit);
  return 0;
} /* main */


