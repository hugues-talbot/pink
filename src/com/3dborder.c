/* $Id: 3dborder.c,v 1.2 2009-09-02 14:23:36 mcouprie Exp $ */
/*! \file 3dborder.c

\brief extracts the border of an object in H3

<B>Usage:</B> 3dborder in.pgm out.pgm

<B>Description:</B>
Extracts the border of an object in H3. The (internal) border is defined as the set 
of the object points which have at least one neighboring background point. 

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

  if (! l3dborder(k))
  {
    fprintf(stderr, "%s: function l3dborder failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[argc - 1]);
  freeimage(k);

  return 0;
} /* main */
