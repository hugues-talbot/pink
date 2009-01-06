/* $Id: convol.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file convol.c

\brief convolution

<B>Usage:</B> convol in.pgm kernel.pgm [mode] out.pgm

<B>Description:</B>
Convolution of \b in.pgm by <B>kernel.pgm</B>.
The result is a float image.
Depending on the value given for the (optional) parameter <B>mode</B>:
\li   <B>mode</B> = 0 (default) : naive algorithm. 
      The image \b in.pgm is considered as null out of its support. 
\li   <B>mode</B> = 1 : naive algorithm. 
      The boundary of image \b in.pgm is extended outside its support.
\li   <B>mode</B> = 2 : convolution using the FFT.
      The image \b in.pgm is considered as null out of its support. 
\li   <B>mode</B> = 3 : convolution using the FFT.
      The boundary of image \b in.pgm is extended outside its support.

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d

<B>Category:</B> signal
\ingroup  signal

\warning
Naive convolution algorithm is in O(|in|*|kernel|). 
For large kernels, use FFT version which is in 
O(n log n) where n = max(|in|,|kernel|)

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvol.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask;
  int32_t mode = 0;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm kernel.pgm [mode] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  mask = readimage(argv[2]);
  if ((image == NULL) || (mask == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 5) mode = atoi(argv[3]);

  if (! convertfloat(&mask))
  {
    fprintf(stderr, "%s: function convertfloat failed\n", argv[0]);
    exit(1);
  }

  if (! convertgen(&image, &mask))
  {
    fprintf(stderr, "%s: function convertgen failed\n", argv[0]);
    exit(1);
  }
  
  if (! lconvol(image, mask, mode))
  {
    fprintf(stderr, "%s: function lconvol failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(mask);
  return 0;
} /* main */

