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
/*! \file histo.c

\brief computes the histogram of an image or a region

<B>Usage:</B> histo in.pgm [mask.pgm] out.list

<B>Description:</B>
Calculates the histogram of \b im.pgm (masked by the binary image
\b mask.pgm, if given) and saves it in file \b out.list .

<B>Types supported:</B> byte 2d, byte 3d, int32_t 2d, int32_t 3d

<B>Category:</B> histo
\ingroup  histo

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lhisto.h>

#define INDEX_FIRST
/*
*/

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask = NULL;
  uint32_t * histo;
  int32_t i, k, s;
  FILE *fd = NULL;


  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s filein.pgm [mask.pgm]\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (argc == 3)
  {
    mask = readimage(argv[2]);
    if (mask == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  fd = fopen(argv[argc-1],"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", argv[0], argv[argc-1]);
    exit(1);
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    histo = (uint32_t *)calloc(1,(NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
    if (histo == NULL)
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }

    if (! lhisto(image, mask, histo))
    {
      fprintf(stderr, "%s: function lhisto failed\n", argv[0]);
      exit(1);
    }
    fprintf(fd, "s %d\n", NDG_MAX-NDG_MIN+1);
    for (i = NDG_MIN; i <= NDG_MAX; i++) fprintf(fd, "%4d %d\n", i, histo[i]);
    free(histo);
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    if (! lhistolong(image, mask, &histo, &s))
    {
      fprintf(stderr, "%s: function lhistolong failed\n", argv[0]);
      exit(1);
    }
    fprintf(fd, "s %d\n", s);
    for (i = 0; i < s; i++) fprintf(fd, "%4d %d\n", i, histo[i]);
    free(histo);
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float w, smin, smax;
    if (! lhistofloat(image, mask, &histo, &s, &w, &smin, &smax))
    {
      fprintf(stderr, "%s: function lhistofloat failed\n", argv[0]);
      exit(1);
    }
    fprintf(fd, "s %d\n", s);
    for (i = 0; i < s; i++) fprintf(fd, "%4d %d\n", i, histo[i]);
    free(histo);
  }

  freeimage(image);

  return 0;
} /* main */
