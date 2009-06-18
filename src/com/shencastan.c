/* $Id: shencastan.c,v 1.1 2009-06-18 06:34:55 mcouprie Exp $ */
/* \file shencastan.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Filtre dérivateur recursif de Shen et Castan */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lderiche.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  double beta;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm beta out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  beta = atof(argv[2]);

  if (! lshencastan(image1, beta))
  {
    fprintf(stderr, "%s: function lderiche failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


