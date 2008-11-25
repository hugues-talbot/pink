/* $Id: max1.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file max1.c

\brief return the maximum of the pixel values of an image

<B>Usage:</B> max1 in.pgm out.list

<B>Description:</B>
This function returns (in the list <B>out.list</B>) 
the maximum of the pixel values of the image \b in.pgm .

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  double maxval;
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

  maxval = lmax1(image);

#ifdef VERBOSE
    printf("maxval: %g\n", maxval);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "s %d\n", 1); 
  fprintf(fd, "%d %g\n", 0, maxval); 
  fclose(fd);

  freeimage(image);

  return 0;
} /* main */
