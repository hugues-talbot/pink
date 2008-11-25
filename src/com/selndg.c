/* $Id: selndg.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file selndg.c

\brief selects pixels with values between two thresholds 

<B>Usage:</B> selndg in.pgm inf sup out.ppm

<B>Description:</B> 
Selects pixels x such that \b inf <= F[x] <= \b sup.
If the input image is of type byte, the output is a binary image.
If the input image is of type long, the output is also a long image
where all pixels whose values are not in the interval, are put to 0.

<B>Types supported:</B> byte 2d, byte 3d, long 2d, long 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/
/* selection de pixels ayant un niveau de gris compris entre 2 bornes */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lselndg.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t inf, sup;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm inf sup out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "selndg: readimage failed\n");
    exit(1);
  }
  
  inf = atoi(argv[2]);
  sup = atoi(argv[3]);

  if (! lselndg(image, inf, sup))
  {
    fprintf(stderr, "selndg: function lselndg failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
