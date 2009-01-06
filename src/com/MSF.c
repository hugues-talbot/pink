/* \file MSE.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Jean Cousty
*/
/* 
NAME

<B>MSE</B> - Compute a minimum spanning extension of an image from a set of markers

SYNOPSIS

<B>MSE</B> GAin.pgm ImageMarqueur.pgm ImageSpanning.pgm

DESCRIPTION

Compute a minimum spanning extension of a weighted edges graph <B>GAin.pgm</B>, taking the
set of markers (vertices) in <B>ImageMarqueur</B> as defined in ?? . 
Types supported: byte 2D, byte 3D.

CLASS 

morph

*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <llabelextrema.h>
#include <lMSF.h>


/* =============================================================== */
int32_t main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * ga;
  struct xvimage * marqueurs;
  struct xvimage * labels;
  int32_t nblabels;
  int32_t nlabels;

  if ((argc != 5))
  {
    fprintf(stderr, "usage: %s GAin.ga marqueurs.pgm typeResul GAout.ga\n TypeResult = 0 result is GA otherwise it is a label image\n", argv[0]);
    exit(1);
  }
  ga = readGAimage(argv[1]);
  marqueurs = readimage(argv[2]);
  if ((ga == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  
  labels = allocimage(NULL, rowsize(marqueurs), colsize(marqueurs), depth(marqueurs), VFF_TYP_4_BYTE);
  if (labels == NULL){   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  /* Il doit y avoir mieux dans Pink pour labeler des composantes connexes */ 
  if(depth(marqueurs) == 1){
    if (! llabelextrema(marqueurs, 4, 0, labels, &nblabels)){
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
  }
  else 
    if (! llabelextrema(marqueurs, 6, 0, labels, &nblabels)){
      fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
      exit(1);
    }
  
  if (! MSF(ga,labels)){
    fprintf(stderr, "%s: lLlpe failed\n", argv[0]);
    exit(1);
  }
  
  /*Il faut surement tester la valeur de retour de writeimage() */
  if(atoi(argv[3]) == 0)
    writerawGAimage(ga,argv[4]); 
  else writeimage(labels,argv[4]);
  
  freeimage(labels);
  freeimage(ga); freeimage(marqueurs); 
  return 0;
}


