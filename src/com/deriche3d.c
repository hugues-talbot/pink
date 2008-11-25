/* $Id: deriche3d.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file deriche3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Filtre lineaire general recursif de Deriche en 3D */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lderiche3d.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  double alpha;
  int32_t function;
  double l;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in1.pgm alpha function [l] out.pgm \n", 
            argv[0]);
    fprintf(stderr, "function = 0 : module du gradient lisse'\n");
    fprintf(stderr, "           1 : direction du gradient lisse'\n");
    fprintf(stderr, "           2 : laplacien lisse'\n");
    fprintf(stderr, "           3 : f - l * laplacien(f)\n");
    fprintf(stderr, "           4 : lisseur\n");
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "deriche3d: readimage failed\n");
    exit(1);
  }
  alpha = atof(argv[2]);
  function = atoi(argv[3]);
  if (function == 3) 
  { 
    if (argc != 6)
    {
      fprintf(stderr, "fonction 3: argument necessaire\n");
      exit(1);
    }
    l = atof(argv[4]);
  }

  if (function == 4)
  { 
    if (! llisseurrec3d(image1, alpha))
    {
      fprintf(stderr, "deriche3d: function llisseurrec3d failed\n");
      exit(1);
    }
  }
  else
  { 
    if (! lderiche3d(image1, alpha, function, l))
    {
      fprintf(stderr, "deriche3d: function lderiche3d failed\n");
      exit(1);
    }
  }

  if (function == 3) writeimage(image1, argv[5]);
  else               writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


