/* $Id: wshedval.c,v 1.3 2009-03-16 15:52:23 mcouprie Exp $ */
/* \file wshedval.c

\brief watershed with valuation by the geodesic saliency

<B>Usage:</B> wshedval in.pgm connex out.pgm

<B>Description:</B>
Watershed with valuation by the geodesic saliency 
- connectivity <B>connex</B>
(Ref: Najman).

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldynamique_grimaud.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (! lwshedval(image, connex))
  {
    fprintf(stderr, "%s: lwshedval failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
  return 0;
} /* main */



