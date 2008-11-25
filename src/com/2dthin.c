/* $Id: 2dthin.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 2dthin.c

\brief homotopic thinning of a 2d Khalimsky image

<B>Usage:</B> 2dthin in.pgm n out.pgm

<B>Description:</B>
Homotopic thinning of a 2d Khalimsky image.
The maximum number of steps is given by <B>n</B>.
If the value given for \b n equals -1, the thinning is continued
until stability.

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

/* Michel Couprie - avril 2000 */

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
  int32_t nsteps;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm n out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);

  if (! l2dthin(k, nsteps))
  {
    fprintf(stderr, "%s: function l2dthin failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[3]);
  freeimage(k);

  return 0;
} /* main */
