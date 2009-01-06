/* $Id: translate.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file translate.c

\brief image translation 

<B>Usage:</B> in.pgm ox oy oz [mode] out.ppm

<B>Description:</B>
Translates the image by the offsets \b ox, \b oy, \b oz in the directions
x, y, z respectively. Offsets may be negative or positive integers.
If \b mode is 0, the points that get out of the image frame are lost. 
If \b mode is 1, a torical space is simulated in all dimensions. 
Default mode is 0.

<B>Types supported:</B> byte 2d, byte3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <loffset.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t ox, oy, oz, mode = 0;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm ox oy oz [mode] out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  ox = atoi(argv[2]);
  oy = atoi(argv[3]);
  oy = atoi(argv[4]);
  if (argc == 7) mode = atoi(argv[5]);

  if (! loffset(image, ox, oy, oz, mode))
  {
    fprintf(stderr, "%s: function loffset failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
