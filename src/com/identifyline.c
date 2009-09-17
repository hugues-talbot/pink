/* $Id: identifyline.c,v 1.1 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file identifyline.c

\brief identification of a best matching line from a set of 2D points

<B>Usage:</B> identifyline in.list out.list

<B>Description:</B>
Identifies the parameters (a,b) of the equation of the 2D line:
ax+b=y that minimizes the least square error between this line 
and the given points. Method: basic linear regression.

<B>Types supported:</B> list 1D, list 2D

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/

/* 
  Michel Couprie - juin 2009
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
  double *pbx, *pby, a, b;

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
  if ((type != 's') && (type != 'b'))
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  pbx = (double *)malloc(n * sizeof(double));
  pby = (double *)malloc(n * sizeof(double));

  if ((pbx == NULL) || (pby == NULL))
  {
    fprintf(stderr, "usage: %s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
    fscanf(fd, "%lf %lf\n", pbx+i, pby+i);

  fclose(fd);

  if (!lidentifyline(pbx, pby, n, &a, &b))
  {
    fprintf(stderr, "%s: lidentifyline failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("a = %g, b = %g\n", a, b);
#endif

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }
  fprintf(fd, "e %d\n", 2); 
  fprintf(fd, "%g %g\n", a, b); 
  fclose(fd);

  free(pbx);
  free(pby);

  return 0;
}

