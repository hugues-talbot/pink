/* $Id: areaselnb.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file areaselnb.c

\brief area based segmentation by selection of a given number or components

<B>Usage:</B> areaselnb in.pgm connex nb out.pgm

<B>Description:</B>
Segmentation by selection of a given number or components, based on an area criterion. 
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
#include <lattribarea.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
    fprintf(stderr, "areaselnb: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lareaselnb(image, connex, param))
  {
    fprintf(stderr, "areaselnb: lareaselnb failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



