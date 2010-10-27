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
/*! \file lengthcurve.c

\brief computes the length of a digital curve

<B>Usage:</B> lengthcurve curve.list [out.list]

<B>Description:</B>
Computes the length of a curve which is specified by the ordered list of its points.

<B>Types supported:</B> curve 2D, curve 3D

<B>Category:</B> geo
\ingroup geo

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <ltangents.h>

#define DEBUG

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t i, j;
  FILE *fd = NULL;
  int32_t npoints;
  char type;
  int32_t *X, *Y;
  int32_t *end;
  double *angle;
  double *mstd;
  double L;

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s curve.list [out.list] \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s: bad file format: %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d", &npoints);
  X = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(X != NULL);
  Y = (int32_t *)calloc(1,npoints*sizeof(int32_t)); assert(Y != NULL);

  for (i = 0; i < npoints; i++)
    fscanf(fd, "%d %d", &(X[i]), &(Y[i]));

  fclose(fd);
  
  end = (int32_t *)malloc(npoints * sizeof(int32_t)); assert(end != NULL);
  angle = (double *)malloc(npoints * sizeof(double)); assert(angle != NULL);
  mstd = (double *)malloc(npoints * sizeof(double)); assert(mstd != NULL);

  ExtractDSSs(npoints, X, Y, end, angle);

#ifdef DEBUG
  printf("npoints = %d\n", npoints);
  for (i = 0; i < npoints; i++)
    if (end[i] != -1)
      printf("DSS %d-%d, angle %g (%g)\n", i, end[i], angle[i], angle[i]*180/M_PI);
#endif

  LambdaMSTD(npoints, end, angle, mstd);

#ifdef DEBUG
  for (i = 0; i < npoints; i++)
    printf("point %d, angle %g (%g)\n", i, mstd[i], mstd[i]*180/M_PI);
#endif

  L = ComputeLength(npoints, mstd);

  if (argc == 3)
  {
    fd = fopen(argv[argc - 1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
      exit(1);
    }
    fprintf(fd, "e %d\n", 1); 
    fprintf(fd, "%g\n", L); 
    fclose(fd);
  }
  else printf("%g\n", L); 

  free(X);
  free(Y);
  free(end);
  free(angle);
  free(mstd);

  return 0;
} /* main */


