/* $Id: 3dthin.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file 3dthin.c

\brief homotopic thinning of a 3d Khalimsky image

<B>Usage:</B> 3dthin in.pgm n out.pgm

<B>Description:</B> Homotopic thinning of a 3d Khalimsky image.
The maximum number of steps is given by <B>n</B>.
If the value given for \b n equals -1, the thinning is continued
until stability.

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

  if (! l3dthin(k, nsteps))
  {
    fprintf(stderr, "%s: function l3dthin failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
