/* $Id: diZenzo.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file diZenzo.c

\brief diZenzo gradient pour les images couleurs

<B>Usage:</B> diZenzo imageRVB.ppm alpha [mode] out.pgm

<B>Description:</B>
Le gradient de diZenzo est d�fini par la donn�e de p, q, et t:

p = Rx*Rx + Vx*Vx + Bx*Bx

q = Ry*Ry + Vy*Vy + By*By

t = Rx*Ry + Vx*Vy + Bx*By

(ou Rx = d�riv�e en x de la bande rouge, Ry est la d�riv�e en y de la bande rouge, etc.)

et le module est donn�e par

G = sqrt(1/2*(p+q+sqrt((p+q)*(p+q) -4(pq-t*t))))

La direction est donn�e par 1/2*atan(2*t/(p-q))

Si le mode est �gale � 0 (valeur d�faut) alors l'image de sortie est le gradient, 
sinon l'image de sortie est une int32_t entre 0 et 360.

Les gradients sont calcul�s par les filtres de Deriche, de param�tre alpha

<B>Types supported:</B> byte 2D

<B>Category:</B> signal
\ingroup  signal

\author Laurent Najman
*/
/* Gradient couleur de diZenzo bas� sur le filtre lineaire general recursif de Deriche */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldiZenzo.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * imageR;
  struct xvimage * imageV;
  struct xvimage * imageB;
  struct xvimage * result;
  double alpha;
  int32_t mode;
  double l;

  if ((argc < 4) || (argc > 5))
  {
    fprintf(stderr, "usage: %s inRVB.ppm alpha [mode] out.pgm \n", 
            argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &imageR, &imageV, &imageB) == 0)
  {
    fprintf(stderr, "diZenzo: readrgbimage failed\n");
    exit(1);
  }

  alpha = atof(argv[2]);
  if (argc == 5) {
    mode = atoi(argv[3]);
  } else mode = 0;
  
  if (mode == 0) {
    if (! ldiZenzoGradient(imageR, imageV, imageB, alpha))
      {
	fprintf(stderr, "deriche: function lderiche failed\n");
	exit(1);
      }
    writeimage(imageR, argv[argc-1]);
  } else {
    result = allocimage(NULL, rowsize(imageR), colsize(imageR), depth(imageR), VFF_TYP_4_BYTE);
    if (result == NULL)
    {   
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    ldiZenzoDirection(imageR, imageV, imageB, result, alpha);
    writeimage(result, argv[argc-1]);
    freeimage(result);
  }


  freeimage(imageR);
  freeimage(imageV);
  freeimage(imageB);

  return 0;
} /* main */


