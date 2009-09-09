/* $Id: ellipseincl.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ellipseincl.c

\brief ellipse identification and drawing from spare points

<B>Usage:</B> ellipseincl in.pgm pts.list filled out.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie and Yskandar Hamam
*/
/* 
  Michel Couprie - avril 2002 
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcutil.h>
#include <ldraw.h>
#include <lborder.h>
#include <lellipseincl.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * ellipse;
  uint8_t *Im;
  uint8_t *El;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *x, *y, xx, yy, R, S, T, U, V, Z;
  int32_t filled;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm pts.list filled out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  if (d > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }
  N = rs * cs;
  Im = UCHARDATA(image);

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  x = (double *)calloc(1,n * sizeof(double));
  y = (double *)calloc(1,n * sizeof(double));
  if ((x == NULL) || (y == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    x[i] = xx;
    y[i] = yy;
  }
  fclose(fd);

  filled = atoi(argv[3]);

  if (!lellipseincl(x, y, n, &R, &S, &T, &U, &V, &Z))
  {
    fprintf(stderr, "%s: lellipseincl failed\n", argv[0]);
    exit(1);
  }

  //printf("%g %g %g %g %g %g\n", R, S, T, U, V, Z);

  free(x);
  free(y);

  ellipse = copyimage(image);
  El = UCHARDATA(ellipse);
  ldrawfilledellipse(ellipse, R, S, T, U, V, Z);
  if (!filled) 
  {
    if (! lborder(ellipse, 8))
    {
      fprintf(stderr, "lellipseincl: lborder failed\n");
      exit(1);
    }
  }
  for (i = 0; i < N; i++) Im[i] = max(Im[i],El[i]);
  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(ellipse);

  return 0;
} /* main */

