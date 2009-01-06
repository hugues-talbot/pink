/* $Id: setvoxdim.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file setvoxdim.c

\brief adds voxel dimensions to file header

<B>Usage:</B> setvoxdim image xdim ydim zdim

<B>Description:</B> Adds voxel dimensions (\b xdim \b ydim \b zdim ) 
to file header.

<B>Types supported:</B> byte 3d, int32_t 3d

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s image xdim ydim zdim \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  image->xdim = atof(argv[2]);
  image->ydim = atof(argv[3]);
  image->zdim = atof(argv[4]);

  writeimage(image, argv[1]);
  freeimage(image);

  return 0;
} /* main */


