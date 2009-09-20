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
  double *x, *y, x1, y1;
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
    fprintf(stderr, "usage: %s : bat file format : %c \n", argv[0], type);
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
    fprintf(stderr, "usage: %s : 3D not yet implemented \n", argv[0]);
    exit(1);
  }
  return 0;
} /* main */


