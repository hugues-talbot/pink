/* $Id: meshconvert.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file meshconvert.c

\brief mesh format conversion

<B>Usage:</B> meshconvert in [resolution] out

<B>Description:</B>

Mesh format conversion. 
Available input formats: mcm, ifs, vtk.
Available output formats: mcm, vtk, pgm (points only).
If the output format is pgm, then the optional argument <b>resolution</b> 
gives the resolution of the grid (homogeneous in x, y and z dimensions). The 
default value is 1.0.

<B>Types supported:</B> mesh 3d

<B>Category:</B> mesh3d, convert
\ingroup  mesh3d, convert

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
#include <mcmesh.h>
#include <mciomesh.h>

#define VERBOSE
#define TOOMUCH

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  int32_t i, mode;
  int32_t formatin, formatout;
  double v, p1 = 0.0, p2 = 0.0;
  double resolution = 1.0;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in [resolution] out\n", argv[0]);
    exit(0);
  }
  if (argc == 4) resolution = atof(argv[2]);

  formatin = UNKNOWN;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".MCM") == 0) formatin = MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".mcm") == 0) formatin = MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".VTK") == 0) formatin = VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".vtk") == 0) formatin = VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".IFS") == 0) formatin = IFS;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".ifs") == 0) formatin = IFS;
  if (formatin == UNKNOWN)
  {
    fprintf(stderr, "%s: bad input file format\n", argv[0]);
    exit(0);
  }
  formatout = UNKNOWN;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".MCM") == 0) formatout = MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".mcm") == 0) formatout = MCM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".VTK") == 0) formatout = VTK;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".vtk") == 0) formatout = VTK;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".PGM") == 0) formatout = PGM;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".pgm") == 0) formatout = PGM;
  if (formatout == UNKNOWN)
  {
    fprintf(stderr, "%s: bad output file format\n", argv[0]);
    exit(0);
  }

  filein = fopen(argv[1],"r");
  if (formatin == MCM) LoadMeshMCM(filein);
  if (formatin == IFS) LoadMeshIFS(filein);
  if (formatin == VTK) LoadBuildVTK(filein);
  fclose(filein);
  mode = atoi(argv[2]);
  if (argc > 4) p1 = atof(argv[3]);
  if (argc > 5) p2 = atof(argv[4]);

  fileout = fopen(argv[argc-1],"w");
  if (formatout == MCM) SaveMeshMCM(fileout);
  if (formatout == VTK) 
  {
    genheaderVTK(fileout, "meshconvert output");    
    SaveMeshVTK(fileout);
  }
  if (formatout == PGM) 
  {
    // 0 1 2     ...     rs-2 rs-1
    //   |bxmin  ...  bxmax|
    meshbox B;
    meshvertex V;
    double xrange, yrange, zrange;
    int32_t rs, cs, ds, ps, N, x, y, z;
    struct xvimage *image;
    uint8_t *F;

    BoundingBoxMesh(&B);
#ifdef VERBOSE    
    printf("bxmin=%g bxmax=%g\n", B.bxmin, B.bxmax);
    printf("bymin=%g bymax=%g\n", B.bymin, B.bymax);
    printf("bzmin=%g bzmax=%g\n", B.bzmin, B.bzmax);
#endif
    xrange = B.bxmax - B.bxmin;
    yrange = B.bymax - B.bymin;
    zrange = B.bzmax - B.bzmin;
    rs = (int32_t)(xrange / resolution) + 2;
    cs = (int32_t)(yrange / resolution) + 2;
    ds = (int32_t)(zrange / resolution) + 2;
#ifdef VERBOSE    
    printf("rs=%d cs=%d ds=%d \n", rs, cs, ds);
#endif
    ps = rs * cs;
    N = ps * ds;
#ifdef TOOMUCH    
    if (N > 10000000) 
    {
      printf("maximum size exceeded, exit\n");
      exit(0);
    }
#endif
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(0);
    }
    F = UCHARDATA(image);
    memset(F, NDG_MIN, N);
    for (i = 0; i < Vertices->cur; i++)
    {                              /* pour chaque sommet de la grille */
      V = Vertices->v[i];
      x = (int32_t)((V.x - B.bxmin) / resolution) + 1;
      y = (int32_t)((V.y - B.bymin) / resolution) + 1;
      z = (int32_t)((V.z - B.bzmin) / resolution) + 1;
      F[z*ps + y*rs + x] = NDG_MAX;
    } /* for i */
    writeimage(image,argv[argc-1]);
  }
  fclose(fileout);
} /* main */
