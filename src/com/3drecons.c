/* $Id: 3drecons.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file 3drecons.c

\brief geodesic reconstruction in a 3d khalimsky order 

<B>Usage:</B> 3drecons g.list f.pgm out.pgm

<B>Description:</B>
Geodesic reconstruction (in the sense of the neighborhood relation theta) 
of the set of points represented by the list \b g.list in the set \b f.pgm .

<B>Types supported:</B> byte 3d

<B>Category:</B> orders
\ingroup  orders

\author Michel Couprie
*/
/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mckhalimsky3d.h>
#include <l3dkhalimsky.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * k;
  FILE *fd;
  int32_t n;
  int32_t *tab;  
  double xx, yy, zz;
  int32_t x, y, z;
  int32_t rs, cs, ds, ps, N;
  int32_t i;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s g.list f.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  k = readimage(argv[2]);  
  if (k == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  rs = rowsize(k);
  cs = colsize(k);
  ds = depth(k);
  ps = rs * cs;
  N = ps * ds;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[2]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'B')
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  tab = (int32_t *)calloc(1,n * sizeof(int32_t));
  if (tab == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &zz);
    x = arrondi(xx);
    y = arrondi(yy);
    z = arrondi(zz);
    if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds))
      tab[i] = x + y * rs + z * ps;
    else
    {      
      fprintf(stderr, "%s: point outside image: %d %d %d\n", argv[0], x, y, z);
      exit(1);
    }
  }
  fclose(fd);

  if (! l3drecons(k, tab, n))
  {
    fprintf(stderr, "%s: function l3drecons failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(k, argv[3]);
  freeimage(k);
  free(tab);

  return 0;
} /* main */
