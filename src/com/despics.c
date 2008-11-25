/* $Id: despics.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file despics.c

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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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

  if ((connexmin != 4) && (connexmin != 8))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask] <4|8> fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (! ldespics(image, mask, connexmin))
  {
    fprintf(stderr, "despics: function ldespics failed\n");
    exit(1);
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
