/* $Id: symse.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file symse.c

\brief inverse of a structuring element

<B>Usage:</B> symse in.pgm out.pgm

<B>Description:</B>
Inverse of a structuring element.

<B>Types supported:</B> byte 2d, byte 3d.

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
   Michel Couprie  -  Mai 1998 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsym.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t x, y, z;
  int32_t rs, cs, ds;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readse(argv[1], &x, &y, &z);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image1);      /* taille ligne */
  cs = colsize(image1);      /* taille colonne */
  ds = depth(image1);        /* nb plans */

  if (! lsym(image1, 'c'))
  {
    fprintf(stderr, "%s: function lsym failed\n", argv[0]);
    exit(1);
  }

  writese(image1, argv[argc - 1], rs - 1 - x, cs - 1 - y, ds - 1 - z);
  freeimage(image1);

  return 0;
} /* main */


