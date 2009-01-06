/* $Id: simplepair.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file simplepair.c

\brief detects simple pairs in a 3D binary image

<B>Usage:</B> simplepair in.pgm [all] out.pgm

<B>Description:</B>
Detects simple pairs in a 3D binary image. If the optional parameter \b all 
is used, then all simple pairs are searched. Otherwise, only one pair (if
any) is given.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie 2008
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lseltopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [all] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 4) && (strcmp(argv[2], "all") != 0))
  {
    fprintf(stderr, "usage: %s filein.pgm [all] fileout.pgm\n", argv[0]);
    exit(1);
  }


  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    if (! lsimplepair(image, 0))
    {
      fprintf(stderr, "%s: function lsimplepair failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! lsimplepair(image, 1))
    {
      fprintf(stderr, "%s: function lsimplepair failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


