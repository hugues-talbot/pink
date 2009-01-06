/* $Id: squel.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file squel.c

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
#include <lsquel.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t seuil;
  int32_t niseuil;
  char * nameout;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm seuil niseuil fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "squel: readimage failed\n");
    exit(1);
  }

  seuil = atoi(argv[2]);
  niseuil = atoi(argv[3]);

  if (! lsquel(image, seuil, niseuil))
  {
    fprintf(stderr, "squel: lsquel failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
