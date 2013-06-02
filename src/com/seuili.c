/* $Id: seuili.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file seuili.c

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
#include <lseuili.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "seuili: readimage failed\n");
    exit(1);
  }

  if (! lseuili(image))
  {
    fprintf(stderr, "seuili: function lseuili failed\n");
    exit(1);
  }

  writeimage(image, argv[2]);
  freeimage(image);

  return 0;
} /* main */
