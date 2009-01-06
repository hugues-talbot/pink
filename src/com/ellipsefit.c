/* $Id: ellipsefit.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file ellipsefit.c

\brief ellipse identification and drawing from spare points

<B>Usage:</B> ellipsefit in.pgm pts.list filled out.pgm

<B>Description:</B>
Identifies the ellipse E which minimizes the sum of the (approximate) distances
between the points in \b pts.list and E. 
If \b filled (int32_t) is set, then a filled ellipse is drawn.

Ref: 
Andrew W. Fitzgibbon, Maurizio Pilu, and Robert B. Fisher
Direct least-squares fitting of ellipses,
IEEE Transactions on Pattern Analysis and Machine Intelligence, 21(5), 476--480, May 1999

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* 
  Michel Couprie - avril 2002 
*/

//#define CHRONO

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclin.h>
#include <mcutil.h>
#include <lbresen.h>
#include <lellipsefit.h>
#ifdef CHRONO
#include <mcchrono.h>
#endif
/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  uint8_t *Im;
  int32_t rs, cs, d, N;
  FILE *fd = NULL;
  int32_t i, n;
  char type;
  double *x, *y, xx, yy, x0, y0, xa, ya, xb, yb;
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

#ifdef CHRONO
  chrono Chrono1;
  start_chrono(&Chrono1);
#endif
  if (!lellipsefit(x, y, n, &x0, &y0, &xa, &ya, &xb, &yb))
  {
    fprintf(stderr, "%s: lellipsefit failed\n", argv[0]);
    exit(1);
  }
#ifdef CHRONO
  printf("nb points = %d ; nb de usec ecoulees = %d\n", n, read_chrono(&Chrono1));
#endif

  //printf("%g %g %g %g %g %g\n", x0, y0, xa, ya, xb, yb);

  free(x);
  free(y);

  if (!filled)
    lellipse(Im, rs, cs, arrondi((xa-x0)), arrondi((ya-y0)), arrondi((xb-x0)), arrondi((yb-y0)), arrondi(x0), arrondi(y0));
  else
  {
    ellipse *ell = AllocEllipseList(rs, cs);
    lellipselist(ell, rs, cs, arrondi((xa-x0)), arrondi((ya-y0)), arrondi((xb-x0)), arrondi((yb-y0)), arrondi(x0), arrondi(y0));
    lplotfilledellipse(ell, Im);
    FreeEllipseList(ell);
  }
  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */

