/* $Id: volselnb.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file volselnb.c

\brief volume based segmentation by selection of a given number or components

<B>Usage:</B> volselnb in.pgm connex nb out.pgm

<B>Description:</B>
Segmentation by selection of a given number or components, based on a 
volume criterion. 
Connexity = <B>connex</B> ; desired number of components = <B>nb</B>.

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
#include <lattribvol.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex, param;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex nb fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "volselnb: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lvolselnb(image, connex, param))
  {
    fprintf(stderr, "volselnb: lvolselnb failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



