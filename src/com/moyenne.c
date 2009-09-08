/* $Id: moyenne.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
/* \file moyenne.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* moyenne de N images */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *I;
  uint32_t * F;
  int32_t rs, cs, d, N;
  int32_t i, x;

  if (argc < 3)
  {
    fprintf(stderr, "usage: %s in1.pgm ... inN.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "moyenne: readimage failed\n");
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;
  I = UCHARDATA(image);

  F = (uint32_t *)calloc(N, sizeof(int32_t));
  if (F == NULL)
  {
    fprintf(stderr, "moyenne: calloc failed\n");
    exit(1);
  }

  i = 2;
  do
  {
    for (x = 0; x < N; x++) F[x] += I[x];

    freeimage(image);
    image = readimage(argv[i]);
    if (image == NULL)
    {
      fprintf(stderr, "moyenne: readimage failed\n");
      exit(1);
    }
    if ((rs!=rowsize(image)) || (cs!=colsize(image)) || (d!=depth(image)))
    {
      fprintf(stderr, "moyenne: incompatible image sizes\n");
      exit(1);
    }
    I = UCHARDATA(image);
    i++;
  } while (i <= argc-2);
  
  for (x = 0; x < N; x++) I[x] = (uint8_t)(F[x]/(argc - 2));

  writeimage(image, argv[argc-1]);
  freeimage(image);
  free(F);

  return 0;
} /* main */


