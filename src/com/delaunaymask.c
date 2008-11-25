/* $Id: delaunaymask.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file delaunaymask.c

\brief delaunay triangulation



<B>Usage:</B> delaunaymask in.list mask.pgm out.list


<B>Description:</B>
Reads a point list in file <B>in.list</B> under the following format:
<pre>  
    b <n>         n <n>    
    x1 y1         x1 y1 v1
    x2 y2   ou    x2 y2 v2
    ...           ...
    xn yn         xn yn vn
</pre>  

Computes a Delaunay triangulation and stores the resulting graph 

into file <B>out.graph</B> under the following format:
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

<B>Types supported:</B> byte 2D

<B>Category:</B> geo
\ingroup  geo

\warning algorithm in O(n^2)

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
#define isnotinmask(x,y) (!M[y*rss+x])

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  FILE *fd = NULL;
  int32_t n, i, j, k, rss, css, N;
  double xx, yy, vv;
  char type;
  point * S;
  double * V;
  int32_t **v; /* la table des listes de voisins (reperes par leur indice dans S) */
  int32_t *nv; /* la table des nombres de voisins */
  int32_t *ec; /* table de booleens indiquant les points de l'enveloppe convexe */
  struct xvimage * mask;	
  int32_t Cx, Cy, Dx, Dy, vois,longlpmax;
  uint8_t *M;
  int32_t npoints;
  int32_t newnv;
  int32_t *del, *lp;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in.list mask.pgm out.graph\n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1],"r");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(1);
  }

  fscanf(fd, "%c", &type);
  if ((type != 'b') && (type != 'n'))
  {
    fprintf(stderr, "usage: %s : bad file format : %c \n", argv[0], type);
    exit(1);
  }

  fscanf(fd, "%d\n", &n);
  S = (point *)calloc(1,n * sizeof(point));
  V = (double *)calloc(1,n * sizeof(double));

  if (type == 'b') 
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf\n", &xx, &yy);
      S[i].x = xx;
      S[i].y = yy;
      V[i] = 255.0;
    }
  }
  else
  {
    for (i = 0; i < n; i++)
    {
      fscanf(fd, "%lf %lf %lf\n", &xx, &yy, &vv);
      S[i].x = xx;
      S[i].y = yy;
      V[i] = vv;
    }
  }
  fclose(fd);

  if (!ldelaunay(S, n, &v, &nv, &ec))
  {
    fprintf(stderr, "%s : ldelaunay failed \n", argv[0]);
    exit(1);
  }

  if (!ltriang_delaunay(S, n, v, nv, ec))
  {
    fprintf(stderr, "%s : ltriang_delaunay failed \n", argv[0]);
    exit(1);
  }

  fd = fopen(argv[3],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[3]);
    exit(1);
  }

  mask = readimage(argv[2]);
  if (mask == NULL)
  {
    fprintf(stderr, "%s: lecture mask �chou�\n", argv[0]);
    exit(1);
  }   
  M = UCHARDATA(mask);
  rss = mask->row_size;
  css = mask->col_size;
  N = rss * css; 
  longlpmax=max(rss,css);
  lp = (int32_t*)malloc(longlpmax*sizeof(int32_t));
  del = (int32_t*)malloc(longlpmax*sizeof(int32_t));
  npoints = 0;

  fprintf(fd, "G %d\n", n);

  for (i = 0; i < n; i++)
  {
    Cx = arrondi(S[i].x);
    Cy = arrondi(S[i].y);
    if (isnotinmask(Cx,Cy))
    {
      fprintf(fd, "%g %g %g %d ", S[i].x, S[i].y, V[i], ec[i]);	
      newnv=nv[i];
      for (k = 0; k < nv[i] ; k++) del[k]=0;
      for (j = 0; j < nv[i]; j++) 
      {
	vois = v[i][j];
	Dx = arrondi(S[vois].x);
	Dy = arrondi(S[vois].y);
	lbresenlist(rss, Cx, Cy, Dx, Dy, lp, &npoints);
	for (k = 0; k < npoints; k++)
	{
	  if (M[lp[k]]) // si le point du segment est dans le masque
	  {
	    del[j]=1;
	    newnv--;
	    goto continuer;
	  }
	}
      continuer: ;
      }
      fprintf(fd, "%d ", newnv);
      for (k = 0; k < nv[i]; k++)
      {
	if (del[k]==0)
	  fprintf(fd, "%d ", v[i][k]);
      }
      fprintf(fd, "\n");
    }
    else
      fprintf(fd, "%g %g %g %d %d\n", S[i].x, S[i].y, V[i], ec[i], 0);	
  }
  fclose(fd);
  free(lp);
  free(del);
}
