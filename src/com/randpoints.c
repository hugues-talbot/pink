/* $Id: randpoints.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file randpoints.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* remplace une proportion p de points par des valeurs aleatoires 
   suivant une loi uniforme (entre i et s) */
/* Michel Couprie - janvier 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcprobas.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  double p;
  uint8_t inf, sup;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in.pgm p i s out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = image->row_size;
  cs = image->col_size;
  d = depth(image);
  N = rs * cs * d;
  Im = UCHARDATA(image);

  p = atof(argv[2]);
  inf = (uint8_t)atoi(argv[3]);
  sup = (uint8_t)atoi(argv[4]);
  
  for (i = 0; i < N; i++)
  {
    if (Uniform(0,1) <= p)
      Im[i] = (uint8_t)Uniform(inf, sup);
  }
  writeimage(image, argv[5]);
  freeimage(image);

  return 0;
} /* main */
