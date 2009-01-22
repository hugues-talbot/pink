/* $Id: text.c,v 1.2 2009-01-22 07:05:36 mcouprie Exp $ */
/* \file drawtext.c

\brief Draws a text line in a 2D image

<B>Usage:</B> drawtext in.pgm x y text col out.pgm

<B>Description:</B>
Draws a text line in image <B>in.pgm</B>, at position 
<B>x</B>, <B>y</B>,  and with color <B>col</B> (in the range 0...1).

<B>Types supported:</B> byte 2d

<B>Category:</B> draw
\ingroup  draw

\author Michel Couprie
*/

/* Michel Couprie - février 2007 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcdrawps.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * image;
  int32_t rs, cs, N;


NOT YET WRITTEN


  return 0;
} /* main */

