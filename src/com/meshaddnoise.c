/* $Id: meshaddnoise.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file meshaddnoise.c

\brief adds gaussian noise to the positions of the vertices of a mesh

<B>Usage:</B> meshaddnoise in alpha out

<B>Description:</B>

Adds gaussian noise to the positions of the vertices of a mesh.
The mean of the gaussian is 0.
The standard deviation of the gaussian is given by <B>alpha</B>. 
Available input formats: mcm, ifs, vtk, ac.
Available output formats: mcm, vtk.

<B>Types supported:</B> mesh 3d

<B>Category:</B> mesh3d
\ingroup  mesh3d

\author Michel Couprie
*/
/* 
  Michel Couprie  -  Fevrier 2004
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

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  int32_t i;
  int32_t formatin, formatout;
  double alpha;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in alpha out\n", argv[0]);
    exit(0);
  }
  formatin = UNKNOWN;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".mcm") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".MCM") == 0) formatin = T_MCM;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".vtk") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".VTK") == 0) formatin = T_VTK;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".ifs") == 0) formatin = T_IFS;
  if (strcmp(argv[1]+strlen(argv[1])-4, ".IFS") == 0) formatin = T_IFS;
  if (formatin == UNKNOWN)
  {
    fprintf(stderr, "%s: bad input file format\n", argv[0]);
    exit(0);
  }
  formatout = UNKNOWN;
  if (strcmp(argv[argc-1]+strlen(argv[argc-1])-4, ".mcm") == 0) formatout = T_MCM;
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
  fclose(filein);

  alpha = atof(argv[2]);

  AddNoiseMesh(alpha);

  fileout = fopen(argv[argc-1],"w");
  if (formatout == T_MCM) SaveMeshMCM(fileout);
  if (formatout == T_VTK) 
  {
    genheaderVTK(fileout, (char *)"meshaddnoise output");    
    SaveMeshVTK(fileout);
  }
  fclose(fileout);
  return 0;
} /* main */
