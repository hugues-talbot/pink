/* $Id: 2dkhalimskize.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file 2dkhalimskize.c

\brief converts a 2D image into a 2D Khalimsky order

<B>Usage:</B> 2dkhalimskize in.pgm <4|8|h|m|M|g|G|a> out.pgm

<B>Description:</B>
Converts a 2D image into a 2D Khalimsky order, or conversely.

Modes <B>0</B>, <B>4</B>, <B>8</B> and <B>h</B> are for binary images. 

Modes <B>m</B>, <B>M</B>, <B>g</B>, <B>G</B> and <B>a</B> are for grayscale images. 

Mode = 
\li    0 : only maximal elements,
\li    4 : emulates 4-connectivity, idem "miss", 
\li    8 : emulates 8-connectivity ("hit" transform followed by a kernel transform),   
\li    h : "hit",
\li    m : min, 
\li    M : max,
\li    g : gradient 2, 
\li    G : gradient 4,
\li    a : average,
\li    R : reverse.

In reverse mode, only the beta-terminal elements (squares) are selected.

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* 
  Michel Couprie - avril 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky2d.h>
#include <l2dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * o;
  struct xvimage * k;
  int32_t mode;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm <0|4|8|h|m|M|g|G|a|R> out.pgm \n", argv[0]);
    exit(1);
  }

  o = readimage(argv[1]);  
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = argv[2][0];
  if ((mode != '0') && (mode != '4') && (mode != '8') && (mode != 'h') && (mode != 'm') &&
      (mode != 'M') && (mode != 'g') && (mode != 'G') && (mode != 'a') && (mode != 'R'))
  {
    fprintf(stderr, "usage: %s in.pgm <0|4|8|h|m|M|g|G|a|R> out.pgm \n", argv[0]);
    exit(1);
  }
  if (mode == 'h') mode = 10; else 
  if (mode == 'm') mode = 1; else 
  if (mode == 'M') mode = 2; else 
  if (mode == 'g') mode = 5; else 
  if (mode == 'G') mode = 6; else 
  if (mode == 'a') mode = 7; else 
  if (mode == 'R') mode = 9; else 
    mode -= '0';

  if (! l2dkhalimskize(o, &k, mode))
  {
    fprintf(stderr, "%s: function l2dkhalimskize failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[3]);
  freeimage(k);
  freeimage(o);

  return 0;
} /* main */
