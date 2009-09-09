/* $Id: identifyplane.c,v 1.2 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file identifyplane.c

\brief identification of a best matching plane from a set of 3D points

<B>Usage:</B> identifyplane in.list out.list

<B>Description:</B>
Identifies the parameters (a, b, c, d) of the equation of the 3D plane:
ax+by+cz+d=0 that minimizes the least square error between this plane 
and the given points. Method: basic linear regression.

<B>Types supported:</B> list 3D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* 
  Michel Couprie - decembre 2008
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclin.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i;
  char type;
  double *pbx, *pby, *pbz, a, b, c, d;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.list out.list\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'B')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  pbx = (double *)malloc(n * sizeof(double));
  pby = (double *)malloc(n * sizeof(double));
  pbz = (double *)malloc(n * sizeof(double));

  if ((pbx == NULL) || (pby == NULL) || (pbz == NULL))
  {
    fprintf(stderr, "usage: %s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
    fscanf(fd, "%lf %lf %lf\n", pbx+i, pby+i, pbz+i);

  fclose(fd);

  if (!lidentifyplane(pbx, pby, pbz, n, &a, &b, &c, &d))
  {
    fprintf(stderr, "%s: lidentifyplane failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("a = %g, b = %g, c = %g, d = %g\n", a, b, c, d);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "e %d\n", 4); 
  fprintf(fd, "%g %g %g %g\n", a, b, c, d); 
  fclose(fd);

  free(pbx);
  free(pby);
  free(pbz);

  return 0;
}

