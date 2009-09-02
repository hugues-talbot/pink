/* $Id: 3dsurfacecollapse.c,v 1.2 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file 3dsurfacecollapse.c

\brief parallel directional constrained collapse with surface detection and preservation

<B>Usage:</B> 3dsurfacecollapse in.pgm mode nsteps [inhibit] out.pgm

<B>Description:</B>
Parallel directional constrained collapse with surface detection and preservation.

The parameter \b mode selects the strategy used to detect surface elements.
The possible choices are:
\li 1: any 2-dimensional facet

The parameter \b nsteps gives the number of "layers" to be removes, if
the value is -1 then the interations continue until stability.

If the parameter \b inhibit is given and is a binary image name,
then the points of this image will be left unchanged. 

<B>Types supported:</B> byte 3d

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
#include <mckhalimsky3d.h>
#include <mcgraphe.h>
#include <l3dcollapse.h>

/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  struct xvimage * inhibimage = NULL;
  int32_t mode;
  int32_t nsteps;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm mode nsteps [inhibit] out.pgm\n", 
                    argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  mode = atoi(argv[2]);
  nsteps = atoi(argv[3]);

  if (argc == 6) 
  {
    inhibimage = readimage(argv[4]);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    inhibimage = allocimage(NULL, rowsize(k), colsize(k), depth(k), VFF_TYP_1_BYTE);
    if (inhibimage == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (mode == 1)
  {
    if (! l3dsurfacecollapse(k, nsteps, inhibimage))
    {
      fprintf(stderr, "%s: function l3dsurfacecollapse failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    fprintf(stderr, "%s: bad mode %d\n", argv[0], mode);
    exit(1);
  }

  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
