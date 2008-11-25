/* $Id: attribute.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file attribute.c

\brief labels each connected component with its attribute value 

<B>Usage:</B> attribute in.pgm connex seuil <min|max|pla> <area|perim|circ|trous|excen|orien|vdiam|hdiam> out.pgm

<B>Description:</B>
Labels each connected component with its attribute value.
Argument \b connex defines the connectivity.
Argument \b seuil defines a threshold to filter out lowest attribute values.

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\warning This operator is in experimental state. Do not use in applications. 

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lattribute.h>

//#define VERBOSE
#define USAGE "usage: %s filein.pgm connex seuil <min|max|pla> <area|perim|circ|trous|excen|orien|vdiam|hdiam> fileout.pgm\n"

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  int32_t nblabels, connex, attrib, typregion, seuil, i;
  struct xvimage * image;
  struct xvimage * result;
  uint8_t * I;
  char *name1, *name;
  char buf[32];

  if (argc != 7)
  {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "attribute: readimage failed\n");
    exit(1);
  }
  I = UCHARDATA(image);

  connex = atoi(argv[2]);
  seuil = atoi(argv[3]);

  if (strcmp(argv[4], "min") == 0) typregion = LABMIN;
  else if (strcmp(argv[4], "max") == 0) typregion = LABMAX;
  else if (strcmp(argv[4], "pla") == 0) typregion = LABPLATEAU;
  else 
  {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  if (strcmp(argv[5], "area") == 0) attrib = AREA;
  else if (strcmp(argv[5], "perim") == 0) attrib = PERIM;
  else if (strcmp(argv[5], "trous") == 0) attrib = TROUS;
  else if (strcmp(argv[5], "circ") == 0) attrib = CIRC;
  else if (strcmp(argv[5], "excen") == 0) attrib = EXCEN;
  else if (strcmp(argv[5], "orien") == 0) attrib = ORIEN;
  else if (strcmp(argv[5], "vdiam") == 0) attrib = VDIAM;
  else if (strcmp(argv[5], "hdiam") == 0) attrib = HDIAM;
  else 
  {
    fprintf(stderr, USAGE, argv[0]);
    exit(1);
  }

  result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  if (! lattribute(image, connex, typregion, attrib, seuil, result, &nblabels))
  {
    fprintf(stderr, "%s: lattribute failed\n", argv[0]);
    exit(1);
  }

#ifdef VERBOSE
  printf("%s : NOMBRE DE COMPOSANTES : %d\n", argv[0], nblabels-1);
#endif

  writelongimage(result, argv[6]);

  freeimage(result);
  freeimage(image);

  return 0;
} /* main */
