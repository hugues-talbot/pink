/* $Id: meshflatten.c,v 1.1 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file meshflatten.c

  \brief mesh flattening

<B>Usage:</B> meshflatten in out

<B>Description:</B>

<B>Types supported:</B> mesh 3d

<B>Category:</B> mesh3d
\ingroup  mesh3d

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcrbtp.h>
#include <mcmesh.h>
#include <mciomesh.h>
#include <lgeo.h>

#define VERBOSE
#define TOOMUCH

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  int32_t i, n;
  int32_t formatin, formatout;
  double resolution = 1.0;
  double *pbx, *pby, *pbz, a, b, c, d, A, B, C, D;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in out\n", argv[0]);
    exit(0);
  }

  formatin = UNKNOWN;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".MCM") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".mcm") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".VTK") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".vtk") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".IFS") == 0) formatin = T_IFS;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".ifs") == 0) formatin = T_IFS;
  if (strcmp(argv[1]+strlen(argv[1])-5, ".CGAL") == 0) formatin = T_CGAL;
  if (strcmp(argv[1]+strlen(argv[1])-5, ".cgal") == 0) formatin = T_CGAL;
  if (formatin == UNKNOWN)
  {
    fprintf(stderr, "%s: bad input file format\n", argv[0]);
    exit(0);
  }
  formatout = UNKNOWN;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".MCM") == 0) formatout = T_MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".mcm") == 0) formatout = T_MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".VTK") == 0) formatout = T_VTK;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".vtk") == 0) formatout = T_VTK;
  if (formatout == UNKNOWN)
  {
    fprintf(stderr, "%s: bad output file format\n", argv[0]);
    exit(0);
  }

  filein = fopen(argv[1],"r");
  if (formatin == T_MCM) LoadMeshMCM(filein);
  if (formatin == T_IFS) LoadMeshIFS(filein);
  if (formatin == T_VTK) LoadBuildVTK(filein);
  if (formatin == T_CGAL) LoadMeshCGAL(filein);
  fclose(filein);

  n = Vertices->cur;
  pbx = (double *)malloc(n * sizeof(double));
  pby = (double *)malloc(n * sizeof(double));
  pbz = (double *)malloc(n * sizeof(double));

  if ((pbx == NULL) || (pby == NULL) || (pbz == NULL))
  {
    fprintf(stderr, "usage: %s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (i = 0; i < n; i++)
  {
    pbx[i] = Vertices->v[i].x;
    pby[i] = Vertices->v[i].y;
    pbz[i] = Vertices->v[i].z;
  }

  if (!lidentifyplane(pbx, pby, pbz, n, &a, &b, &c, &d))
  {
    fprintf(stderr, "%s: lidentifyplane failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("a = %g, b = %g, c = %g, d = %g\n", a, b, c, d);
#endif

  // projection des vertices sur le plan ax+by+cz+d=0

  D = a*a + b*b + c*c;
  A = b*b + c*c;
  B = a*a + c*c;
  C = a*a + b*b;
  for (i = 0; i < n; i++)
  {
    Vertices->v[i].x = (A*pbx[i]-a*b*pby[i]-a*c*pbz[i]-a*d) / D;
    Vertices->v[i].y = (B*pby[i]-b*a*pbx[i]-b*c*pbz[i]-b*d) / D;
    Vertices->v[i].z = (C*pbz[i]-c*a*pbx[i]-c*b*pby[i]-c*d) / D;
  }

  fileout = fopen(argv[argc-1],"w");
  if (formatout == T_MCM) SaveMeshMCM(fileout);
  if (formatout == T_VTK) 
  {
    genheaderVTK(fileout, (char *)"meshflatten output");    
    SaveMeshVTK(fileout);
  }
  fclose(fileout);
} /* main */
