/* $Id: 3dinvariants.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3dinvariants.c

\brief computes the numbers of connected components,
cavities and tunnels of a 3D Khalimsky order

<B>Usage:</B> 3dinvariants in.pgm

<B>Description:</B>
Computes the numbers of connected components,
cavities and tunnels of a 3D Khalimsky order

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - avril 2001 */

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
  int32_t nbcc, nbcav, nbtun, euler; 

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

  if (! l3dinvariants(k, &nbcc, &nbcav, &nbtun, &euler))
  {
    fprintf(stderr, "%s: function l3dinvariants failed\n", argv[0]);
    exit(1);
  }

  printf("car. Euler = %d\n", euler);
  printf("%d composantes connexes, ", nbcc);
  printf("%d cavites, ", nbcav);
  printf("%d tunnels\n", nbtun);
  
  freeimage(k);

  return 0;
} /* main */
