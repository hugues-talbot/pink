/* $Id: convol3.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file convol3.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* convolution par un masque 3x3 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvol3.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask;

  if ((argc < 4) || (argc > 5) || ((argc == 5) && (strcmp(argv[3], "norm") != 0)))
  {
    fprintf(stderr, "usage: %s in.pgm mask.pgm [norm] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  mask = readimage(argv[2]);
  if ((image == NULL) || (mask == NULL))
  {
    fprintf(stderr, "convol3: readimage failed\n");
    exit(1);
  }
  
  if (! lconvol3(image, mask, (argc == 5)))
  {
    fprintf(stderr, "convol3: function lconvol3 failed\n");
    exit(1);
  }

  if (argc == 5) writeimage(image, argv[4]); else writeimage(image, argv[3]);
  freeimage(image);
  freeimage(mask);
  return 0;
} /* main */

