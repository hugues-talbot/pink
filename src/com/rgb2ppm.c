/* $Id: rgb2ppm.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file rgb2ppm.c

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

<B>rgb2ppm</B> - converts a truecolor non-compressed RGB file into a PPM file

SYNOPSIS

<B>rgb2ppm</B> in.rgb out.ppm

DESCRIPTION

The input file must be non-compressed RGB with 24 bits per pixel.

Types supported: rgb

CLASS 

convert

*/

/* 
  Michel Couprie - septembre 2001
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
    fprintf(stderr, "usage: %s in.rgb out.ppm \n", argv[0]);
    exit(1);
  }

  readrgb(argv[1], &g1, &g2, &g3);
  
  writergbimage(g1, g2, g3, argv[argc-1]);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
