/* $Id: mcmbuild.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* 
  Michel Couprie  -  Mars 2002
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

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  FILE *filein = NULL;
  FILE *fileout = NULL;
  meshbox MB0; 

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.mcm out.mcm\n", argv[0]);
    exit(0);
  }

  filein = fopen(argv[1],"r");
  LoadBuildMCM(filein);
  fclose(filein);
  fileout = fopen(argv[2],"w");
  SaveMeshMCM(fileout);
  fclose(fileout);
  TermineMesh();
} /* main */
