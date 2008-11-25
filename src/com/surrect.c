/* $Id: surrect.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file surrect.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeo.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t connex;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm connex out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);

  if (! lrectangleincluant(image1, connex))
  {
    fprintf(stderr, "%s: function lmatchrect failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


