/* $Id: surimp.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file surimp.c

\brief superimposition of a binary image with a grayscale image

<B>Usage:</B> surimp i1.ndg i2.bin out.ppm

<B>Description:</B> Superimposes the binary image <b>i2.bin</b> (in red) to the 
grayscale image <b>i1.bin</b>. The result is stored as a color image <b>out.ppm</b>.

<B>Types supported:</B> byte 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;
  struct xvimage * tmp;
  struct xvimage * tmp3;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s i1.ndg i2.bin out.ppm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {   fprintf(stderr,"%s: readimage failed\n", argv[0]);
      exit(1);
  }

  tmp = copyimage(image2);
  linverse(tmp);
  tmp3 = copyimage(image1);
  lmin(tmp3,tmp);
  copy2image(tmp,image2);
  ladd(tmp,tmp3);
  writergbimage(tmp, tmp3, tmp3, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);
  freeimage(tmp);
  freeimage(tmp3);

  return 0;
} /* main */
