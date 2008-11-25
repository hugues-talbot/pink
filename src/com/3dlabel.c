/* $Id: 3dlabel.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file 3dlabel.c

\brief labels the theta-connected component of a 3D Khalimsky order

<B>Usage:</B> 3dlabel in.pgm out.pgm

<B>Description:</B>
Labels the theta-connected component of a 3D Khalimsky order.

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
  struct xvimage * k;
  struct xvimage * lab;
  
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  lab = allocimage(NULL, rowsize(k), colsize(k), depth(k), VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! l3dlabel(k, lab))
  {
    fprintf(stderr, "%s: function l3dlabel failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(lab, argv[2]);
  freeimage(k);
  freeimage(lab);

  return 0;
} /* main */
