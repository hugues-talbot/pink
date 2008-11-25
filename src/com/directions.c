/* $Id: directions.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file directions.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Calcul de la direction approximativement tangente ou normale (entre 0 et 179) 
            en chaque point objet (la direction pour les points non objet est 255)
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldirections.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t connex;
  int32_t nbpoints;
  int32_t normale;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in1.pgm connex nbpoints normale out.pgm \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "directions: readimage failed\n");
    exit(1);
  }
  connex = atoi(argv[2]);
  nbpoints = atoi(argv[3]);
  normale = atoi(argv[4]);

  if (! ldirections(image1, connex, nbpoints, normale))
  {
    fprintf(stderr, "directions: function ldirections failed\n");
    exit(1);
  }

  writeimage(image1, argv[5]);
  freeimage(image1);

  return 0;
} /* main */


