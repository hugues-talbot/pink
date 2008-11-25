/* $Id: recons.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file recons.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* operateur de reconstruction binaire */
/* 
   entrees : img, une image en niveaux de gris
             seg, une image binaire (pre-segmentation topologiquement correcte,
                                     sur-ensemble de la segmentation desiree)
             connex, 4 ou 8
   sortie : seg, la segmentation (binaire) desiree
*/
/* Michel Couprie -  avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <lrecons.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * img;
  struct xvimage * seg;
  int32_t connex;
  int32_t memoire;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s img.pgm seg.pgm connex mem out.pgm \n", argv[0]);
    exit(1);
  }

  img = readimage(argv[1]);
  seg = readimage(argv[2]);
  if ((img == NULL) || (seg == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  connex = atoi(argv[3]);
  if ((connex != 4) && (connex != 8))
  {   
    fprintf(stderr, "%s: parametre connex = [4|8]\n", argv[0]);
    exit(1);
  }

  memoire = atoi(argv[4]);
  if ((memoire != 0) && (memoire != 1))
  {   
    fprintf(stderr, "%s: parametre mem = [0|1]\n", argv[0]);
    exit(1);
  }

  if (! lrecons(img, seg, connex, memoire))
  {
    fprintf(stderr, "recons: function lrecons failed\n");
    exit(1);
  }

  writeimage(seg, argv[5]);
  freeimage(img);
  freeimage(seg);

  return 0;
} /* main */
