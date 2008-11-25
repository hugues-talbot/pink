/* $Id: erosplan.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file erosplan.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* operateur d'erosion numerique par un element structurant plan quelconque */
/* Michel Couprie - mars 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lerosplan.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * elem;
  int32_t x, y;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s f.pgm el.pgm x y out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  elem = readimage(argv[2]);
  if ((image == NULL) || (elem == NULL))
  {
    fprintf(stderr, "erosplan: readimage failed\n");
    exit(1);
  }
  
  x = atoi(argv[3]);
  y = atoi(argv[4]);
  if (! lerosplan(image, elem, x, y))
  {
    fprintf(stderr, "erosplan: function lerosplan failed\n");
    exit(1);
  }

  writeimage(image, argv[5]);
  freeimage(image);
  freeimage(elem);

  return 0;
} /* main */
