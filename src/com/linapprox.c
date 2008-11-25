/* $Id: linapprox.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* \file linapprox.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>linapprox</B> - 

SYNOPSIS

<B>linaprox</B> in.list epsilon out.list

DESCRIPTION

    From: "Optimum Uniform Piecewise Linear Approximation of Planar Curves", J.G. Dunham, 
    IEEE PAMI v8 n1 1986

CLASS 

geo

*/

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llinapprox.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  double *x;
  double *y; 
  int32_t i, n;
  double eps;
  FILE *fd = NULL;
  double xx, yy;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list epsilon out.list \n", argv[0]);
    exit(1);
  }

  eps = atof(argv[2]);

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'b')
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  x= (double *)calloc(1,n * sizeof(double));
  y= (double *)calloc(1,n * sizeof(double));

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf\n", &xx, &yy);
    x[i] = xx;
    y[i] = yy;
  }

  fclose(fd);

  SJCurveAproximate(x,y,n,eps);

  free(x);
  free(y);
  

  return 0;
} /* main */


