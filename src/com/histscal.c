/* $Id: histscal.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file histscal.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* transformation multi-lineaire de l'echelle de gris d' une image */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhistscal.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t a, A, b, B, c, C;

  if ((argc != 7) && (argc != 9))
  {
    fprintf(stderr, "usage: %s in.pgm a A b B [c C] out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "histscal: readimage failed\n");
    exit(1);
  }
  
  a = atoi(argv[2]);
  A = atoi(argv[3]);
  b = atoi(argv[4]);
  B = atoi(argv[5]);
  if (argc == 9)
  {
    c = atoi(argv[6]);
    C = atoi(argv[7]);
    if (! lhistscal3(image, a, A, b, B, c, C))
    {
      fprintf(stderr, "histscal: function lhistscal3 failed\n");
      exit(1);
    }
  }
  else
    if (! lhistscal(image, a, A, b, B))
    {
      fprintf(stderr, "histscal: function lhistscal failed\n");
      exit(1);
    }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
