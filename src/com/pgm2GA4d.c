/*! \file pgm2GA4d.c

\brief converts a series of 3

<B>Usage:</B> pgm2GA4d prefix_in first last prefix_mark mode GA4d_out.pgm

<B>Description:</B> 
Create a 4D weighted-edges graph from the 4D image made of the
concatenation of the 3D images <B>prefix_inxxxx.pgm</B> (where nnnn is
a four digit decimal integer and where <B>first</B> and <B>last</B>
specify respectively the first and the last volume). The values of
edges are computed according to the parameter mode. If mode is set to
0 then the intensity diference between the two extremities pixels of
the edges is choosen for. Otherwise the maximum value is taken. The
outputed GA is necessarily 4D, 8-connected (i.e. it corresponds to the
direct adjacency in dimension 4).

<B>Types supported:</B> byte 2d, byte 3d, byte 4d

<B>Category:</B> convert
\ingroup convert

\author Jean Cousty - janvier 2006
*/


#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <lppm2GA.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage4D * im;
  struct GA4d * ga;
  int32_t param;
  if (argc != 6)
  {
    fprintf(stderr, "usage: %s prefix_in first last mode GA4d_out.pgm \nparap = 0 difference entre pixel extremite, param = 1 max entre pixel extremite,\n", 
            argv[0]);
    exit(1);
  }
  if ((im = readimage4D(argv[1], atoi(argv[2]), atoi(argv[3]))) == NULL )
  {
    fprintf(stderr, "%s: ne peut lire %s \n", argv[0], argv[1]);
    exit(1);
  }
  //printf("taille de l'image 3d: %d\n", depth(im));
  if( (ga = allocGA4d(argv[2], rowsize(im->frame[0]) , colsize(im->frame[0]), depth(im->frame[0]), im->ss )) == NULL )
  {
    fprintf(stderr, "%s: ne peut pas allouer de graphe d'arete \n", argv[0]);
    freeimage4D(im);
    exit(1);
  }
  param = atoi(argv[4]);
  if( lpgm2ga4d(im,ga,param) != 1 )
  {
    fprintf(stderr, "%s: erreur de lppm2ga \n", argv[0]);
    exit(1);
  }
  writeGA4d(ga, argv[5]);
  freeimage4D(im); freeGA4d(ga);
  return 0;
}
