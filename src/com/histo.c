/* $Id: histo.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file histo.c

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
#include <lhisto.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  uint32_t * histo;
  int32_t i;
  FILE *fd = NULL;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask.pgm] fileout\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  histo = (uint32_t *)calloc(1,(NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
  if (histo == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    fd = fopen(argv[2],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
      exit(1);
    }
  }
  else
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    fd = fopen(argv[3],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[3]);
      exit(1);
    }
  }

  if (! lhisto(image, mask, histo))
  {
    fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
    exit(1);
  }

  /* ESSAI
  { int32_t n = lhistsum(histo);
    lhistdilat(histo, 4);
    printf("sum avant dilat = %d, apres = %d\n", n, lhistsum(histo));
  }
  */

  for (i = NDG_MIN; i <= NDG_MAX; i++) fprintf(fd, "%4d %d\n", i, histo[i]);

  fclose(fd);
  free(histo);
  freeimage(image);
  if (mask) freeimage(mask);

  return 0;
} /* main */
