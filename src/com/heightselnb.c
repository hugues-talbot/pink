/* $Id: heightselnb.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
/*! \file heightselnb.c

\brief height based segmentation by selection of a given number or components

<B>Usage:</B> heightselnb in.pgm connex nb out.pgm

<B>Description:</B>
Segmentation by selection of a given number or components, based on a 
height (contrast) criterion. 
Connexity = <B>connex</B> ; desired number of components = <B>nb</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

/*
%TEST heightselnb %IMAGES/2dbyte/gray/g2gel.pgm 4 3 %RESULTS/heightselnb_g2gel_4_3.pgm
%TEST heightselnb %IMAGES/2dbyte/gray/g2gel.pgm 8 3 %RESULTS/heightselnb_g2gel_8_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 6 3 %RESULTS/heightselnb_g3a_6_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 18 3 %RESULTS/heightselnb_g3a_18_3.pgm
%TEST heightselnb %IMAGES/3dbyte/gray/g3a.pgm 26 3 %RESULTS/heightselnb_g3a_26_3.pgm
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribheight.h>

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
    fprintf(stderr, "heightselnb: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  if (! lheightselnb(image, connex, param, 0))
  {
    fprintf(stderr, "heightselnb: lheightselnb failed\n");
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */



