/* $Id: detectcercles.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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
