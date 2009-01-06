/* $Id: segmentnumi.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file segmentnumi.c

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
#include <lsegmentnumi.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s filein.pgm fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentnumi: readimage failed\n");
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "segmentnumi: allocimage failed\n");
    exit(1);
  }

  if (! lsegmentnumi(image, result))
  {
    fprintf(stderr, "segmentnumi: lsegmentnumi failed\n");
    exit(1);
  }

  writelongimage(result, argv[2]);
  freeimage(image);
  freeimage(result);
  return 0;
} /* main */



