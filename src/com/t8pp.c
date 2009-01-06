/* $Id: t8pp.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file t8pp.c

\brief computes the connecivity number t8pp for each image pixel

<B>Usage:</B> t8pp in out

<B>Description:</B> 
Computes the connecivity number t8pp (as defined in ref. BEC97) for each image pixel.

<B>Types supported:</B> byte 2D

<B>Category:</B> topo
\ingroup  topo

\author Michel Couprie
*/
/* Michel Couprie - juillet 1996, mai 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ltopotypes.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "t8pp: readimage failed\n");
    exit(1);
  }

  if (! lt8pp(image))
  {
    fprintf(stderr, "t8pp: function lt8pp failed\n");
    exit(1);
  }
  
  writeimage(image, argv[argc - 1]);
  freeimage(image);

  return 0;
} /* main */
