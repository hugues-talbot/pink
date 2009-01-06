/* $Id: jones.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file jones.c

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
#include <ljones.h>

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
    fprintf(stderr, "jones: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  
  if (! ljones(image, connex))
  {
    fprintf(stderr, "jones: ljones failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */



