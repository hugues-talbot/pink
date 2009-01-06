/* $Id: detectcercles.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file detectcercles.c

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
#include <ldetectcercles.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayon;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s f.pgm rayon out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "detectcercles: readimage failed\n");
    exit(1);
  }
  
  rayon = atoi(argv[2]);
  if (! ldetectcercles(image, rayon))
  {
    fprintf(stderr, "detectcercles: function ldetectcercles failed\n");
    exit(1);
  }

  writeimage(image, argv[3]);
  freeimage(image);
  return 0;
} /* main */
