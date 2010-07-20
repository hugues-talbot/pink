/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file drawspline.c

\brief draw spline orientations in a vector field

<B>Usage:</B> drawspline in.pgm in.spline out.pgm

<B>Description:</B>
Draws spline orientations in a vector field. 
The spline is specified by its control points in a text file.
The parameter \b in.pgm gives a vector field into which the spline is to be drawn.
The file format for \b in.spline is the following for 2D:

s n+1 (where n+1 denotes the number of control points)<br>
x1 y1<br>
...<br>
xn+1 yn+1<br>
C0X1 C0Y1 C1X1 C1Y1 C2X1 C2Y1 C3X1 C3Y1<br>
...<br>
C0Xn C0Yn C1Xn C1Yn C2Xn C2Yn C3Xn C3Yn<br>

and in the 3D case:

S n+1 (where n+1 denotes the number of control points)<br>
x1 y1 z1<br>
...<br>
xn+1 yn+1 zn+1<br>
C0X1 C0Y1 C0Z1 C1X1 C1Y1 C1Z1 C2X1 C2Y1 C2Z1 C3X1 C3Y1 C3Z1<br>
...<br>
C0Xn C0Yn C0Zn C1Xn C1Yn C1Zn C2Xn C2Yn C2Zn C3Xn C3Yn C3Zn<br>

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> draw geo
\ingroup  draw geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>
#include <mcutil.h>
#include <ldraw.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * field;
  int32_t j;
  FILE *fd = NULL;
  int32_t npoints;
  double *x, *y, *z, *t, len = 0.0;
  double xx, yy, zz;
  double Px[4], Py[4], Pz[4];
  double *X0, *X1, *X2, *X3, *Y0, *Y1, *Y2, *Y3, *Z0, *Z1, *Z2, *Z3;
  double tx, ty, tz, tn;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.pgm spline.txt out.pgm \n", argv[0]);
    exit(1);
  }

  field = readimage(argv[1]);
  if (field == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[2],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 's') && (type != 'S'))
  {
    fprintf(stderr, "%s : bad file format : %c \n", argv[0], type);
    exit(1);
  }
  fscanf(fd, "%d", &npoints);

  if (npoints < 2)
  {
    fprintf(stderr, "%s: not enough points\n", argv[0]);
    exit(1);
  }

  if (type == 's')
  {
    double p0, p1, p2, p3, q0, q1, q2, q3;

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
      fscanf(fd, "%lf%lf", &xx, &yy);
      x[j] = xx; y[j] = yy; t[j] = (double)j;
    }
    for (j = 0; j < npoints-1; j++)
    {
      fscanf(fd, "%lf%lf%lf%lf%lf%lf%lf%lf", &p0, &q0, &p1, &q1, &p2, &q2, &p3, &q3);
      X0[j] = p0; X1[j] = p1; X2[j] = p2; X3[j] = p3; 
      Y0[j] = q0; Y1[j] = q1; Y2[j] = q2; Y3[j] = q3; 
    }
    
    for (j = 0; j < npoints-1; j++)
    {
      Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
      Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
      ldrawtangents2d(field, (double *)Px, (double *)Py, 10, t[j], t[j+1]);
    }

    free(x); free(y); free(t); 
    free(X0); free(X1); free(X2); free(X3);
    free(Y0); free(Y1); free(Y2); free(Y3);
  }
  else if (type == 'S')
  {
    double p0, p1, p2, p3, q0, q1, q2, q3, r0, r1, r2, r3;
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    z = (double *)calloc(1,npoints*sizeof(double));
    t = (double *)calloc(1,npoints*sizeof(double));
    X0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    X3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Y3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z0 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z1 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z2 = (double *)calloc(1,(npoints-1)*sizeof(double));
    Z3 = (double *)calloc(1,(npoints-1)*sizeof(double));
    for (j = 0; j < npoints; j++)
    {
      fscanf(fd, "%lf%lf%lf", &xx, &yy, &zz);
      x[j] = xx; y[j] = yy; z[j] = zz; t[j] = (double)j;
    }
    for (j = 0; j < npoints-1; j++)
    {
      fscanf(fd, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf", &p0, &q0, &r0, &p1, &q1, &r1, &p2, &q2, &r2, &p3, &q3, &r3);
      X0[j] = p0; X1[j] = p1; X2[j] = p2; X3[j] = p3; 
      Y0[j] = q0; Y1[j] = q1; Y2[j] = q2; Y3[j] = q3; 
      Z0[j] = r0; Z1[j] = r1; Z2[j] = r2; Z3[j] = r3; 
    }
    
    for (j = 0; j < npoints-1; j++)
    {
      Px[0] = X0[j]; Px[1] = X1[j]; Px[2] = X2[j]; Px[3] = X3[j];
      Py[0] = Y0[j]; Py[1] = Y1[j]; Py[2] = Y2[j]; Py[3] = Y3[j];
      Pz[0] = Z0[j]; Pz[1] = Z1[j]; Pz[2] = Z2[j]; Pz[3] = Z3[j];
      ldrawtangents3d(field, (double *)Px, (double *)Py, (double *)Pz, 10, t[j], t[j+1]);
    }

    free(x); free(y); free(z); free(t); 
    free(X0); free(X1); free(X2); free(X3);
    free(Y0); free(Y1); free(Y2); free(Y3);
    free(Z0); free(Z1); free(Z2); free(Z3);
  }
  fclose(fd);

  writeimage(field, argv[argc-1]);
  freeimage(field);

  return 0;
} /* main */

