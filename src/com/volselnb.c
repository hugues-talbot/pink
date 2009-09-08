/* $Id: volselnb.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
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

/*
%TEST volselnb %IMAGES/2dbyte/gray/g2gel.pgm 4 3 %RESULTS/volselnb_g2gel_4_3.pgm
%TEST volselnb %IMAGES/2dbyte/gray/g2gel.pgm 8 3 %RESULTS/volselnb_g2gel_8_3.pgm
%TEST volselnb %IMAGES/3dbyte/gray/g3a.pgm 6 3 %RESULTS/volselnb_g3a_6_3.pgm
%TEST volselnb %IMAGES/3dbyte/gray/g3a.pgm 18 3 %RESULTS/volselnb_g3a_18_3.pgm
%TEST volselnb %IMAGES/3dbyte/gray/g3a.pgm 26 3 %RESULTS/volselnb_g3a_26_3.pgm
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



