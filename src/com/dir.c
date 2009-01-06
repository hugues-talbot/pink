/* $Id: dir.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file dir.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* selectionne les points est, sud, ouest ou nord */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldir.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  int32_t dir;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm dir out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "dir: readimage failed\n");
    exit(1);
  }
  dir = atoi(argv[2]);

  if (! ldir(image1, dir))
  {
    fprintf(stderr, "dir: function ldir failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */


