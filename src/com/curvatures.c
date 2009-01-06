/* $Id: curvatures.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file curvatures.c

\brief compute the curvatures of a natural cubic spline

<B>Usage:</B> curvatures spline.txt n out.txt

<B>Description:</B>
Samples the spline in \b spline.txt and compute the curvature at each sample point.
The file \b spline.txt contains a 2D or 3D spline under the format:<br>
s nbpoints<br>  
x1 y1<br>
x2 y2<br>
x3 y3<br>
...<br>
or:
S nbpoints<br>  
x1 y1 z1<br>
x2 y2 z2<br>
x3 y3 z3<br>
...<br>

<B>Types supported:</B> 2D, 3D splines

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcsplines.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t j, nsamples, npoints;
  FILE *fd = NULL;
  double *x, *y, *z, x1, y1, z1;
  double *sk, *rhok;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s spline.txt n out.txt \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 's') && (type != 'S'))
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d", &npoints);
  if (type == 's')
  {
    x = (double *)calloc(1,npoints*sizeof(double));
    y = (double *)calloc(1,npoints*sizeof(double));
    for (j = 0; j < npoints; j++)
    {
      fscanf(fd, "%lf%lf", &x1, &y1);
      x[j] = x1; y[j] = y1;
    }
    fclose(fd);

    nsamples = atoi(argv[2])+1;

    sk = (double *)calloc(1,nsamples*sizeof(double));
    rhok = (double *)calloc(1,nsamples*sizeof(double));

    scn_curvatures(x, y, npoints, nsamples, sk, rhok);

    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
//    fprintf(fd, "%d\n", nsamples-1);
    for (j = 1; j < nsamples; j++)
      fprintf(fd, "%lf %lf\n", sk[j], rhok[j]);
    fclose(fd);

    free(x); free(y); free(sk); free(rhok);
  }
  else
  {
    fprintf(stderr, "usage: %s : 3D not yet implemented \n");
    exit(1);
  }
  return 0;
} /* main */


