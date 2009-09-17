/* $Id: skel_EK3.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file skel_EK3.c

\brief parallel 3D binary curvilinear skeleton based on ends

<B>Usage:</B> skel_EK3 in.pgm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel 3D binary thinning or curvilinear skeleton based on ends. 
The parameter \b nsteps gives,
if positive, the number of parallel thinning steps to be processed.
If the value given for \b nsteps equals -1, the thinning is continued
until stability.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Warning:</B> The object must not have any point on the frame of the image.

<B>Types supported:</B> byte 3d

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
#include <lskelpar3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t nsteps;

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

  if (depth(image) != 1)
  {
    if (! lskelEK3(image, nsteps, inhibit))
    {
      fprintf(stderr, "%s: lskelEK3 failed\n", argv[0]);
      exit(1);
    } 
  }
  else
  {
    fprintf(stderr, "%s: image must be 3D\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
