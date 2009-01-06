/* $Id: houghcercles.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file houghcercles.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Michel Couprie - mars 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lhoughcercles.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * accu;
  int32_t rayonmin;
  int32_t pasrayon;
  int32_t nbpas;
  int32_t rs, cs;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s f.pgm rayonmin pasrayon nbpas accu.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  
  rayonmin = atoi(argv[2]);
  pasrayon = atoi(argv[3]);
  nbpas = atoi(argv[4]);

  accu = allocimage(NULL, rs, cs, nbpas, VFF_TYP_1_BYTE);
  if (accu == NULL)
  {  
    fprintf(stderr,"%s : allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lhoughcercles(image, rayonmin, pasrayon, nbpas, accu))
  {
    fprintf(stderr, "%s: function lhoughcercles failed\n", argv[0]);
    exit(1);
  }

  writeimage(accu, argv[5]);
  freeimage(image);
  freeimage(accu);
  return 0;
} /* main */
