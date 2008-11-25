/* $Id: fermetrous3d.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file fermetrous3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Operateur de fermeture de trous en 3d */
/* Michel Couprie - decembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfermetrous3d.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * image;      /* image (binaire) de depart */
  int32_t connex;
  int32_t tailletrous;
  
  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm connex tailletrous out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  tailletrous = atoi(argv[3]);

  if (! lfermetrous3dbin(image, connex, tailletrous))
  {
    fprintf(stderr, "%s: function lfermetrous3dbin failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
