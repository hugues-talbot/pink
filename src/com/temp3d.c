/* $Id: temp3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file temp3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mckhalimsky3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * f;
  struct xvimage * g;
  int32_t rs, cs, ps, ds, N;
  uint8_t *F;
  int32_t x;
  int32_t t, tb;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  g = allocimage(NULL, 7, 7, 7, VFF_TYP_1_BYTE);
  if (g == NULL)
  {
    fprintf(stderr,"%s : malloc failed\n", argv[0]);
    exit(1);
  }

  f = readimage(argv[1]);  
  if (f == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(f);
  cs = colsize(f);
  ds = depth(f);
  F = UCHARDATA(f);
  ps = rs * cs;
  N = ps * ds;

  printimage(f);
  printf("N = %d\n", N);

  for (x = 0; x < N; x++)
  {
    if (F[x]) 
    {
      t = T3d(f, x % rs, (x % ps) / rs, x / ps, g);
      tb = Tbar3d(f, x % rs, (x % ps) / rs, x / ps, g);
      printf("point %d,%d,%d : T = %d ; Tbar = %d\n", x % rs, (x % ps) / rs, x / ps, t, tb);
    }
  } 
  
  freeimage(g);
  freeimage(f);

  return 0;
} /* main */
