/* $Id: lambdakern.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file lambdakern.c

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
#include <llambdakern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t lambda;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex lambda fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  lambda = atoi(argv[3]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "lambdakern: readimage failed\n");
    exit(1);
  }

  if (! llambdakern(image, lambda, connex))
  {
    fprintf(stderr, "lambdakern: llambdakern failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
