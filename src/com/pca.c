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
/*! \file pca.c

\brief principal component analysis

<B>Usage:</B> pca in.list out.list

<B>Description:</B>
Computes the main direction (2D or 3D vector) of a set of points in 2D or 3D space, by the method of Pricipal Component Analysis (PCA).

<B>Types supported:</B> list 2D, list 3D

<B>Category:</B> stats
\ingroup stats

\author Michel Couprie
*/

/*
%TEST pca %IMAGES/2dlist/binary/l2cloud1.list %RESULTS/pca_l2cloud1.list
*/

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lmoments.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i;
  char type;
  double *X, *Y, *Z, dx, dy, dz;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.list out.list \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 'b') && (type != 'B'))
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  
  if (type == 'b') // 2D
  {
    X = (double *)malloc(n * sizeof(double)); assert(X != NULL);
    Y = (double *)malloc(n * sizeof(double)); assert(Y != NULL);
    for (i = 0; i < n; i++)
      fscanf(fd, "%lf %lf\n", &(X[i]), &(Y[i]));
    if (! ldirectionprincipale2d(X, Y, n, &dx, &dy))
    {
      fprintf(stderr, "%s: ldirectionprincipale2d failed\n", argv[0]);
      exit(1);
    }
    fclose(fd);
    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "e %d\n", 2); 
    fprintf(fd, "%lf %lf\n", dx, dy); 
    fclose(fd);
  }
  else // 3D
  {
    X = (double *)malloc(n * sizeof(double)); assert(X != NULL);
    Y = (double *)malloc(n * sizeof(double)); assert(Y != NULL);
    Z = (double *)malloc(n * sizeof(double)); assert(Z != NULL);
    for (i = 0; i < n; i++)
      fscanf(fd, "%lf %lf %lf\n", &(X[i]), &(Y[i]), &(Z[i]));
    if (! ldirectionprincipale3d(X, Y, Z, n, &dx, &dy, &dz))
    {
      fprintf(stderr, "%s: ldirectionprincipale3d failed\n", argv[0]);
      exit(1);
    }
    fclose(fd);
    fd = fopen(argv[argc-1],"w");
    if (!fd)
    {
      fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
      exit(1);
    }
    fprintf(fd, "e %d\n", 3); 
    fprintf(fd, "%lf %lf %lf\n", dx, dy, dz); 
    fclose(fd);
  }

  return 0;
} /* main */
