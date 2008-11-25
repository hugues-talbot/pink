/* $Id: tracevect.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file tracevect.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
  Trace des vecteurs dont les specifications (pt debut, pt fin) 
  sont lues dans un fichier. En entete: le nombre de vecteurs.

  Utilise l'algorithme de Bresenham.

  Michel Couprie - juin 1998 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lbresen.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i, x1, y1, x2, y2;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t nvect;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm vect.txt out.pgm \n", argv[0]);
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
  if (d > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }
  N = rs * cs;
  Im = UCHARDATA(image);

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%d", &nvect);

  for (i = 0; i < nvect; i++)
  {
    fscanf(fd, "%d%d%d%d", &x1, &y1, &x2, &y2);
    lbresen(Im, rs, x1, y1, x2, y2);
  }
  fclose(fd);
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

