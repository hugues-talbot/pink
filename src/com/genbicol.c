/* $Id: genbicol.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file genbicol.c

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

<B>genbicol</B> - generates a bicolor image

SYNOPSIS

<B>genbicol</B> col1 col2 out.pgm

DESCRIPTION

The result image <B>out.pgm</B> is a square which contains four pixels. 
The pixels of the first column have the color <B>col1</B>.
The pixels of the second column have the color <B>col2</B>.

Types supported: byte 2d.

CLASS 

gene

*/

/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i;
  uint8_t *Im;
  int32_t rs, cs;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s col1 col2 out.pgm \n", argv[0]);
    exit(1);
  }

  rs = 2;
  cs = 2;
  image = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  Im = UCHARDATA(image);
  Im[0] = atoi(argv[1]);
  Im[1] = atoi(argv[2]);
  Im[2] = atoi(argv[1]);
  Im[3] = atoi(argv[2]);

  writeimage(image, argv[3]);
  freeimage(image);

  return 0;
} /* main */

