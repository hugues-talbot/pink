/* $Id: isometry.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file isometry.c

\brief basic isometries in 3D 

<B>Usage:</B> isometry in MODE out

<B>Description:</B>
Basic isometries in 2D and 3D.
Available modes: xzy, yxz, yzx, zxy, zyx, (3D), and
0: identity,
1: rotation 90 degrees clockwise,
2: rotation 180 degrees clockwise,
3: rotation 270 degrees clockwise,
4: symmetry / vertical axis,
5: symmetry / horizontal axis.

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/*
   Michel Couprie  -  Novembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lisometry.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in mode out \n", argv[0]);
    fprintf(stderr, "available modes: xzy, yxz, yzx, zxy, zyx, 0, 1, 2, 3, 4, 5\n");
    exit(1);
  }

  if ((strcmp(argv[2], "xzy") != 0) &&
      (strcmp(argv[2], "yxz") != 0) &&
      (strcmp(argv[2], "yzx") != 0) &&
      (strcmp(argv[2], "zxy") != 0) &&
      (strcmp(argv[2], "zyx") != 0) &&
      (strcmp(argv[2], "0") != 0) &&
      (strcmp(argv[2], "1") != 0) &&
      (strcmp(argv[2], "2") != 0) &&
      (strcmp(argv[2], "3") != 0) &&
      (strcmp(argv[2], "4") != 0) &&
      (strcmp(argv[2], "5") != 0)
     )
  {   
    fprintf(stderr, "%s() : bad mode : %s\n", argv[0], argv[2]);
    fprintf(stderr, "available modes: xzy, yxz, yzx, zxy, zyx, 0, 1, 2, 3, 4, 5\n");
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lisometry(image1, argv[2], &image2))
  {
    fprintf(stderr, "%s: function lisometry failed\n", argv[0]);
    exit(1);
  }

  writeimage(image2, argv[3]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


