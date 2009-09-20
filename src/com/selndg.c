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
/*! \file selndg.c

\brief selects pixels with values between two thresholds 

<B>Usage:</B> selndg in.pgm inf sup out.ppm

<B>Description:</B> 
Selects pixels x such that \b inf <= F[x] <= \b sup.
If the input image is of type byte, the output is a binary image.
If the input image is of type long, the output is also a long image
where all pixels whose values are not in the interval, are put to 0.

<B>Types supported:</B> byte 2d, byte 3d, long 2d, long 3d

<B>Category:</B> arith
\ingroup  arith

\author Michel Couprie
*/
/* selection de pixels ayant un niveau de gris compris entre 2 bornes */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lselndg.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t inf, sup;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s in.pgm inf sup out.ppm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "selndg: readimage failed\n");
    exit(1);
  }
  
  inf = atoi(argv[2]);
  sup = atoi(argv[3]);

  if (! lselndg(image, inf, sup))
  {
    fprintf(stderr, "selndg: function lselndg failed\n");
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
