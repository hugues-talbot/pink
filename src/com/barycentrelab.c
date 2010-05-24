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
/*! \file barycentrelab.c

\brief compute the center of gravity of each labeled region

<B>Usage:</B> barycentrelab in.pgm out.pgm

<B>Description:</B>
Compute the center of gravity of each labeled region.
Labels should be consecutive integers. 
Only strictly positive labels are considered. 

<B>Types supported:</B> byte 2d

<B>Category:</B> geo
\ingroup  geo

\author Michel Couprie
*/
/* Calcule les isobarycentres des regions etiquetees d'une image. */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lbarycentre.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagebyte;
  int32_t *L;
  uint8_t *B;
  int32_t x;
  int32_t rs, cs, d, N;

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "barycentre: readimage failed\n");
    exit(1);
  }

  if (! lbarycentrelab(image))
  {
    fprintf(stderr, "%s: function lbarycentrelab failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;
  L = SLONGDATA(image);
  
  imagebyte = allocimage(image->name, rs, cs, d, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  B = UCHARDATA(imagebyte);

  for (x = 0; x < N; x++) B[x] = (uint8_t)mcmin(L[x],255);

  writeimage(imagebyte, argv[argc-1]);
  freeimage(image);
  freeimage(imagebyte);

  return 0;
} /* main */
