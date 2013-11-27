/* $Id: segment.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/* \file segment.c

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

<B>segment</B> - tool for image segmentation

SYNOPSIS

<B>segment</B> in.pgm connex out.pgm
<B>segment</B> in.pgm connex [mes1 seuil1 [mes2 seuil2 ... ]] [max] out.pgm

DESCRIPTION

Segmentation by selection of components in the component tree.

The first syntax, with only the three parameters <B>in.pgm</B>, <B>connex</B> and
<B>out.pgm</B>, calls an interactive version of the tool.
The parameter <B>connex</B> specifies the connexity (4, 8).

The second syntax allows to specify one or several couples "mes seuil".
The parameter "mes" specifies one of the following measures: <B>surf</B> (area), 
<B>prof</B> (depth), or <B>vol</B> (volume). The parameter "seuil" is an integer 
value. The optional parameter <B>max</B> indicates that the resulting segmentation 
must be maximal (in the sense of the inclusion) while still equivalent to the 
"standard" one.

Types supported: byte 2D.

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
#include <lsegment.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t connex;
  int32_t mesure;
  int32_t seuil;
  int32_t maximise = 0;
  int32_t a;
  int32_t seuilsurf;
  int32_t seuilprof;
  int32_t seuilvol;
#ifdef VERSION_EXPERIMENTALE
  int32_t seuilper;
  int32_t seuilcirc;
  int32_t seuilrdis;
  int32_t seuilnbtr;
  int32_t seuildtr;
  int32_t seuilexc;
#endif

  if (argc < 4) goto Erreur;

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);
  mesure = 0;
  for (a = 3; a < argc - 2; a += 2)
  { 
    if      (strcmp(argv[a], "surf") == 0) { mesure |= SURFACE;      seuilsurf = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "prof") == 0) { mesure |= PROFONDEUR;   seuilprof = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "vol")  == 0) { mesure |= VOLUME;       seuilvol  = atoi(argv[a+1]); }
#ifdef VERSION_EXPERIMENTALE
    else if (strcmp(argv[a], "per")  == 0) { mesure |= PERIMETRE;    seuilper  = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "circ") == 0) { mesure |= CIRCULARITE;  seuilcirc = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "rdis") == 0) { mesure |= ROOTDIST;     seuilrdis = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "nbtr") == 0) { mesure |= NBTROUS;      seuilnbtr = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "dtr")  == 0) { mesure |= DENSTROUS;    seuildtr  = atoi(argv[a+1]); }
    else if (strcmp(argv[a], "exc")  == 0) { mesure |= EXCENTRICITE; seuilexc  = atoi(argv[a+1]); }
#endif
    else { fprintf(stderr, "1 : a:%d, argv[a] = %s\n", a, argv[a]); goto Erreur; }
  }

  if ((argc % 2) == 1)
  {
    if (strcmp(argv[argc - 2], "max") == 0) maximise = 1;
    else { fprintf(stderr, "2\n"); goto Erreur; }
  }
  
#ifdef VERSION_EXPERIMENTALE
  if (! lsegment(image, connex, mesure, seuilsurf, seuilprof, seuilvol, seuilper, seuilcirc,
                 seuilrdis, seuilnbtr, seuildtr, seuilexc, maximise))
#else
  if (! lsegment(image, connex, mesure, seuilsurf, seuilprof, seuilvol, maximise))
#endif
  {
    fprintf(stderr, "%s: lsegment failed\n", argv[0]);
    exit(1);
  }
  
  writeimage(image, argv[argc - 1]);
  freeimage(image);
  return;

Erreur:
#ifdef VERSION_EXPERIMENTALE
  fprintf(stderr, 
  "usage: %s filein.pgm connex [mes1 seuil1 [mes2 seuil2 ... ]] [max] fileout.pgm\n  mes = <surf | prof | vol | per | circ | rdis | nbtr | dtr | exc>\n", argv[0]);
#else
  fprintf(stderr, 
  "usage: %s filein.pgm connex [mes1 seuil1 [mes2 seuil2 ... ]] [max] fileout.pgm\n  mes = <surf | prof | vol>\n", argv[0]);
#endif
  exit(1);

  return 0;
} /* main */



