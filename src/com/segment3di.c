/* $Id: segment3di.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file segment3di.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>segment3di</B> - interactive tool for 3D image segmentation

SYNOPSIS

<B>segment3di</B> in.pgm connex out.pgm

DESCRIPTION

Segmentation by selection of components in the component tree.
The parameter <B>connex</B> specifies the connexity (6, 18, 26).

Types supported: byte 3D.

CLASS 

morph ctree

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsegment3di.h>

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
    fprintf(stderr, "segment3di: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);

  if (! lsegment3di(image, connex))
  {
    fprintf(stderr, "segment3di: lsegment3di failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */



