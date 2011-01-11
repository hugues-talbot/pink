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
/*! \file any2complex.c

\brief converts a "byte", "long" of "float" image to a "complex" image

<B>Usage:</B> any2complex in out

<B>Description:</B> 

<B>Types supported:</B> byte 2d, byte 3d, long 2d, long 3d, float 2d, float 3d.

<B>Category:</B> convert
\ingroup convert

\author Michel Couprie
*/

/* Michel Couprie - décembre 2010 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * imagecomplex;
  struct xvimage * image;
  int32_t x, rs, cs, ds, N;
  float *C;
  
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s in1.pgm out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]); 
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image);
  cs = colsize(image);
  ds = depth(image);
  N = rs * cs * ds;
  imagecomplex = allocmultimage(NULL, rs, cs, ds, 1, 2, VFF_TYP_FLOAT);
  if (imagecomplex == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  C = FLOATDATA(imagecomplex);
  razimage(imagecomplex);

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *I = UCHARDATA(image);
    for (x = 0; x < N; x++) 
      C[x] = 
	(float)(I[x]);
  }
  else
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t *I = SLONGDATA(image);
    for (x = 0; x < N; x++) C[x] = (float)I[x];
  }
  else
  if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *I = FLOATDATA(image);
    for (x = 0; x < N; x++) C[x] = (float)I[x];
  }
  else
  if (datatype(image) == VFF_TYP_DOUBLE)
  {
    double *I = DOUBLEDATA(image);
    for (x = 0; x < N; x++) C[x] = (float)I[x];
  }
  else
  {
    fprintf(stderr, "%s: bad data type: %d\n", argv[0], datatype(image));
    exit(1);
  }
  
  imagecomplex->xdim = image->xdim;
  imagecomplex->ydim = image->ydim;
  imagecomplex->zdim = image->zdim;

  writeimage(imagecomplex, argv[argc-1]);
  freeimage(imagecomplex);
  freeimage(image);

  return 0;
} /* main */
