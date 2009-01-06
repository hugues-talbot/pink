/* $Id: areaclosing.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file areaclosing.c

\brief area closing

<B>Usage:</B> areaclosing in.pgm connex area out.pgm

<B>Description:</B>
Area closing with connexity <B>connex</B> and area <B>area</B>.
Deletes the components of the lower cross-sections 
which have an area not higher than <B>area</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribarea.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex area fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "areaclosing: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lareaclosing(image, connex, param+1))
  {
    fprintf(stderr, "areaclosing: lareaclosing failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



