/* $Id: segmentlignes.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file segmentlignes.c

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
#include <lsegment.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  int32_t seuilbas;
  int32_t seuilhaut;

  if (argc != 6)
  {
    fprintf(stderr, 
    "usage: %s filein.pgm connex seuilbas seuilhaut fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segment: readimage failed\n");
    exit(1);
  }
  connex = atoi(argv[2]);
  seuilbas = atoi(argv[3]);
  seuilhaut = atoi(argv[4]);
  
  if (! lsegmentlignes(image, connex, seuilbas, seuilhaut))
  {
    fprintf(stderr, "segment: lsegmentligne failed\n");
    exit(1);
  }
  
  writeimage(image, argv[5]);
  freeimage(image);

  return 0;
} /* main */



