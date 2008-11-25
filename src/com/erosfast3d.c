/* $Id: erosfast3d.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file erosfast3d.c

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

<B>erosfast3d</B> - morphological erosion by an elementary plane structuring element

SYNOPSIS

<B>erosfast3d</B> in.pgm el out.pgm

DESCRIPTION

The structuring element is specified by the value of the parameter <B>el</B>, 
which can be one of the following ones: vois26, vois18, vois6, vois4, vois8, vois2.

Types supported: byte 3D.

CLASS 

morph

*/

/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  uint8_t *mask = NULL;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm maskname out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  if (strcmp(argv[2], "vois6") == 0) mask = vois6; else
  if (strcmp(argv[2], "vois18") == 0) mask = vois18; else
  if (strcmp(argv[2], "vois26") == 0) mask = vois26; else
  {
    fprintf(stderr, "%s: bad maskname - use one of the following:\n", argv[0]);
    fprintf(stderr, "vois6, vois18, vois26\n");
    exit(1);
  }

  if (! lerosfast3d(image, mask))
  {
    fprintf(stderr, "%s: function lerosfast3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
