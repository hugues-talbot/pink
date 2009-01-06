/* $Id: tuf.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file tuf.c

\brief topological upper filter

<B>Usage:</B> tuf in.pgm connexmin r out.pgm

<B>Description:</B>
Topological upper filter. Performs the homotopic thinning controlled by
a radius <B>r</B>, followed by a peak deletion, and a homotopic reconstruction under
the original image.

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
/* 
   Filtre topologique 

   Michel Couprie - janvier 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltrestopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayon;
  int32_t connexmin;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm connexmin rayon out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connexmin = atoi(argv[2]);
  rayon = atoi(argv[3]);

  if (! ltuf(image, connexmin, rayon))
  {
    fprintf(stderr, "%s: function ltuf failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
