/* $Id: temp3dthin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file temp3dthin.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
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
  struct xvimage * o;
  struct xvimage * k;
  struct xvimage * kp;
  int32_t nsteps;
  int32_t stablealpha, stablebeta;
  int32_t i, x, y, z;
  int32_t rs, cs, ps, d, N;
  uint8_t * K;
  uint8_t * KP;
  
  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm n out.pgm \n", argv[0]);
    exit(1);
  }


  o = readimage(argv[1]);  
  if (o == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  nsteps = atoi(argv[2]);

  fprintf(stderr, "Debut traitement\n");

  k = Khalimskize3d(o);

  fprintf(stderr, "Khalimskisation terminee\n");

  if (k == NULL)
  {
    fprintf(stderr, "%s: Khalimskize3d failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  d = depth(k);
  ps = rs * cs;
  N = ps * d;
  K = UCHARDATA(k);

  Connex6Obj3d(k);

  fprintf(stderr, "Connex6 termine\n");

  InitPileGrilles3d();

  kp = copyimage(k);
  KP = UCHARDATA(kp);

  for (i = 1; i <= nsteps; i++)
  {
    fprintf(stderr, "step %d\n", i);
    if (i % 2)
    {
      stablealpha = 1;
      for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z * ps + y * rs + x] && Alpha3Simple3d(k, x, y, z))
          { KP[z * ps + y * rs + x] = 0; stablealpha = 0; }
      memcpy(K, KP, N);
    }
    else
    {
      stablebeta = 1;
      for (z = 0; z < d; z++)
      for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (K[z * ps + y * rs + x] && Beta3Simple3d(k, x, y, z))
          { KP[z * ps + y * rs + x] = 0; stablebeta = 0; }
      memcpy(K, KP, N);
    }
    if (stablealpha && stablebeta) break;
  }

  TerminePileGrilles3d();
  
  ColorieKh3d(k);
  writeimage(k, "tmp");

  DeKhalimskize3d(k, o);
  writeimage(o, argv[3]);

  freeimage(o);
  freeimage(k);
  freeimage(kp);

  return 0;
} /* main */
