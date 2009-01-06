/* $Id: circleincl.c,v 1.3 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file circleincl.c

\brief circle identification and drawing from spare points

<B>Usage:</B> circleincl in.pgm pts.list filled out.pgm

<B>Description:</B>

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author John Chaussard
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
#include <lballincl.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * circle;
  uint8_t *Im;
  uint8_t *El;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *points, xx, yy, R, X, Y;
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
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  points = (double *)calloc(1, 2 * n * sizeof(double));
  if (points == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    points[i+i] = xx;
    points[i+i+1] = yy;
  }
  fclose(fd);

  filled = atoi(argv[3]);

  if (!compute_min_disk_with_border_constraint(points, n, NULL, 0, &X, &Y, &R))
  {
    fprintf(stderr, "%s: compute_min_disk_with_border_constraint failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("center %g,%g radius %g\n", X, Y, R);
#endif

  free(points);

  circle = copyimage(image);
  El = UCHARDATA(circle);
  ldrawball(circle, R, X, Y, 0);
  if (!filled) 
  {
    if (! lborder(circle, 8))
    {
      fprintf(stderr, "lcircleincl: lborder failed\n");
      exit(1);
    }
  }
  for (i = 0; i < N; i++) Im[i] = max(Im[i],El[i]);
  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(circle);

  return 0;
} /* main */

