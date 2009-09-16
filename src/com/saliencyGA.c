/* \file saliencyGa.c

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

<B>saliencyGA</B> - saliency on 4-connected edge-weighted graph

SYNOPSIS

<B>saliencyGA</B> GAin.pgm type[0 surface, 1 dynamic, 2 volume 3 alphaOmegaCC] GAOut.pgm [annexe.pgm]

DESCRIPTION

Compute an edge weighted graph which is the saliency of the input for
a give type of filter. Type may take a value in {0, 1, 2}, where 0
stands for area closing, 1 for dynamic closing and 2 for simple volume
closing. We warn that simple volume closing corresponds to the
saliency by area closing of the M-border watershed.

Compute an edge weighted graph which is the saliency of the input for
a give type of filter. Type may take a value in {0, 1, 2, 3}, where 0
stands for area closing, 1 for dynamic closing and 2 for simple volume
closing and 3 for alpha-omega-CC. We warn that simple volume closing
corresponds to the saliency by area closing of the M-border watershed.

In the case of alpha-omega-CC, the annexe.pgm is the original image from which the gradient GAin.pgm is computed.

Types supported: GA byte 2D.

CLASS 

connect

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lhierarchie.h>


/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * ga, *annexe;
  int32_t param;
  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s GAin.pgm type[0 surface, 1 dynamic, 3 volumeSimple] GAOut.pgm \n", argv[0]);
    exit(1);
  }
  ga = readGAimage(argv[1]);
  if (ga == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  switch(atoi(argv[2]))
  {
  case 0: param = SURFACE; break;
  case 1: param = DYNAMIC; break;
  case 2: param = VOLUME; break; 
  case 3: param = OMEGA; break; 
  default: fprintf(stderr, "%s: Mauvais parametre\n",argv[0]); exit(0);
  }
  if (param == OMEGA) {
    annexe = readimage(argv[4]);
    if (annexe == NULL)
      {
	fprintf(stderr, "%s: readimage failed\n", argv[0]);
	exit(1);
      }
  } else 
    annexe = NULL;
  if (saliencyGa(ga, param, annexe) != 1)
  {
    fprintf(stderr, "%s: flowLPEAttributOpenning failed\n", argv[0]);
    exit(1);
  }
  writerawGAimage(ga, argv[3]);
  freeimage(ga);  
  return 0;
}
