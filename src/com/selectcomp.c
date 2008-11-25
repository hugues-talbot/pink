/* $Id: selectcomp.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file selectcomp.c

\brief selects a connected component

<B>Usage:</B> selectcomp in.pgm connex x y z out.pgm

<B>Description:</B>
The connected component of the binary image <B>in.pgm</B>
(according to the connectivity <B>connex</B>)
which contains the point (<B>x</B>,<B>y</B>,<B>z</B>)
is preserved. All other points are deleted.
Possible values for \b connex are 4, 8 (2D), 6, 18, 26, 60, 260 (3D).
Values 60 and 260 correspond to restrictions of 6 and 26 connectivities to the 
current xy plane.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t x, y, z, connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm connex x y z out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "selectcomp: readimage failed\n");
    exit(1);
  }
  connex = atoi(argv[2]);
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  z = atoi(argv[5]);
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    if (! lselectcomp(image, connex, x, y, z))
    {
      fprintf(stderr, "selectcomp: function ldeletecomp failed\n");
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "selectcomp: bad data type\n");
    exit(1);
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);
  return 0;
} /* main */


