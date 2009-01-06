/* $Id: fusreg.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file fusreg.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfusreg.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * result;
  int32_t memoire;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s image.pgm marqueurs.pgm mem fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "fusreg: readimage failed\n");
    exit(1);
  }

  marqueurs = readimage(argv[2]);
  if (marqueurs == NULL)
  {
    fprintf(stderr, "fusreg: readimage failed\n");
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "fusreg: allocimage failed\n");
    exit(1);
  }

  memoire = atoi(argv[3]);
  if ((memoire != 0) && (memoire != 1))
  {   
    fprintf(stderr, "fusreg: parametre mem = [0|1]\n");
    exit(1);
  }

  if (! lfusreg(image, marqueurs, result, memoire))
  {
    fprintf(stderr, "fusreg: lfusreg failed\n");
    exit(1);
  }

  writelongimage(result, argv[4]);
  freeimage(image);
  freeimage(marqueurs);
  freeimage(result);
  return 0;
} /* main */



