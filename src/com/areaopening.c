/* $Id: areaopening.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
/*! \file areaopening.c

\brief area opening

<B>Usage:</B> areaopening in.pgm connex area out.pgm

<B>Description:</B>
Area opening with connexity <B>connex</B> and area <B>area</B>.
Deletes the components of the upper cross-sections 
which have an area not higher than <B>area</B>.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

/*
%TEST areaopening %IMAGES/2dbyte/gray/g2gel.pgm 4 20 %RESULTS/areaopening_g2gel_4_20.pgm
%TEST areaopening %IMAGES/2dbyte/gray/g2gel.pgm 8 20 %RESULTS/areaopening_g2gel_8_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 6 20 %RESULTS/areaopening_g3a_6_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 18 20 %RESULTS/areaopening_g3a_18_20.pgm
%TEST areaopening %IMAGES/3dbyte/gray/g3a.pgm 26 20 %RESULTS/areaopening_g3a_26_20.pgm
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
    fprintf(stderr, "areaopening: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[2]);
  param = atoi(argv[3]);
  
  if (! lareaopening(image, connex, param+1))
  {
    fprintf(stderr, "areaopening: lareaopening failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */



