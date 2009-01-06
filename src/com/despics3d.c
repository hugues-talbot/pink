/* $Id: despics3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file despics3d.c

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
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltrestopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask;
  int32_t connexmin;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask] connexmin fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "despics: readimage failed\n");
    exit(1);
  }

  if (argc == 5)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "despics: readimage failed\n");
      exit(1);
    }
    connexmin = atoi(argv[3]);
  }
  else
  {
    mask = NULL;
    connexmin = atoi(argv[2]);
  }

  if ((connexmin != 6) && (connexmin != 26))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask] <6|26> fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (! ldespics3d(image, mask, connexmin))
  {
    fprintf(stderr, "%s: function ldespics3d failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
