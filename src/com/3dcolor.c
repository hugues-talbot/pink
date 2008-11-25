/* $Id: 3dcolor.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3dcolor.c

\brief Gives a specific color to points in a 3D khalimsky grid,
according to their rank

<B>Usage:</B> 3dcolor in.pgm out.pgm

<B>Description:</B>
Gives a specific color to points in a 3D khalimsky grid,
according to their rank

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

  if (! l3dcolor(k))
  {
    fprintf(stderr, "%s: function l3dcolor failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[2]);
  freeimage(k);

  return 0;
} /* main */
