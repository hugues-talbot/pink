/* $Id: kern.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file kern.c

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
#include <lkern.h>

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

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "kern: readimage failed\n");
    exit(1);
  }

  if (! lkern(image, connex))
  {
    fprintf(stderr, "kern: lkern failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
