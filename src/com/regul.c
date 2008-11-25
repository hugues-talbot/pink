/* $Id: regul.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file regul.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/

#define SATUR

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lregul.h>
#include <llvkern.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  int32_t mode;
  double alpha;
  int32_t nbreg;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in1.pgm mode [0:basse, 1:haute, 2: les deux] alpha out.pgm \n", argv[0]);
    exit(1);
  }
  mode = atoi(argv[2]);
  if ((mode < 0) || (mode > 2))
  {
    fprintf(stderr, "usage: %s in1.pgm mode [0:basse, 1:haute, 2: les deux] alpha out.pgm \n", argv[0]);
    exit(1);
  }
  alpha = atof(argv[3]);

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

#ifdef SATUR
  do
  {
#endif
    if (! lregul(image1, mode, alpha, &nbreg))
    {
      fprintf(stderr, "%s: function lregul failed\n", argv[0]);
      exit(1);
    }
    printf("regul : %d REGULARISATIONS\n", nbreg);
    {
      if (! llvkern(image1, 0, 4))
      {
        fprintf(stderr, "%s: function llvkern failed\n", argv[0]);
        exit(1);
      }
    }
#ifdef SATUR
  }
  while (nbreg);
#endif

  writeimage(image1, argv[4]);
  freeimage(image1);

  return 0;
} /* main */


