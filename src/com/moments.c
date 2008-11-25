/* $Id: moments.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file moments.c

\brief calcul des moments d'ordre 1 et 2 d'une image binaire (nuage de points) 

<B>Usage:</B> moments in.pgm out.list

<B>Description:</B>



<B>Types supported:</B> byte 2D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llabelextrema.h>
#include <lmoments.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t n, i;
  struct xvimage * image;
  Indicateur indic;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.list \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "moments: readimage failed\n");
    exit(1);
  }

  if (! limagemoments(image, &indic))
  {
    fprintf(stderr, "%s: limagemoments failed\n", argv[0]);
    exit(1);
  }

  PrintIndicateur(indic),

  freeimage(image);

  return 0;
} /* main */
