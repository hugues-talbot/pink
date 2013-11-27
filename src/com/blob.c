/* $Id: blob.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file blob.c

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
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lblob.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t niter;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm niter fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  niter = atoi(argv[2]);

  if (! lblob(image, niter))
  {
    fprintf(stderr, "%s: blob failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

