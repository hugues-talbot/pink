/* $Id: selrect.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file selrect.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* selection de pixels ayant un niveau de gris compris entre 2 bornes */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lselrect.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t x, y, w, h;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x y w h out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "selrect: readimage failed\n");
    exit(1);
  }
  
  x = atoi(argv[2]);
  y = atoi(argv[3]);
  w = atoi(argv[4]);
  h = atoi(argv[5]);

  if (! lselrect(image, x, y, w, h))
  {
    fprintf(stderr, "selrect: function lselndg failed\n");
    exit(1);
  }

  writeimage(image, argv[6]);
  freeimage(image);

  return 0;
} /* main */
