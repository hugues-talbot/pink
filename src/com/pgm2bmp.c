/* $Id: pgm2bmp.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file pgm2bmp.c

\brief converts a PGM file into a truecolor non-compressed BMP file

<B>Usage:</B> pgm2bmp in.pgm out.bmp

<B>Description:</B>

Converts a PGM file into a truecolor non-compressed BMP file.
The output file will be non-compressed BMP with 24 bits per pixel.

<B>Types supported:</B> byte 2D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * g1;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.bmp \n", argv[0]);
    exit(1);
  }

  g1 = readimage(argv[1]);
  
  writebmp(g1, g1, g1, argv[argc-1]);
  freeimage(g1);

  return 0;
} /* main */
