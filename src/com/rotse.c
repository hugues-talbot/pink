/* $Id: rotse.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file rotse.c

\brief rotation of a structuring element

<B>Usage:</B> rotse in.pgm angle out.pgm

<B>Description:</B>
Rotation of a structuring element, by an angle of 0, 90, 180 or 270 degrees.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/*
   Michel Couprie  -  Janvier 2005
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsym.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t x, y, z, i1, j1, i2, j2;
  int32_t rs, cs, ds;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm angle out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readse(argv[1], &x, &y, &z);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image1);      /* taille ligne */
  cs = colsize(image1);      /* taille colonne */
  ds = depth(image1);        /* nb plans */

  if (ds != 1)
  {
    fprintf(stderr, "%s: NYI for 3D images\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2], "0") == 0) writese(image1, argv[argc - 1], x, y, z);
  else if (strcmp(argv[2], "180") == 0) 
  {
    if (! lsym(image1, 'c'))
    {
      fprintf(stderr, "%s: function lsym failed\n", argv[0]);
      exit(1);
    }
    writese(image1, argv[argc - 1], rs - 1 - x, cs - 1 - y, 1);
  }
  else if ((strcmp(argv[2], "90") == 0) || (strcmp(argv[2], "270") == 0))
  {
    struct xvimage * image2;
    uint8_t *I1, *I2;
    image2 = allocimage(NULL, cs, rs, 1, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    I1 = UCHARDATA(image1);
    I2 = UCHARDATA(image2);
    for (j2 = i1 = 0; i1 < rs; j2++, i1++)
      for (i2 = cs-1, j1 = 0; i2 >= 0; i2--, j1++)
	I2[j2*cs + i2] = I1[j1*rs + i1];
    if (strcmp(argv[2], "90") == 0) 
      writese(image2, argv[argc - 1], cs - 1 - y, x, 1);
    else
    {
      if (! lsym(image2, 'c'))
      {
	fprintf(stderr, "%s: function lsym failed\n", argv[0]);
	exit(1);
      }
      writese(image2, argv[argc - 1], y, rs - 1 - x, 1);
    }
    freeimage(image2);
  }
  else
  {
    fprintf(stderr, "%s: bad angle value (must be 0, 90, 180, 270) : %s\n", argv[0], argv[2]);
    exit(1);
  }
  freeimage(image1);
  return 0;
} /* main */


