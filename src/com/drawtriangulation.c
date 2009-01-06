/* $Id: drawtriangulation.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file drawtriangulation.c

\brief draw a triangulation

<B>Usage:</B> drawtriangulation in.list in.pgm out.pgm

<B>Description:</B>
Reads a triangulation in file <B>in.list</B> under the following format:
<pre>
    G <n>
    x1 y1 v1 ec1 ns1 s11 s12 ... s1ns1
    x2 y2 v2 ec2 ns2 s21 s22 ... s1ns2
    ...
    xn yn vn ecn nsn sn1 sn2 ... s1nsn
</pre>  
where xi, yi are the coordinates of the ith vertex, vi is the associated 
value (if given in the input file), eci is a int32_t which indicates 
whether the vertex i belongs to the convex hull, nsi denotes the number
of adjacent vertices, and si1 si2 ... sins1 is the list of the indexes of
the adjacent vertices (counted from 0).

Draws the triangulation in image in.pgm, output image is out.pgm

<B>Types supported:</B> byte 2D

<B>See also:</B> delaunay.c

<B>Category:</B> geo, draw
\ingroup  geo, draw

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <lvoronoi.h>
#include <lbresen.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  FILE *fd = NULL;
  int32_t n, i, j;
  int32_t x, y, val, e, nbvois;
  char type;
  point * S;
  double * V;
  int32_t **vois; /* la table des listes de voisins (reperes par leur indice dans S) */
  int32_t *nv; /* la table des nombres de voisins */
  int32_t *ec; /* table de booleens indiquant les points de l'enveloppe convexe */
  struct xvimage * image;
  int32_t rs, cs, ds, N;
  uint8_t *F;
  int32_t Ax, Ay, Bx, By, v;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list in.pgm out.pgm\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if (type != 'G')
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  S = (point *)calloc(1, n * sizeof(point));
  V = (double *)calloc(1, n * sizeof(double));
  nv = (int32_t *)calloc(n, sizeof(int32_t));
  ec = (int32_t *)calloc(n, sizeof(int32_t));
  vois = (int32_t **)calloc(n, sizeof(int32_t *));
  if ((nv == NULL) || (ec == NULL) || (vois == NULL))
  {
    fprintf(stderr, "%s: malloc failed(1)\n", argv[0]);
    return 0;
  }
  for (i = 0; i < n; i++)
  {
    vois[i] = (int32_t *)calloc(n, sizeof(int32_t));  
    if (vois[i] == NULL)
    {
      fprintf(stderr, "%s: malloc failed(i = %d)\n", argv[0], i);
      return 0;
    }
  }

  for (i = 0; i < n; i++)
  {
    fscanf(fd, "%d %d %d %d %d\n", &x, &y, &val, &e, &nbvois);
    S[i].x = x;
    S[i].y = y;
    V[i] = val;
    ec[i] = e;
    nv[i] = nbvois;
    for (j = 0; j < nbvois; j++)
    {
      fscanf(fd, "%d\n", &e);
      vois[i][j] = e;
    }
  }
  fclose(fd);

  image = readimage(argv[2]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (depth(image) != 1)
  {
    fprintf(stderr, "%s: not yet implemented for 3d images\n", argv[0]);
    exit(1);
  }
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  F = UCHARDATA(image);

  for (i = 0; i < n; i++)
  {
    Ax = S[i].x;
    Ay = S[i].y;
    for (j = 0; j < nv[i]; j++) 
    {
      v = vois[i][j];
      Bx = S[v].x;
      By = S[v].y;
      lbresen(F, rs, Ax, Ay, Bx, By);
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  free(S);
  free(V);
  free(nv);
  free(ec);
  for (i = 0; i < n; i++) free(vois[i]);
  free(vois);
  return 0;
}

