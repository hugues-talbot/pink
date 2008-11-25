/* $Id: meshregul.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file meshregul.c

\brief mesh smoothing

<B>Usage:</B> meshregul in mode [param1 [param2]] out

<B>Description:</B>

Mesh smoothing. 
The possible choices for parameter <B>mode</B> are:
\li 0: Laplacian smoothing (param1 = number of iterations, default value 5)
\li 1: Hamam method [HC07] (SOWA, param1 = theta, default value 1.0)
\li 2: Vollmer, Mencl and Mueller method [VMM] (param1 = alpha, param2 = beta, default values 0.1 and 0.6 resp.)
\li 3: Hamam method [HC07], variant: matrix A used instead of AA (FOWA, param1 = theta, default value 1.0)
\li 4: Hamam method [HC07], variant: theta = infty (SOWA, param1 = number of iterations, default value 5)
\li 5: Hamam method [HC07], conjugate gradient algorithm (SOWA, param1 = theta, default value 1.0)
\li 6: Taubin method [Tau95] (param1 = lambda, param2 = mu, param3 = N, default values 0.33 and -0.34 and 60)

[HC07] Y. Hamam and M. Couprie, "An optimisation-based approach to mesh smoothing: reformulation and extensions", to appear, 2007.

[VMM99] J. Vollmer and R. Mencl and H. Muller,
"Improved Laplacian Smoothing of Noisy Surface Meshes",
Computer Graphics Forum, Vol. 18, N. 3, pp. 131-138, 1999.

[Tau95] G. Taubin, 
"Curve and surface smoothing without shrinkage"
Proceedings of the Fifth International Conference on Computer Vision,
pp. 852-857, 1995.

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
#include <mcmesh.h>
#include <mciomesh.h>
#include <mcgeo.h>

//#define VERBOSE
#define EVALUATION
#define NOISEPARAM 0.05

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  int32_t i, mode;
  int32_t formatin, formatout;
  double v, p1 = -1.0, p2 = -1.0, p3 = -1.0, md0, md, err;
#ifdef EVALUATION
  double xc, yc, zc;
  double MSE;
#endif

  if ((argc != 4) && (argc != 5) && (argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in mode [param1 [param2 [param3]]] out\n0: Laplacian, 1: Hamam, 2: Vollmer, 3,4,5: Hamam (variants), 6: Taubin\n", argv[0]);
    exit(0);
  }
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
  if (formatout == UNKNOWN)
  {
    fprintf(stderr, "%s: bad output file format\n", argv[0]);
    exit(0);
  }

  filein = fopen(argv[1],"r");
  if (filein == NULL)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[1]);
    exit(0);
  }
  if (formatin == MCM) LoadMeshMCM(filein);
  if (formatin == IFS) LoadBuildIFS(filein);
  if (formatin == VTK) LoadBuildVTK(filein);
  fclose(filein);
  mode = atoi(argv[2]);
  if (argc > 4) p1 = atof(argv[3]);
  if (argc > 5) p2 = atof(argv[4]);
  if (argc > 6) p3 = atof(argv[5]);

#ifdef EVALUATION
  AddNoiseMesh(NOISEPARAM);
  IsobarMesh(&xc, &yc, &zc);
  md0 = MeanDistCenter();

  fileout = fopen("_noisemesh","w");
  genheaderVTK(fileout, "mcube output");    
  SaveMeshVTK(fileout);
  fclose(fileout);
#endif

#ifdef EVALUATION
  printf("%g\t", p1);
#endif

  if (mode == 0) { if (p1 == -1.0) p1 = 5; RegulMeshLaplacian((int32_t)p1); }
  else if (mode == 1) { if (p1 == -1.0) p1 = 1.0; RegulMeshHamam(p1); } // A^2
  else if (mode == 2) { if (p1 == -1.0) p1 = 0.1; if (p2 == -1.0) p2 = 0.6; RegulMeshHC(p1, p2); }
  else if (mode == 3) { if (p1 == -1.0) p1 = 1.0; RegulMeshHamam1(p1); } // A
  else if (mode == 4) { if (p1 == -1.0) p1 = 5; RegulMeshHamam2((int32_t)p1); } // A^2, theta = infty
  else if (mode == 5) { if (p1 == -1.0) p1 = 1.0; RegulMeshHamam3(p1); } // gradient conjugue
  else if (mode == 6) { if (p1 == -1.0) p1 = 0.33; if (p2 == -1.0) p2 = -0.34; if (p3 == -1.0) p3 = 60; RegulMeshTaubin(p1, p2, (int)p3); }
  else
  {
    fprintf(stderr, "%s: bad mode: %d\n", argv[0], mode);
    exit(0);
  }

#ifdef EVALUATION
  md = MeanDistCenter();
  MSE = 0.0;
  for (i = 0; i < Vertices->cur; i++)
  {
    err = md - dist3(xc,yc,zc,Vertices->v[i].x,Vertices->v[i].y,Vertices->v[i].z);
    MSE += err * err;
  }
  //printf("shrinking rate : %g ; MSE = %g\n", md / md0, MSE / Vertices->cur);
  printf("%g\t%g\n", md / md0, MSE / Vertices->cur);
#endif

  fileout = fopen(argv[argc-1],"w");
  if (fileout == NULL)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(0);
  }
  if (formatout == MCM) SaveMeshMCM(fileout);
  if (formatout == VTK) 
  {
    genheaderVTK(fileout, "mcube output");    
    SaveMeshVTK(fileout);
  }
  fclose(fileout);
} /* main */
