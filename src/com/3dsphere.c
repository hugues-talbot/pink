/* $Id: 3dsphere.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* NOT SUPPORTED - EXPERIMENTAL */
/* \file 3dsphere.c

\brief Builds the discretization of a sphere in the 3d Khalimsky space.

<B>Usage:</B> 3dsphere in.pgm x0 y0 z0 radius out.pgm

<B>Description:</B>
  Draws into the Khalimsky volume \b k, the discretized sphere of center 
  \b x0, \b y0, \b z0 and of radius \b r.

<B>Types supported:</B> byte 3d

<B>Category:</B> topo 
\ingroup  topo 

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
  int32_t x0, y0, z0;
  double radius;  

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x0 y0 z0 radius out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  x0 = atoi(argv[2]);
  y0 = atoi(argv[3]);
  z0 = atoi(argv[4]);
  radius = atof(argv[5]);

  if (! l3dsphere(k, x0, y0, z0, radius))
  {
    fprintf(stderr, "%s: function l3dsphere failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc - 1]);
  freeimage(k);

  return 0;
} /* main */
