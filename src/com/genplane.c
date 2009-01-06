/* $Id: genplane.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file genplane.c

\brief generates a plane normal to one of the directions x,y,z

<B>Usage:</B> genplane rs cs ds dir n out.pgm

<B>Description:</B>
The size of the result image <B>out.pgm</B> is given by the parameters 
<B>rs</B>, <B>cs</B>, <B>ds</B>.
The parameter <B>dir</B> is set to <B>x</B>, <B>y</B>, or <B>z</B>.
The parameter <B>n</B> is an integer.
The output binary image <B>out.pgm</B> contains the plane defined by <B>dir=n</B>.

<B>Types supported:</B> byte 3d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* Michel Couprie - decembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, ds, ps, N;
  char name[128];
  int32_t x, y, z, n;
  char axe;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s rs cs ds <x|y|z> n out.pgm\n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  ps = rs * cs;
  N = ps * ds;
  axe = argv[4][0];
  n = atoi(argv[5]);
  sprintf(name, "genplan(%c=%d)", axe, n);
  image = allocimage(name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (image == NULL)
  {
    fprintf(stderr, "genplan: allocimage failed\n");
    exit(1);
  }
  Im = UCHARDATA(image);
  memset(Im, 0, N);

  switch(axe)
  {
    case 'x': 
      if ((n < 0) || (n >= rs))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (z = 0; z < ds; z++)
        for (y = 0; y < cs; y++)
          Im[z * ps + y * rs + n] = NDG_MAX;
      break;
    case 'y': 
      if ((n < 0) || (n >= cs))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (z = 0; z < ds; z++)
        for (x = 0; x < rs; x++)
          Im[z * ps + n * rs + x] = NDG_MAX;
      break;
    case 'z': 
      if ((n < 0) || (n >= ds))
      {
        fprintf(stderr, "%s: bad n = %d\n", argv[0], n);
        exit(1);
      }
      for (y = 0; y < cs; y++)
        for (x = 0; x < rs; x++)
          Im[n * ps + y * rs + x] = NDG_MAX;
      break;
    default:
      fprintf(stderr, "usage: %s rs cs ds <x|y|z> n out.pgm\n", argv[0]);
      exit(1);
  } /* switch */

  writeimage(image, argv[6]);
  freeimage(image);
  return 0;
} /* main */

