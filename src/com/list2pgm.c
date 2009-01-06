/* $Id: list2pgm.c,v 1.3 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file list2pgm.c

\brief converts from point list representation to pgm

<B>Usage:</B> list2pgm in.list <in.pgm|rs cs ds> [scale] out.pgm

<B>Description:</B>

Reads the file <B>in.list</B>. This file must have one of the following formats:
<pre>  
  e <n>       s <n>         b <n>         n <n>            B <n>            N <n>    
  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1
  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2
  ...         ...           ...           ...              ...              ...
  xn          xn vn         xn yn         xn yn vn         xn yn zn         zn xn yn vn
</pre>
The formats <B>e</B>, <B>s</B>, <B>b</B>, <B>n</B>, <B>B</B>, and <B>N</B>, correspond respectively
to binary 1D, graylevel 1D, binary 2D, graylevel 2D, binary 3D and graylevel 3D images.
If a file name <B>in.pgm</B> is given, then the points read in <B>in.list</B> are 
inserted (if possible) into the image read in <B>in.pgm</B>. Else, they are inserted in
a new image, the dimensions of which are <B>rs</B>, <B>cs</B>, and <B>d</B>.

The optional parameter \b scale allows to scale the coordinates.

<B>Types supported:</B> list 1D, 2D, 3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - septembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, v, n, i;
  double xx, yy, zz, vv, scale;
  uint8_t *F;
  char type;

  if ((argc != 4) && (argc != 6) && (argc != 5) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.list <in.pgm|rs cs ds> [scale] out.pgm\n", argv[0]);
    exit(1);
  }

  if ((argc == 4) || (argc == 5))
  {
    image = readimage(argv[2]);
    if (image == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
    rs = rowsize(image);
    cs = colsize(image);
    ds = depth(image);
    ps = rs * cs;
    N = ps * ds;
    F = UCHARDATA(image);
  }
  else
  {
    rs = atoi(argv[2]);
    cs = atoi(argv[3]);
    ds = atoi(argv[4]);
    ps = rs * cs;
    N = ps * ds;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    F = UCHARDATA(image);
    memset(F, 0, N);
  }

  if ((argc == 5) || (argc == 7)) scale = atof(argv[argc-2]); else scale = 1.0;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 'e') && (type != 's') && (type != 'b') && (type != 'n') && (type != 'B') && (type != 'N'))
  {
    fprintf(stderr, "usage: %s: bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);

  if (type == 'e') 
  {
    if ((cs != 1) || (ds != 1))
    {
      fprintf(stderr, "%s: type e is for 1D images\n", type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf\n", &xx);
      xx *= scale;
      x = arrondi(xx);
      if ((x >= 0) && (x < rs))
        F[x] = 255;
    }
  }
  else if (type == 's') 
  {
    if ((cs != 1) || (ds != 1))
    {
      fprintf(stderr, "%s: type s is for 1D images\n", type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &vv);
      xx *= scale;
      x = arrondi(xx);
      v = arrondi(vv);
      if ((x >= 0) && (x < rs) && (v >= 0) && (v < 256))
        F[x] = v;
    }
  }
  else if (type == 'b') 
  {
    if (ds != 1)
    {
      fprintf(stderr, "%s: type b is for 2D images\n", type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &yy);
      xx *= scale;
      yy *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs))
        F[y * rs + x] = 255;
    }
  }
  else if (type == 'n') 
  {
    if (ds != 1)
    {
      fprintf(stderr, "%s: type n is for 2D images\n", type);
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &vv);
      xx *= scale;
      yy *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      v = arrondi(vv);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (v >= 0) && (v < 256))
        F[y * rs + x] = v;
    }
  } else if (type == 'B') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &zz);
      xx *= scale;
      yy *= scale;
      zz *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      z = arrondi(zz);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds))
        F[z * ps + y * rs + x] = 255;
    }
  }
  else if (type == 'N') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf %lf\n", &xx, &yy, &zz, &vv);
      xx *= scale;
      yy *= scale;
      zz *= scale;
      x = arrondi(xx);
      y = arrondi(yy);
      z = arrondi(zz);
      v = arrondi(vv);
      if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < ds) && (v >= 0) && (v < 256))
        F[z * ps + y * rs + x] = v;
    }
  }

  fclose(fd);
  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

