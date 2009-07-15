/* $Id: 2dpardircollapse.c,v 1.2 2009-07-15 05:31:01 mcouprie Exp $ */
/*! \file 2dpardircollapse.c

\brief parallel directional constrained collapse

<B>Usage:</B> 2dpardircollapse in.pgm nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel directional constrained collapse.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky2d.h>
#include <mcgraphe.h>
#include <l2dcollapse.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * inhibimage = NULL;
  int32_t nsteps;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm nsteps [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);

  if (argc == 5) 
  {
    inhibimage = readimage(argv[3]);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (! l2dpardircollapse(k, nsteps, inhibimage))
  {
    fprintf(stderr, "%s: function l2dpardircollapse failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
