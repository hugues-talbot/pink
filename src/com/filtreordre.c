/* $Id: filtreordre.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file filtreordre.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* filtre d'ordre sur un voisinage quelconque */
/* Michel Couprie - decembre 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfiltreordre.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y, z;
  double r;

  if ((argc != 7) && (argc != 8))
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm x y [z] r out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readimage(argv[2]);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "filtreordre: readimage failed\n");
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  if (argc == 7)
    r = atof(argv[5]);
  else
  {
    z = atof(argv[5]);
    r = atof(argv[6]);
  }
  if ((r < 0.0) || (r > 1.0))
  {
    fprintf(stderr, "filtreordre: r = %g ; on doit avoir 0 <= r <= 1\n", r);
    exit(1);
  }
  if (argc == 7)
  {
    if (! lfiltreordre(image, elem, x, y, r))
    {
      fprintf(stderr, "filtreordre: function lfiltreordre failed\n");
      exit(1);
    }
  }
  else
  {
    if (! lfiltreordre3d(image, elem, x, y, z, r))
    {
      fprintf(stderr, "filtreordre: function lfiltreordre failed\n");
      exit(1);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
