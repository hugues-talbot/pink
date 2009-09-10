/* \file long2byte.c

\brief Souza and Banon's thinning algorithm

<B>Usage:</B> long2byte in.pgm out.pgm

<B>Description:</B>
The input image must be a 8-distance map computed from 
the 2dkhalimskization of a binary image, with mode h.

<B>Types supported:</B> int32_t 2d

<B>Category:</B> experimental
\ingroup  experimental

\author Michel Couprie
*/

/*
   Michel Couprie - nov. 2004
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * imagelong;
  int32_t *g;
  int32_t x, y, i, rs, cs, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  imagelong = readimage(argv[1]);  
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (datatype(imagelong) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: image type must be uint32_t\n", argv[0]);
    exit(1);
  }

  rs = rowsize(imagelong);
  cs = colsize(imagelong);
  N = rs * cs;
  g = SLONGDATA(imagelong);

  for (x = 1; x <= rs-2; x++)
  for (y = 1; y <= cs-2; y++)
  {
    if ((g[y*rs + x] > 0) && 
	(abs(g[y*rs + x-1]) <= g[y*rs + x]) &&
	(abs(g[(y-1)*rs + x]) <= g[y*rs + x]) &&
	((abs(g[y*rs + x+1]) <= g[y*rs + x]) || (g[y*rs + x-1] < 0)) &&
	((abs(g[(y+1)*rs + x]) <= g[y*rs + x]) || (g[(y-1)*rs + x] < 0))
       )
      g[y*rs + x] = -g[y*rs + x];
  }

  for (x = rs-2; x >= 1; x--)
  for (y = cs-2; y >= 1; y--)
  {
    if ((g[y*rs + x] > 0) && 
	(((abs(g[y*rs + x-1]) > g[y*rs + x]) && (g[y*rs + x+1] < 0)) ||
	 ((abs(g[(y-1)*rs + x]) > g[y*rs + x]) && (g[(y+1)*rs + x] < 0)))
       )
      g[y*rs + x] = -g[y*rs + x];    
  }

  for (x = 0; x < N; x++) if (g[x] < 0) g[x] = 1; else g[x] = 0;

  writeimage(imagelong, argv[argc-1]);
  freeimage(imagelong);
  return 0;
} /* main */
