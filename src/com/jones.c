/* $Id: jones.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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



