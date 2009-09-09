/* $Id: signal2pgm.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file signal2pgm.c

\brief converts from signal representation to pgm

<B>Usage:</B> signal2pgm in.list [scale] out.pgm

<B>Description:</B>

Reads the file <B>in.list</B>. This file must have one of the following formats:
<pre>  
    s &lt;n&gt; 
    x1 y1 
    x2 y2 
    ...   
    xn yn 
</pre>

The optional parameter \b scale allows to scale the y coordinates.

<B>Types supported:</B> signal 1D

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/

/* 
  Michel Couprie - decembre 2004
*/

#include <stdio.h>
#include <stdint.h>
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
  int32_t rs, cs, N, x, y, v, n, i;
  double xx, yy, vv, scale;
  uint8_t *F;
  char type;
  int32_t *signal;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.list [scale] out.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 4) scale = atof(argv[2]); else scale = 1.0;

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 's')
  {
    fprintf(stderr, "usage: %s : bat file format : %c \n", type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  signal = (int32_t *)calloc(1,n * sizeof(int32_t));
  if (signal == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }









PAS FINI !!!!!!!!!







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

  fclose(fd);



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




  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return 0;
}

