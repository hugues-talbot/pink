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
/*! \file ppm2pgm.c

\brief converts a color ppm image into 3 grayscale pgm images

<B>Usage:</B> ppm2pgm in.ppm r.pgm g.pgm b.pgm

<B>Description:</B>
Converts a color ppm image \b in.ppm into 3 grayscale pgm images
<B>r.pgm</B>, <B>g.pgm</B> and <B>b.pgm</B> which are the red, 
green and blue color planes of the original color image.

<B>Types supported:</B> byte color 2d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
/* explose une image en couleurs pour donner trois images en ndg */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * g1;
  struct xvimage * g2;
  struct xvimage * g3;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.ppm g1.pgm g2.pgm g3.pgm\n", argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &g1, &g2, &g3) == 0)
  {
    fprintf(stderr, "rgb2bw: readrgbimage failed\n");
    exit(1);
  }
  
  writeimage(g1, argv[2]);
  writeimage(g2, argv[3]);
  writeimage(g3, argv[4]);
  freeimage(g1);
  freeimage(g2);
  freeimage(g3);

  return 0;
} /* main */
