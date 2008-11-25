/* $Id: tlf.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file tlf.c

\brief topological lower filter

<B>Usage:</B> tlf in.pgm connexmin r out.pgm

<B>Description:</B>
Topological lower filter. Performs the homotopic thickening controlled by
a radius <B>r</B>, followed by a well deletion, and a homotopic reconstruction over
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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

  if (! ltlf(image, connexmin, rayon))
  {
    fprintf(stderr, "%s: function ltlf failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
