/* $Id: ptWsimple.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file label.c

\brief detects W-simple points in a 2D binary image

<B>Usage:</B> label in.pgm connex out.pgm

<B>Description:</B>
The argument \b connex selects the connectivity (4, 8 in 2D).

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup  connect

\author Jean Cousty (2007)
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <larith.h>
#include <llabelextrema.h>

/* =============================================================== */
int32_t main(int32_t argc, char **argv) 
/* =============================================================== */
{
  int32_t nblabels, connex, i,y,k;
  int32_t rs, cs, d, n, N, label;
  uint8_t *G;
  uint32_t *LABEL;
  struct xvimage * image;
  struct xvimage * result;
  struct xvimage * result2;
  int32_t function;
  int32_t incr;
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s filein.pgm connex fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  n = rs * cs;          /* taille plan */
  N = n * d;            /* taille image */

  if(d !=1){
    fprintf(stderr,"Cette version ne traite que les images 2D\n");
    freeimage(image);
    exit(0); 
  }
  connex = atoi(argv[2]);
  if( (connex != 4) && (connex != 8)){
    fprintf(stderr,"Connexité inconnue en 2D\n");
    freeimage(image);
    exit(0);
  }
  if(connex == 4) incr = 2; else incr = 1;
  function = LABMIN; 
  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE); 
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! llabelextrema(image, connex, function, result, &nblabels))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", argv[0]);
    exit(1);
  }
  
  result2 = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_1_BYTE);
  
  LABEL = ULONGDATA(result);
  G = UCHARDATA(result2);
  memset(G,0,N);
  for(i = 0; i < N; i++){
    if(LABEL[i] == 0){ // le points appartient à l'objet X
      G[i] = 255;
      label = -1;
      for(k = 0; k < 8; k += incr){
	y = voisin(i, k, rs, N);
	if (y != -1)
	  if(LABEL[y] > 0) // y appartient à \Bar{X}
	    if(label != -1){ 
	      if(label != LABEL[y])
		G[i] = 0;
	    } else label = LABEL[y];
      }//for(k = 0 ... )
    }// if(LABEL[i] ...)
  }// for(i = 0...)
  
  writeimage(result2, argv[argc-1]);
  freeimage(result);
  freeimage(result2);
  freeimage(image);

  return 0;
} /* main */


