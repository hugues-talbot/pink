/* $Id: grid.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file grid.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* surimpressionne une grille a l'image en ndg */

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
  struct xvimage * in;
  struct xvimage * temp1;
  struct xvimage * temp2;
  int32_t x;
  uint8_t *I;
  uint8_t *T1;
  uint8_t *T2;
  int32_t rs, cs, N;
  int32_t gridstep;
  int32_t step5;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm gridstep out.ppm \n", argv[0]);
    exit(1);
  }

  in = readimage(argv[1]);
  if (in == NULL)
  {
    fprintf(stderr, "grid: readimage failed\n");
    exit(1);
  }
  
  gridstep = atoi(argv[2]);
  step5 = 5 * gridstep;
  temp1 = copyimage(in);
  temp2 = copyimage(in);
  if ((temp1 == NULL) || (temp2 == NULL))
  {
    fprintf(stderr, "grid: copyimage failed\n");
    exit(1);
  }
  
  rs = in->row_size;
  cs = in->col_size;
  N = rs * cs;
  T1 = UCHARDATA(temp1);
  T2 = UCHARDATA(temp2);
  I = UCHARDATA(in);
  for (x = 0; x < N; x++)
  {
    if (((x / rs) % gridstep  == gridstep - 1) || ((x % rs) % gridstep == gridstep - 1))
    {
      I[x] = NDG_MIN;
      T1[x] = NDG_MIN;
      T2[x] = NDG_MAX;           /* petites divisions en bleu */
    }    
    if (((x / rs) % step5  == step5 - 1) || ((x % rs) % step5 == step5 - 1))
    {
      I[x] = NDG_MIN;
      T1[x] = NDG_MAX;           /* grandes divisions en vert */
      T2[x] = NDG_MIN;
    }
  }

  writergbimage(in, temp1, temp2, argv[3]);
  freeimage(in);
  freeimage(temp1);
  freeimage(temp2);

  return 0;
} /* main */
