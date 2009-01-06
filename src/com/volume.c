/* $Id: volume.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file volume.c

\brief return the sum of the pixel values of an image

<B>Usage:</B> volume in.pgm out.list

<B>Description:</B>
This function returns (in the list <B>out.list</B>) 
the sum of the pixel values of the image \b in.pgm .

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d, float 2d, float 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  double vol;
  FILE *fd = NULL;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.list\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (!lvolume(image, &vol))
  {
    fprintf(stderr, "%s: lvolume failed\n", argv[0]);
    exit(1);
  }  

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "s %d\n", 1); 
  fprintf(fd, "%d %g\n", 0, vol); 
  fclose(fd);

  freeimage(image);

  return 0;
} /* main */
