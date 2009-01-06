/* $Id: skeleucl.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file skeleucl.c

\brief Euclidean binary skeleton

<B>Usage:</B> skeleucl in.pgm connex [inhibit] out.pgm

<B>Description:</B>
Eclidean binary skeleton.

The parameter \b connex indicates the connectivity of the binary object.
Possible choices are 4, 8 in 2d and 6, 26 in 3d.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

References:<BR> 
[CCZ07] M. Couprie, D. Coeurjolly and R. Zrour: <A HREF="http://www.esiee.fr/~coupriem/Pdf/ccz07.pdf">"Discrete bisector function and Euclidean skeleton in 2D and 3D"</A>, <I>Image and Vision Computing</I>, Vol.&nbsp;25, No.&nbsp;10, pp.&nbsp;1543-1556, 2007.<BR>

<B>Types supported:</B> byte 2d, byte 3d

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
#include <mcgeo.h>
#include <ldist.h>
#include <lskeletons.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * inhibit = NULL;
  int32_t connex;
  int32_t ret, priovalue;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm connex [inhibit] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (argc == 5)
  {
    inhibit = readimage(argv[3]);
    if (inhibit == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! lskeleucl(image, connex, inhibit))
  {
    fprintf(stderr, "%s: lskeleucl failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
