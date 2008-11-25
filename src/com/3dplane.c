/* $Id: 3dplane.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3dplane.c

\brief Builds the discretization of a plane in the 3d Khalimsky space.

<B>Usage:</B> 3dplane in.pgm a b c d out.pgm

<B>Description:</B>
  Draws into the Khalimsky volume \b k, the discretization of the plane
  defined by the equation: \b a x + \b b y + \b c z + \b d = 0

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
  double a, b, c, d;  

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm a b c d out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  a = atof(argv[2]);
  b = atof(argv[3]);
  c = atof(argv[4]);
  d = atof(argv[5]);

  if (! l3dplane(k, a, b, c, d))
  {
    fprintf(stderr, "%s: function l3dplane failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc - 1]);
  freeimage(k);

  return 0;
} /* main */
