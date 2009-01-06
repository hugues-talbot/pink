/* $Id: pgm2list.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file pgm2list.c

\brief converts from pgm format to list format

<B>Usage:</B> pgm2list image.pgm <e|s|b|n|B|N> out.list

<B>Description:</B>
Converts from pgm format to list format:
\li e: set of points
\li s: 1d digital signal
\li b: binary 2D image
\li n: grayscale 2D image
\li B: binary 3D image
\li N: grayscale 3D image

In formats \b e, \b b and \b B, only the points with non-null values are 
considered.

In formats \b s, \b n and \b N, all the points (even those having a 
null value) are considered.

Summary of list formats:
<pre>  
  e <n>       s <n>         b <n>         n <n>            B <n>            N <n>    
  x1          x1 v1         x1 y1         x1 y1 v1         x1 y1 z1         x1 y1 z1 v1
  x2    or    x2 v2   or    x2 y2   or    x2 y2 v2   or    x2 y2 z2   or    z2 x2 y2 v2
  ...         ...           ...           ...              ...              ...
  xn          xn vn         xn yn         xn yn vn         xn yn z3         z3 xn yn vn
</pre>
<B>Types supported:</B> byte 2D, byte 3D, int32_t 2D, int32_t 3D, float 2D, float 3D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/*
  Michel Couprie - septembre 1999
  update janvier 2006 (int32_t, float)
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  FILE *fd = NULL;
  int32_t rs, cs, ds, ps, N, x, y, z, n;
  char type;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s image.pgm <e|s|b|n|B|N> out.list \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
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

  type = argv[2][0];
  if ((type != 'e') && (type != 's') && (type != 'b') && (type != 'n') && (type != 'B') && (type != 'N'))
  {
    fprintf(stderr, "usage: %s image.pgm <e|s|b|n|B|N> out.list \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[argc - 1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc - 1]);
    exit(1);
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image);

    n = 0;                     /* compte le nombre de points non nuls */ 
    for (x = 0; x < N; x++) if (F[x]) n++;

    if (type == 'b') 
    {
      fprintf(fd, "b %d\n", n); 
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	{
	  if (F[y * rs + x]) fprintf(fd, "%d %d\n", x, y); 
	}
    }
    else if (type == 'n') 
    {
      fprintf(fd, "n %d\n", N);
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  fprintf(fd, "%d %d %d\n", x, y, F[y * rs + x]); 
    } else if (type == 'B') 
    {
      fprintf(fd, "B %d\n", n); 
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    if (F[z * ps + y * rs + x]) fprintf(fd, "%d %d %d\n", x, y, z); 
    }
    else if (type == 'N') 
    {
      fprintf(fd, "N %d\n", N);
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    fprintf(fd, "%d %d %d %d\n", x, y, z, F[z * ps + y * rs + x]); 
    }
    else if (type == 'e') 
    {
      fprintf(fd, "e %d\n", n); 
      for (x = 0; x < N; x++)
	if (F[x]) fprintf(fd, "%d\n", x); 
    }
    else if (type == 's') 
    {
      fprintf(fd, "s %d\n", N); 
      for (x = 0; x < N; x++)
	fprintf(fd, "%d %d\n", x, F[x]); 
    }
  }
  else
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(image);
    n = 0;                     /* compte le nombre de points non nuls */ 
    
    for (x = 0; x < N; x++) if (F[x]) n++;

    if (type == 'b') 
    {
      fprintf(fd, "b %d\n", n); 
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (F[y * rs + x]) fprintf(fd, "%d %d\n", x, y); 
    }
    else if (type == 'n') 
    {
      fprintf(fd, "n %d\n", N);
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  fprintf(fd, "%d %d %d\n", x, y, F[y * rs + x]); 
    } else if (type == 'B') 
    {
      fprintf(fd, "B %d\n", n); 
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    if (F[z * ps + y * rs + x]) fprintf(fd, "%d %d %d\n", x, y, z); 
    }
    else if (type == 'N') 
    {
      fprintf(fd, "N %d\n", N);
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    fprintf(fd, "%d %d %d %d\n", x, y, z, F[z * ps + y * rs + x]); 
    }
    else if (type == 'e') 
    {
      fprintf(fd, "e %d\n", n); 
      for (x = 0; x < N; x++)
	if (F[x]) fprintf(fd, "%d\n", x); 
    }
    else if (type == 's') 
    {
      fprintf(fd, "s %d\n", N); 
      for (x = 0; x < N; x++)
	fprintf(fd, "%d %d\n", x, F[x]); 
    }
  }
  else
  if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(image);
    n = 0;                     /* compte le nombre de points non nuls */ 
    
    for (x = 0; x < N; x++) if (F[x] != (float)0) n++;

    if (type == 'b') 
    {
      fprintf(fd, "b %d\n", n); 
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  if (F[y * rs + x] != (float)0) fprintf(fd, "%d %d\n", x, y); 
    }
    else if (type == 'n') 
    {
      fprintf(fd, "n %d\n", N);
      for (y = 0; y < cs; y++)
	for (x = 0; x < rs; x++)
	  fprintf(fd, "%d %d %g\n", x, y, F[y * rs + x]); 
    } else if (type == 'B') 
    {
      fprintf(fd, "B %d\n", n); 
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    if (F[z * ps + y * rs + x] != (float)0) fprintf(fd, "%d %d %d\n", x, y, z); 
    }
    else if (type == 'N') 
    {
      fprintf(fd, "N %d\n", N);
      for (z = 0; z < ds; z++)
	for (y = 0; y < cs; y++)
	  for (x = 0; x < rs; x++)
	    fprintf(fd, "%d %d %d %g\n", x, y, z, F[z * ps + y * rs + x]); 
    }
    else if (type == 'e') 
    {
      fprintf(fd, "e %d\n", n); 
      for (x = 0; x < N; x++)
	if (F[x] != (float)0) fprintf(fd, "%d\n", x); 
    }
    else if (type == 's') 
    {
      fprintf(fd, "s %d\n", N); 
      for (x = 0; x < N; x++)
	fprintf(fd, "%d %g\n", x, F[x]); 
    }
  }
  else
  {
    fprintf(stderr, "%s: bad data type\n", argv[0]);
    exit(1);
  }

  fclose(fd);
  freeimage(image);
  return 0;
}
