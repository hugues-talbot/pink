/* $Id: comphisto.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file comphisto.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* compression d'histogramme */
/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lcomphisto.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t n;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm n out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "comphisto: readimage failed\n");
    exit(1);
  }
  
  n = atoi(argv[2]);

  if (! lcomphisto(image, n))
  {
    fprintf(stderr, "comphisto: function lcomphisto failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */
