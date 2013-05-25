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
/* \file convol3.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* convolution par un masque 3x3 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lconvol3.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * mask;

  if ((argc < 4) || (argc > 5) || ((argc == 5) && (strcmp(argv[3], "norm") != 0)))
  {
    fprintf(stderr, "usage: %s in.pgm mask.pgm [norm] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  mask = readimage(argv[2]);
  if ((image == NULL) || (mask == NULL))
  {
    fprintf(stderr, "convol3: readimage failed\n");
    exit(1);
  }
  
  if (! lconvol3(image, mask, (argc == 5)))
  {
    fprintf(stderr, "convol3: function lconvol3 failed\n");
    exit(1);
  }

  if (argc == 5) writeimage(image, argv[4]); else writeimage(image, argv[3]);
  freeimage(image);
  freeimage(mask);
  return 0;
} /* main */

