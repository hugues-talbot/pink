/* $Id: delcomp.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file delcomp.c

\brief deletes a connected component

<B>Usage:</B> delcomp in.pgm x y z v out.pgm

<B>Description:</B>
The connected component of the binary image <B>in.pgm</B>
which contains the point (<B>x</B>,<B>y</B>,<B>z</B>)
is deleted.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect, topobin
\ingroup  connect, topobin

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgeodesic.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t x, y, z, connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm connex x y z out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "delcomp: readimage failed\n");
    exit(1);
  }
  connex = atoi(argv[2]);
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  z = atoi(argv[5]);
  
  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    if (! ldelcomp(image1, connex, x, y, z))
    {
      fprintf(stderr, "delcomp: function ldelcomp failed\n");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "delcomp: bad data type\n");
    exit(1);
  }
  writeimage(image1, argv[argc-1]);
  freeimage(image1);
  return 0;
} /* main */


