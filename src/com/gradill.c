/* $Id: gradill.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file gradill.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>gradill</B> - introduces an illumination gradient in an image

SYNOPSIS

<B>gradill</B> in.pgm alpha out.pgm

DESCRIPTION

The parameter <B>alpha</B> must be between 0.0 and 1.0.
The first row of <B>out.pgm</B> is equal to the first row of <B>in.pgm</B>.
The last row of <B>out.pgm</B> is equal to the last row of <B>in.pgm</B>
multiplied by a factor (1-alpha).
The other rows are multiplied by factors ranging linearly between 
1 and (1-alpha).

Types supported: byte 2d.

CLASS 

gene

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lgradill.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  double alpha;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm alpha out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "gradill: readimage failed\n");
    exit(1);
  }

  alpha = atof(argv[2]);
  if ((alpha <= 0) || (alpha > 1))
  {
    fprintf(stderr, "gradill: le parametre doit etre entre 0 et 1\n");
    exit(1);
  }

  if (! lgradill(image1, alpha))
  {
    fprintf(stderr, "gradill: function lgradill failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


