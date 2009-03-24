/* $Id: average1.c,v 1.2 2009-03-24 14:50:25 mcouprie Exp $ */
/*! \file average1.c

\brief return the average of the pixel values of an image

<B>Usage:</B> average1 in.pgm [mask.pgm] out.list

<B>Description:</B>
This function returns (in the list <B>out.list</B>) 
the average of the pixel values of the image \b in.pgm .
If the optional parameter \b mask.pgm is given, then only the 
values which correspond to non-null points of mask are averaged.

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

//#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  double averageval;
  FILE *fd = NULL;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [mask.pgm] out.list\n", argv[0]);
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
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    averageval = laverage2(image, mask);
  }
  else
    averageval = laverage1(image);

#ifdef VERBOSE
    printf("averageval: %g\n", averageval);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "s %d\n", 1); 
  fprintf(fd, "%d %g\n", 0, averageval); 
  fclose(fd);

  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
