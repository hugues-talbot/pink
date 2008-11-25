/* $Id: reconsplateaux.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file reconsplateaux.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* operateur de reconstruction de plateaux marques par une image binaire */
/* Michel Couprie - mai 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lreconsplateaux.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t connex;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s f.pgm g.pgm connex out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "reconsdilat: readimage failed\n");
    exit(1);
  }

  connex = atoi(argv[3]);

  if (! lreconsplateaux(image1, image2, connex))
  {
    fprintf(stderr, "%s: function lreconsplateaux failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[4]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
