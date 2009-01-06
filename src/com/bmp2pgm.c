/* $Id: bmp2pgm.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file bmp2pgm.c

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

<B>bmp2pgm</B> - converts a truecolor non-compressed BMP file into a (grayscale) PGM file

SYNOPSIS

<B>bmp2pgm</B> in.bmp out.pgm

DESCRIPTION

The input file must be non-compressed BMP with 24 bits per pixel.
If the original image contains color data, then the green band is extracted.

Types supported: bmp

CLASS 

convert

*/

/* 
  Michel Couprie - octobre 2000
*/

#include<stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include<stdlib.h>
#include<string.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.bmp out.pgm \n", argv[0]);
    exit(1);
  }

  readbmp(argv[1], &g1, &g2, &g3);
  
  writeimage(g2, argv[argc-1]);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
