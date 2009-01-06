/* $Id: lambdalthin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file lambdalthin.c

\brief grayscale filtered leveling

<B>Usage:</B> lambdalthin in.pgm <imcond.pgm|null> connex niter h out.pgm

<B>Description:</B>
Filtered leveling for 2D grayscale images. 

<B>Types supported:</B> byte 2d

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llambdakern.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t connex, h, n;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm <imcond.pgm|null> niter connex h out.pgm\n", argv[0]);
    exit(1);
  }

  n = atoi(argv[3]);

  connex = atoi(argv[4]);
  if ((connex != 4) && (connex != 8))
  {
    fprintf(stderr, "%s : connex = <4|8>\n", argv[0]);
    exit(1);
  }

  h = atoi(argv[5]);

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! llambdathin(image, imagecond, n, connex, h))
  {
    fprintf(stderr, "%s: llambdathin failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */



