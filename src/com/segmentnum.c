/* $Id: segmentnum.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file segmentnum.c

\brief tool for multiclass image segmentation

<B>Usage:</B> segmentnum in.pgm measure threshold out.pgm

<B>Description:</B>
Segmentation by selection of components in the component tree, computed on the 
lineal gradient of the original image.

The parameter "measure" specifies one of the following measures: <B>surf</B> (area), 
<B>prof</B> (depth), or <B>vol</B> (volume). The parameter "threshold" is an integer 
value. 

<B>Types supported:</B> byte 2D

<B>Category:</B> connect
\ingroup connect

\warning experimental - do not use in applications

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lsegmentnum.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * result;
  int32_t trace;
  char * nameout;
  int32_t mesure;
  int32_t seuil;

  if ((argc < 5) || (argc > 6) || ((argc == 6) && (strcmp(argv[4], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm <surf|prof|vol> seuil [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[2], "surf") == 0) mesure = SURFACE;
  else if (strcmp(argv[2], "prof") == 0) mesure = PROFONDEUR;
  else if (strcmp(argv[2], "vol") == 0) mesure = VOLUME;
  else 
  {
    fprintf(stderr, "usage: %s filein.pgm <surf|prof|vol> seuil [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  seuil = atoi(argv[3]);

  if (argc == 5)
  {
    trace = 0;
    nameout = argv[4];
  }
  else
  {
    trace = 1;
    nameout = argv[5];
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "segmentnum: readimage failed\n");
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "segmentnum: allocimage failed\n");
    exit(1);
  }

  if (! lsegmentnum(image, mesure, seuil, trace, result))
  {
    fprintf(stderr, "segmentnum: lsegmentnum failed\n");
    exit(1);
  }

  writelongimage(result, nameout);
  freeimage(image);
  freeimage(result);
  return 0;
} /* main */



