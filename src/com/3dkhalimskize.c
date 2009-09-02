/* $Id: 3dkhalimskize.c,v 1.3 2009-09-02 14:23:36 mcouprie Exp $ */
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

<B>Types supported:</B> byte 3d, long 3d, float 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/*
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm 6 %RESULTS/3dkhalimskize_b3a_6.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm 26 %RESULTS/3dkhalimskize_b3a_26.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.pgm h %RESULTS/3dkhalimskize_b3a_h.k
%TEST 3dkhalimskize %IMAGES/3dbyte/gray/g3a.pgm m %RESULTS/3dkhalimskize_g3a_m.k
%TEST 3dkhalimskize %IMAGES/3dlong/l3a.pgm M %RESULTS/3dkhalimskize_l3a_M.k
%TEST 3dkhalimskize %IMAGES/3dfloat/f3a.pgm a %RESULTS/3dkhalimskize_f3a_a.k
%TEST 3dkhalimskize %IMAGES/3dbyte/binary/b3a.k R %RESULTS/3dkhalimskize_b3a_R.pgm
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
