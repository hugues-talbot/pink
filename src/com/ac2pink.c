/* $Id: ac2pink.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file ac2pink.c

\brief converts a 3D scene from AC3D format to Pink 3Dscene format

<B>Usage:</B> ac2pink in.ac out.3sc

<B>Description:</B>

Reads the file <B>in.ac</B>. This file must be in the AC3D format.
Generates the output file <B>out.3sc</B> in the 3Dscene format : 

FILE ::= '3Dscene' <N> <OBJ>*
OBJ ::= <LINE> | <CLOSEDLINE> | <SPLINE> | <CLOSEDSPLINE> | <POLYGON>
LINE ::= 'line' <N> <POINT>*
CLOSEDLINE ::= 'closedline' <N> <POINT>*
SPLINE ::= 'spline' <N> <POINT>*
CLOSEDSPLINE ::= 'closedspline' <N> <POINT>*
POLYGON ::= 'polygon' <N> <POINT>*
N ::= ascii_coded_integer
POINT ::= ascii_coded_float ascii_coded_float ascii_coded_float

<B>Types supported:</B> AC3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define TYP_OBJ_LINE       0
#define TYP_OBJ_CLOSEDLINE 1
#define TYP_OBJ_POLYGON    2

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  FILE *fdin = NULL;
  FILE *fdout = NULL;
  int32_t I, i, N, n;
  double x, y, z;
  double x0, y0, z0;
  char buf[1024];
  int32_t typobj, ret;
  double x_scale = 50, y_scale = 50, z_scale = 50;
  double x_offset = 50, y_offset = 50, z_offset = 50;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.ac out.3sc\n", argv[0]);
    exit(1);
  }

  fdin = fopen(argv[1],"r");
  if (!fdin)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  ret = fscanf(fdin, "%s", buf);
  if (strncmp(buf, "AC3D", 4) != 0)
  {
    fprintf(stderr, "%s : bad file format 1 : %s\n", argv[0], buf);
    exit(1);
  }

  while ((ret != EOF) && (strncmp(buf, "world", 5) != 0)) ret = fscanf(fdin, "%s", buf); 
  ret = fscanf(fdin, "%s", buf); 
  if (strncmp(buf, "kids", 4) != 0)
  {
    fprintf(stderr, "%s : bad file format 2 : %s\n", argv[0], buf);
    exit(1);
  }
  ret = fscanf(fdin, "%d", &N);

  fdout = fopen(argv[2],"w");
  if (!fdout)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fprintf(fdout, "3Dscene %d\n", N);

  for (I = 0; I < N; I++)
  {
    while ((ret != EOF) && (strncmp(buf, "OBJECT", 6) != 0)) ret = fscanf(fdin, "%s", buf); 
    ret = fscanf(fdin, "%s", buf);
    if (strncmp(buf, "poly", 4) != 0) break;
    ret = fscanf(fdin, "%s", buf);
    if (strncmp(buf, "name", 4) != 0)
    {
      fprintf(stderr, "%s : bad file format 3 : %s\n", argv[0], buf);
      exit(1);
    }
    ret = fscanf(fdin, "%s", buf);
    if (strncmp(buf+1, "line", 4) == 0) typobj = TYP_OBJ_LINE; else
    if (strncmp(buf+1, "closedline", 10) == 0) typobj = TYP_OBJ_CLOSEDLINE; else
    {
      fprintf(stderr, "%s : bad object name : %s \n", argv[0], buf);
      exit(1);
    }
    ret = fscanf(fdin, "%s", buf);
    if (strncmp(buf, "loc", 3) != 0)
    {
      fprintf(stderr, "%s : bad file format : %s instead of loc\n", argv[0], buf);
      exit(1);
    }
    fscanf(fdin, "%lf%lf%lf", &x0, &y0, &z0);
    fscanf(fdin, "%s", buf);
    if (strncmp(buf, "numvert", 7) != 0)
    {
      fprintf(stderr, "%s : bad file format : %s instead of numvert\n", argv[0], buf);
      exit(1);
    }
    ret = fscanf(fdin, "%d", &n);
    switch (typobj)
    {
      case TYP_OBJ_LINE: fprintf(fdout, "line %d\n", n); break;
      case TYP_OBJ_CLOSEDLINE: fprintf(fdout, "closedline %d\n", n); break;
    }
    for (i = 0; i < n; i++)
    {
      ret = fscanf(fdin, "%lf%lf%lf", &x, &y, &z);
      fprintf(fdout, "%lf %lf %lf\n", x_offset + x_scale * (x0+x), y_offset + y_scale * (y0+y), z_offset + z_scale * (z0+z));
    }
  }

  fclose(fdin);
  fclose(fdout);
  return 0;
}

