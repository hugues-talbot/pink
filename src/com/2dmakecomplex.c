/* $Id: 2dmakecomplex.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 2dmakecomplex.c

\brief makes a cellular complex from the given set

<B>Usage:</B> 2dmakecomplex in.pgm out.pgm

<B>Description:</B>
Makes a cellular complex from the given set, 
by performing the topological closure.

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - avril 2006 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
  struct xvimage * k;
  int32_t nsteps;
  
  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [i] out.pgm \n", argv[0]);
    exit(1);
  }
  
  if ((argc == 4) && (strcmp(argv[2],"i") != 0))
  {
    fprintf(stderr, "usage: %s in.pgm [i] out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    if (! l2dmakecomplex(k))
    {
      fprintf(stderr, "%s: function l2dmakecomplex failed\n", argv[0]);
      exit(1);
    }
  }
  else
    if (! l2dclosebeta(k))
    {
      fprintf(stderr, "%s: function l2dclosebeta failed\n", argv[0]);
      exit(1);
    }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
