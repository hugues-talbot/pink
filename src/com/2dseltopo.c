/* $Id: 2dseltopo.c,v 1.1 2009-06-24 05:32:12 mcouprie Exp $ */
/*! \file 2dseltopo.c

\brief selects points in the 2d khalimsky grid according to a local topological criterion 

<B>Usage:</B> 2dseltopo in d- d+ a- a+ b- b+ out

<B>Description:</B>
  Selects the elements x of the Khalimsky object K
  which satisfy the following inequalities : 

  \b d1 <= d(x) <= \b d2

  \b a1 <= a(x) <= \b a2

  \b b1 <= b(x) <= \b b2

  where

  d(x) = dimension of x

  a(x) = number of elements under x of dimension d(x) - 1

  b(x) = number of elements over x of dimension d(x) + 1

<B>Types supported:</B> byte 2d

<B>Category:</B> orders
\ingroup orders

\author Michel Couprie
*/

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
  struct xvimage * image;
  int32_t dm, dp, am, ap, bm, bp;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s in d- d+ a- a+ b- b+ out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  dm = atoi(argv[2]);
  dp = atoi(argv[3]);
  am = atoi(argv[4]);
  ap = atoi(argv[5]);
  bm = atoi(argv[6]);
  bp = atoi(argv[7]);

  if (! l2dseltype(image, dm, dp, am, ap, bm, bp))
  {
    fprintf(stderr, "%s: function l2dseltype failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */