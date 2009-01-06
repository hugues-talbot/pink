/* $Id: skel_NK2.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file skel_NK2.c

\brief parallel 2D binary curvilinear skeleton

<B>Usage:</B> skel_NK2 in.pgm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel 2D binary thinning or curvilinear skeleton. The parameter \b nsteps gives,
if positive, the number of parallel thinning steps to be processed.
If the value given for \b nsteps equals -1, the thinning is continued
until stability.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Types supported:</B> byte 2d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lskelpar.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t ret, nsteps;
  int32_t filter = 0;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm nsteps [inhibit] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);
  if (argc == 5)
  {
    inhibit = readimage(argv[3]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (depth(image) == 1)
  {
    if (! lskelNK2(image, nsteps, inhibit))
    {
      fprintf(stderr, "%s: lskelNK2 failed\n", argv[0]);
      exit(1);
    } 
  }
  else
  {
    fprintf(stderr, "%s: image must be 2D\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
