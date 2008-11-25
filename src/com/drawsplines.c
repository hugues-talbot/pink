/* $Id: drawsplines.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file drawsplines.c

\brief draw spline segments which are specified by a text file

<B>Usage:</B> drawsplines in.pgm vect.txt out.pgm

<B>Description:</B>
The file \b vect.txt contains a list of splines under the format:<br>
nb_splines<br>
nb_points_spline_1 x11 y11 x12 y12 ...<br>
nb_points_spline_2 x21 y21 x22 y22 ...<br>
nb_points_spline_3 x31 y31 x32 y32 ...<br>
...<br>

<B>Types supported:</B> byte 2D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <ldraw.h>

double pol3(double *P, double v) { return P[0] + P[1]*v +  P[2]*v*v +  P[3]*v*v*v; }

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i, j;
  FILE *fd = NULL;
  int32_t nsplines, npoints;
  double *x, *y, *t, x1, y1;
  double Px[4], Py[4];
  double *X0, *X1, *X2, *X3,  *Y0, *Y1, *Y2, *Y3;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm splines.txt out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) > 1)
  {
    fprintf(stderr, "%s: image volumiques : pas implemente\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }
  
  fscanf(fd, "%d", &nsplines);

  for (i = 0; i < nsplines; i++)
  {
    fscanf(fd, "%d", &npoints);
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
    X0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    for (j = 0; j < npoints; j++)
    {
      fscanf(fd, "%lf%lf", &x1, &y1);
      x[j] = x1; y[j] = y1; t[j] = (double)j;
    }
    scn_solvespline(t, x, npoints, X0, X1, X2, X3);
    scn_solvespline(t, y, npoints, Y0, Y1, Y2, Y3);
    
    for (j = 0; j < npoints-1; j++)
    {
      Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
      Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];

      ldrawcubic2(image, (double *)Px, (double *)Py, 10, t[j], t[j+1]);
    }

    free(x); free(y); free(t); 
    free(X0); free(X1); free(X2); free(X3);
    free(Y0); free(Y1); free(Y2); free(Y3);
  }
  fclose(fd);

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


