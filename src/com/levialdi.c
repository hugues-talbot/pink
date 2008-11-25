/* $Id: levialdi.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file levialdi.c

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
#include <llevialdi.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex, n;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex n fileout.pgm\n", 
                    argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "levialdi: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  n = atoi(argv[3]);

#ifdef LEVIALDI
  if (! llevialdi(image, connex, n))
  {
    fprintf(stderr, "levialdi: llevialdi failed\n");
    exit(1);
  }
#else
  if (! lrao(image, connex, n))
  {
    fprintf(stderr, "levialdi: lrao failed\n");
    exit(1);
  }
#endif

  writeimage(image, argv[4]);
  freeimage(image);
  return 0;
} /* main */
