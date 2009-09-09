/* $Id: longestplateau.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file longestplateau.c

\brief finds the location of the longest plateau in 1D sequence

<B>Usage:</B> longestplateau in.list [tolerance] out.list

<B>Description:</B>

Reads the sequence S from the file <B>in.list</B>. 
This file must have the following format:
<pre>  
  e &lt;n&gt;
  x1
  x2
  ...
  xn
</pre>
The tolerance t (float) is given by the parameter <B>tolerance</B>
(default value is 0).
A <i>plateau</i> is a subsequence P of S formed by consecutive elements of P, 
between indices i and j, and such that max{P[k];i<=k<=j} - min{P[k];i<=k<=j} <= t.
The program returns the base index and length of the first occurence of a plateau with 
maximal length in S.

<B>Types supported:</B> list 1D

<B>Category:</B> signal
\ingroup signal

\author Michel Couprie
*/

/* 
  Michel Couprie - avril 2006
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int32_t longest(double *tab, int32_t n, int32_t i, double tol)
/* =============================================================== */
{
  double minv, maxv; 
  int32_t j = i;
  minv = maxv = tab[i];
  while ((j < n) && ((maxv - minv) <= tol)) 
  {
    j++;
    if (tab[j] < minv) minv = tab[j];
    else if (tab[j] > maxv) maxv = tab[j];
  }
  return j - i;
} // longest()

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i, l, imax, lmax;
  char type;
  double v, tol, *tab;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.list [tolerance] out.list\n", argv[0]);
    exit(1);
  }

  if (argc == 4) tol = atof(argv[2]); else tol = 0.0;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'e')
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  tab = (double *)malloc(n * sizeof(double));
  if (tab == NULL)
  {
    fprintf(stderr, "%s: malloc failed \n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf\n", &v);
    tab[i] = v;
  }
  fclose(fd);

  imax = 0; lmax = longest(tab, n, 0, tol);
  for (i = 1; i < n; i++)  
  {
    l = longest(tab, n, i, tol);
    if (l > lmax) { imax = i; lmax = l; }
  }
  free(tab);

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }
  fprintf(fd, "e 2\n");
  fprintf(fd, "%d %d\n", imax, lmax);
  fclose(fd);

  return 0;
}

