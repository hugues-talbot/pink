/* $Id: labeltree.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file labeltree.c

\brief branch distance

<B>Usage:</B> labeltree in.pgm connex out.pgm

<B>Description:</B>
Level of the branch section defined by his level in the tree. Returns the level
max of the branches.

The distance d used depends on the parameter \b connex :
\li 4: 4-distance in 2d
\li 8: 8-distance in 2d
\li 6: 6-distance in 3d
\li 18: 18-distance in 3d
\li 26: 26-distance in 3d

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> topo
\ingroup   topo

\author Cédric Allène
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabeltree.h>

/* =============================================================== */
int main(argc, argv) 
/* /include/llabeltree.h */
/* /src/lib/llabeltree.c */
/* /src/com/labeltree.c */
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t connex;
  struct xvimage * image;
  struct xvimage * result;
  
  int32_t d_max = 0;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm connex out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  if ((connex != 4) && (connex != 8) && (connex != 6) && (connex != 18) && (connex != 26))
  {
    fprintf(stderr, "usage: %s in.pgm <4|8|6|18|26> fileout.pgm\n", argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  d_max = llabeltree(image, connex, result);
  if (! d_max)
  {
    fprintf(stderr, "%s: llabeltree failed\n", argv[0]);
    exit(1);
  }
  
  printf ("%d\n", d_max);
  
  result->name = NULL;
  writeimage(result, argv[argc-1]);
  freeimage(image);
  freeimage(result);

  return 0;
} /* main */


