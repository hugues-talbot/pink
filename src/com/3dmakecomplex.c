/* $Id: 3dmakecomplex.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3dmakecomplex.c

\brief makes a cellular complex from the given set

<B>Usage:</B> 3dmakecomplex in.pgm out.pgm

<B>Description:</B>
Makes a cellular complex from the given set, 
by performing the topological closure.

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - juillet 2007 */

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

  if (! l3dmakecomplex(k))
  {
    fprintf(stderr, "%s: function l3dmakecomplex failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
