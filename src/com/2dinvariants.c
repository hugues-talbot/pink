/* $Id: 2dinvariants.c,v 1.1 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file 2dinvariants.c

\brief computes the numbers of connected components,
cavities and tunnels of a 2D Khalimsky order

<B>Usage:</B> 2dinvariants in.pgm

<B>Description:</B>
Computes the numbers of connected components
and holes of a 2D Khalimsky order

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - août 2009 */

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
  struct xvimage * k;
  int32_t nbcc, nbcav, euler; 

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s in.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! l2dinvariants(k, &nbcc, &nbcav, &euler))
  {
    fprintf(stderr, "%s: function l2dinvariants failed\n", argv[0]);
    exit(1);
  }

  printf("car. Euler = %d\n", euler);
  printf("%d composantes connexes, ", nbcc);
  printf("%d trous\n", nbcav);
  
  freeimage(k);

  return 0;
} /* main */
