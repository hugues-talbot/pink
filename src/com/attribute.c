/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
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
int main(int argc, char **argv)
/* =============================================================== */
{
  int32_t nblabels, connex, attrib, typregion, seuil;
  struct xvimage * image;
  struct xvimage * result;
  uint8_t * I;

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
