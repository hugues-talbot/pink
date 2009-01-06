/* $Id: segment3d.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file segment3d.c

obsolete - use segmentheight, segmentarea, segmentvol

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* 
NAME

<B>segment3d</B> - tool for 3D image segmentation

SYNOPSIS

<B>segment3d</B> in.pgm connex <surf|prof|vol> seuil <max|min> out.pgm

DESCRIPTION

Segmentation by selection of components in the component tree.
The parameter <B>connex</B> specifies the connexity (6, 18, 26).
The following parameter specifies one of the following measures: <B>surf</B> (area), 
<B>prof</B> (depth), or <B>vol</B> (volume). The parameter "seuil" is an integer value. 
The parameter <B>min</B> (resp. <B>max</B>) indicates that the resulting segmentation 
must be maximal (resp. maximal, in the sense of the inclusion).

Types supported: byte 3D.

CLASS 

morph ctree

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsegment3d.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t trace;
  char * nameout;
  int32_t connex;
  int32_t mesure;
  int32_t maximise;
  int32_t seuil;

  if ((argc < 7) || (argc > 8) || ((argc == 8) && (strcmp(argv[6], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm connex <surf|prof|vol> seuil <max|min> [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[3], "surf") == 0) mesure = SURFACE;
  else if (strcmp(argv[3], "prof") == 0) mesure = PROFONDEUR;
  else if (strcmp(argv[3], "vol") == 0) mesure = VOLUME;
  else 
  {
    fprintf(stderr, "usage: %s filein.pgm connex <surf|prof|vol> seuil <max|min> [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[5], "max") == 0) maximise = 1;
  else if (strcmp(argv[5], "min") == 0) maximise = 0;
  else 
  {
    fprintf(stderr, "usage: %s filein.pgm connex <surf|prof|vol> seuil <max|min> [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  seuil = atoi(argv[4]);

  if (argc == 7)
  {
    trace = 0;
    nameout = argv[6];
  }
  else
  {
    trace = 1;
    nameout = argv[7];
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segment3d: readimage failed\n");
    exit(1);
  }

  if (! lsegment3d(image, connex, mesure, seuil, maximise, trace))
  {
    fprintf(stderr, "segment3d: lsegment3d failed\n");
    exit(1);
  }

  writeimage(image, nameout);
  freeimage(image);

  return 0;
} /* main */



