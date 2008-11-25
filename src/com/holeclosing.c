/* $Id: holeclosing.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file holeclosing.c

\brief 3d hole closing

<B>Usage:</B> holeclosing in connex holesize out

<B>Description:</B> 
Hole closing in 3d binary images. 
The parameter \b connex gives the connectivity used for the object;
possible choices are 6 and 26.
Holes which have a "size" greater than \b holesize are let open 
(where -1 is used as a symbol for infinity).

Let X be the set of points of the binary image \b in, 
let Y be a full enclosing box. 
The algorithm is the following:

\verbatim
Repeat until stability:
    Select a point p of Y \ X such that Tb(p,Y) = 1
        or such that Tb(p,Y) = 2 and d(p,X) > holesize
        which is at the greatest distance from X
    Y := Y \ {p}
Result: Y
\endverbatim

Reference: 
Z. Aktouf, G. Bertrand, L.Perroton: 
"A three-dimensional holes closing algorithm",
<I>Pattern Recognition Letters</I>, No.23, pp.523-531, 2002.

<B>Types supported:</B> byte 3d

<B>Category:</B> topobin
\ingroup  topobin

\author Michel Couprie
*/
/* Operateur de fermeture de trous en 3d */
/* Michel Couprie - decembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfermetrous3d.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;      /* image (binaire) de depart */
  struct xvimage * guide;      /* image (long) */
  int32_t connex;
  int32_t tailletrous;
  
  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm connex holesize [guide] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  connex = atoi(argv[2]);
  tailletrous = atoi(argv[3]);

  if (argc == 6)
  {
    guide = readimage(argv[4]);  
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
	
    if(rowsize(image)!=rowsize(guide) || colsize(image)!=colsize(guide) || 
       depth(image)!=depth(guide))
      {
  	fprintf(stderr, "%s: different size of input image and guide image\n", argv[0]);
  	exit(1);
    }

    if (! lfermetrous3dbin2(image, guide, connex, tailletrous))
    {
      fprintf(stderr, "%s: function lfermetrous3dbin2 failed\n", argv[0]);
      exit(1);
    }
    freeimage(guide);
  }
  else 
  {
    if (! lfermetrous3dbin(image, connex, tailletrous))
    {
      fprintf(stderr, "%s: function lfermetrous3dbin failed\n", argv[0]);
      exit(1);
    }
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
