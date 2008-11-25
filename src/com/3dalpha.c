/* $Id: 3dalpha.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3dalpha.c

\brief alpha-dilation of a 3d khalimsky order 

<B>Usage:</B> 3dalpha in.pgm out.pgm

<B>Description:</B>
Let X be the set of elements of the order \b in.pgm, 
the result is the union of alpha(x) for all x in X.

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/
/* Michel Couprie - juillet 2007 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[1]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! l3dalpha(k))
  {
    fprintf(stderr, "%s: function l3dalpha failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc-1]);
  freeimage(k);

  return 0;
} /* main */
