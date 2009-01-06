/* $Id: ccv.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file ccv.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* operateur de calcul du carre du coefficient de variation du niveau de gris
   dans le voisinage d'un point */
/* Michel Couprie - avril 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lccv.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * elem;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readimage(argv[2]);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "ccv: readimage failed\n");
    exit(1);
  }
  
  if (! lccv(image, elem))
  {
    fprintf(stderr, "ccv: function lccv failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
