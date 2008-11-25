/* $Id: 3dkhalimskize.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file 3dkhalimskize.c

\brief converts a 3D image into a 3D Khalimsky order

<B>Usage:</B> 3dkhalimskize in.pgm <6|26|h|m|M|a> out.pgm

<B>Description:</B>
Converts a 3D image into a 3D Khalimsky order.

Modes <B>6</B>, <B>26</B> and <B>h</B> are for binary images. 

Modes <B>m</B>, <B>M</B> and <B>a</B> are for grayscale images. 

Mode = 
\li    6 : emulates 6-connectivity, idem "miss", 
\li    26 : emulates 26-connectivity ("hit" transform followed by a kernel transform),   
\li    h : "hit",
\li    m : min, 
\li    M : max,
\li    a : average.
\li    R : reverse.

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * o;
  struct xvimage * k;
  int32_t mode;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm <6|26|h|m|M|a|R> out.pgm \n", argv[0]);
    exit(1);
  }

  o = readimage(argv[1]);
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = argv[2][0];
  if ((mode != '6') && (mode != '2') && (mode != 'h') && (mode != 'm') && (mode != 'M') && (mode != 'a') && (mode != 'R'))
  {
    fprintf(stderr, "usage: %s in.pgm <6|26|h|m|M|a|R> out.pgm \n", argv[0]);
    exit(1);
  }
  if ((mode == '2') && (argv[2][1] != '6'))
  {
    fprintf(stderr, "usage: %s in.pgm <6|26|h|m|M|a> out.pgm \n", argv[0]);
    exit(1);
  }
  if (mode == 'h') mode = 0; else 
  if (mode == 'm') mode = 1; else 
  if (mode == 'M') mode = 2; else 
  if (mode == 'a') mode = 5; else 
  if (mode == 'R') mode = 6; else 
  if (mode == '2') mode = 3; else 
  if (mode == '6') mode = 4;

  if (! l3dkhalimskize(o, &k, mode))
  {
    fprintf(stderr, "%s: function l3dkhalimskize failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);
  freeimage(o);

  return 0;
} /* main */
