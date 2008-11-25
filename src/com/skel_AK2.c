/* $Id: skel_AK2.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file skel_AK2.c

\brief parallel 2D binary curvilinear skeleton

<B>Usage:</B> skel_AK2 in.pgm nsteps [inhibit] out.pgm

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
    if (! lskelAK2(image, nsteps, inhibit))
    {
      fprintf(stderr, "%s: lskelAK2 failed\n", argv[0]);
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
