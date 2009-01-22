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

<B>MSE</B> - Compute a minimum spanning extension of an image from a
set of markers

SYNOPSIS

<B>MSE4d</B> GAin.pgm MarqueurPrefix first last TypeResult
LabelImageResult

DESCRIPTION

Compute a minimum spanning forest relative to the opening of the the
set of markers (vertices) in <B>ImageMarqueur</B> in the edge-weighted
graph <B>GAin.pgm</B>, as defined in ??. The marker sequence should be
a sequence of 3D byte images formated MarqueurPrefixXXXX.pgm. If
TypeResult is set to 0 then a 4DGA is outputed, otherwise if
TypeResult = 1 a sequence of 3D byte label images are outputed.

Types supported: byte 4D.

CLASS 

connect

*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lMSF.h>


/* =============================================================== */
int main(int32_t argc, char **argv) 
/* =============================================================== */
{
  struct GA4d * ga;
  struct xvimage4D * marqueurs;
  int32_t nblabels;
  int32_t nlabels;

  /* A priori il n'y a (quasi?) rien à modifier pour passer à des
     marqueurs codés en uint32_t */

  if ((argc != 7))
  {
    fprintf(stderr, "usage: %s GAin.pgm MarqueurPrefix first last TypeResult LabelImageResult \n TypeResult = 0 result is GA otherwise it is a label image\n", argv[0]);
    exit(1);
  }
  if( (ga = readGA4d(argv[1])) == NULL)
  {
    fprintf(stderr, "%s: readGA4d failed\n", argv[0]);
    exit(1);
  }
 
  if( (marqueurs = readimage4D(argv[2], atoi(argv[3]), atoi(argv[4]))) == NULL)
  {
    fprintf(stderr, "%s: readimage4d failed\n", argv[0]);
    exit(1);
  } 
  
  if (! MSF4d(ga,marqueurs)){
    fprintf(stderr, "%s: MSF4d failed\n", argv[0]);
    exit(1);
  }
  printf("Calcule de la MSE effectué \n");
  /* Il faut surement tester la valeur de retour de writeimage() */
  if(atoi(argv[3]) == 0)
    writeGA4d(ga,argv[4]); 
  else 
    writeimage4D(marqueurs, argv[argc - 1], atoi(argv[3]), atoi(argv[4]));
  
  freeGA4d(ga); freeimage4D(marqueurs); 
  return 0;
}
