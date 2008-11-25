/* $Id: skew.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file skew.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* transformation non-lineaire de l'echelle de gris d' une image */
/* Michel Couprie - juillet 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lskew.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  double skew;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.pgm skew out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "skew: readimage failed\n");
    exit(1);
  }
  skew = atof(argv[2]);

  if (! lskew(image1, skew))
  {
    fprintf(stderr, "skew: function lskew failed\n");
    exit(1);
  }

  writeimage(image1, argv[3]);
  freeimage(image1);

  return 0;
} /* main */
