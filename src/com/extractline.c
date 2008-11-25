/* $Id: extractline.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file extractline.c

\brief extracts a line between two given points from a 3D image

<B>Usage:</B> extractline in.pgm x1 y1 x2 y2 out.pgm

<B>Description:</B>
The original 2D image is <B>in.pgm</B>. 
The output 1D image <B>out.pgm</B> contains the line between 
points (x1,y1) and (x2,y2) extracted from <B>in.pgm</B>.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#define PARANO

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lbresen.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * imgres;
  int32_t i, j, n, nn, rs, cs, x1, y1, x2, y2;
  uint8_t *I, *R;
  int32_t *listpoints;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in.pgm x1 y1 x2 y2 out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (datatype(image) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }
  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: only 2D images allowed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  I = UCHARDATA(image);

  x1 = atoi(argv[2]);
  y1 = atoi(argv[3]);
  x2 = atoi(argv[4]);
  y2 = atoi(argv[5]);

  /* ---------------------------------------------------------- */
  /* extraction d'une ligne */
  /* ---------------------------------------------------------- */

  n = max(abs(x2-x1),(y2-y1))+1;
  listpoints = (int32_t *)calloc(n, sizeof(int32_t));
  if (image == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  lbresenlist(rs, x1, y1, x2, y2, listpoints, &nn);
#ifdef PARANO
  if (nn != n)
  {
    fprintf(stderr, "%s: WARNING: unexpected number of points returned by lbresenlist()\n", argv[0]);
    fprintf(stderr, "expected : %d ; got : %d\n", n, nn);
  }
#endif
  imgres = allocimage(NULL, n, 1, 1, datatype(image));
  R = UCHARDATA(imgres);
  for (i = 0; i < n; i++) R[i] = I[listpoints[i]];    

  writeimage(imgres, argv[argc-1]);
  freeimage(imgres);
  freeimage(image);
  free(listpoints);
  return 0;
} /* main */



