/* $Id: htkern3d.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */

OBSOLETE - voir htkern.c

/* \file htkern3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhtkern3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imagecond;
  int32_t connexmin;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s im.pgm <imcond.pgm|null> connexmin fileout.pgm\n", argv[0]);
    exit(1);
  }

  connexmin = atoi(argv[3]);
  if ((connexmin != 6) && (connexmin != 26))
  {
    fprintf(stderr, "%s : connexmin = <6|26>\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "htkern3d: readimage failed\n");
    exit(1);
  }

  if (strcmp(argv[2],"null") == 0) 
    imagecond = NULL;
  else
  {
    imagecond = readimage(argv[2]);
    if (imagecond == NULL)
    {
      fprintf(stderr, "htkern3d: readimage failed\n");
      exit(1);
    }
  }

  if (! lhtkern3d(image, imagecond, connexmin))
  {
    fprintf(stderr, "htkern3d: lhtkern3d failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);
  if (imagecond != NULL) freeimage(imagecond);

  return 0;
} /* main */
