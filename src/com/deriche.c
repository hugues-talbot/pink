/* $Id: deriche.c,v 1.3 2009-05-14 11:37:26 mcouprie Exp $ */
/* \file deriche.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Filtre lineaire general recursif de Deriche */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lderiche.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
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
    fprintf(stderr, "           5 : derivateur en x\n");
    fprintf(stderr, "           6 : derivateur en y\n");
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  alpha = atof(argv[2]);
  function = atoi(argv[3]);
  if (function == 3) 
  { 
    if (argc != 6)
    {
      fprintf(stderr, "%s: fonction 3: argument necessaire\n", argv[0]);
      exit(1);
    }
    l = atof(argv[4]);
  }

  if (! lderiche(image1, alpha, function, l))
  {
    fprintf(stderr, "%s: function lderiche failed\n", argv[0]);
    exit(1);
  }

  if (function == 3) writeimage(image1, argv[5]);
  else               writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


