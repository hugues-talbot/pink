/* $Id: bisector_talbot.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file bisector_talbot.c

\brief compute the bisector function of a binary image - Vincent-Talbot algorithm

<B>Usage:</B> bisector_talbot in.pgm out.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d 

<B>Category:</B> morpho
\ingroup  morpho

\author Laurent Najman, Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmedialaxis.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage *angles;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  angles = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_FLOAT);
  if (angles == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lbisector_talbot(image, angles))
  {
    fprintf(stderr, "%s: lbisector_talbot failed\n", argv[0]);
    exit(1);
  }

  writeimage(angles, argv[argc-1]);

  freeimage(image);
  freeimage(angles);
  return 0;
} /* main */
