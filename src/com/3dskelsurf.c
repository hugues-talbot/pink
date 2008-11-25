/* $Id: 3dskelsurf.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file 3dskelsurf.c

\brief surfacic skeleton

<B>Usage:</B> in.pgm n out.pgm

<B>Description:</B> Surfacic skeleton in the sense defined in ref. [LB99]
The parameter \b n gives the number of parallel steps of simple,
non surface end point deletions.

Reference:<BR> 
[LB99] C. Lohou, Gilles Bertrand: <A HREF="http://www.esiee.fr/~info/christophe/spie99lb.ps.gz">"Poset approach to 3D parallel thinning"</A>, <I>SPIE Vision Geometry VIII</I>, Vol.&nbsp;3811, pp.&nbsp;45-56, 1999.

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders


\author Michel Couprie
*/
/* Michel Couprie - mai 2000 */

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

  if (! l3dskelsurf(k, nsteps))
  {
    fprintf(stderr, "%s: function l3dskelsurf failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[3]);
  freeimage(k);

  return 0;
} /* main */
