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
/*! \file diZenzo.c

\brief diZenzo gradient for color images

<B>Usage:</B> diZenzo imageRVB.ppm alpha [mode] out.pgm
<B>Python Usage:</B> pink.cpp.signal.diZenzo( red_image, green_image, blue_image, alpha )
<B>Python Usage:</B> pink.cpp.signal.diZenzodirection( red_image, green_image, blue_image, alpha )

<B>Description:</B>
The diZenzo gradient is defined by p, q, and t:

p = Rx*Rx + Gx*Gx + Bx*Bx

q = Ry*Ry + Gy*Gy + By*By

t = Rx*Ry + Gx*Gy + Bx*By

(where Rx = is the derivative of the red band of the image in the x
direction, Ry is the derivative in the y direction, etc.)

The magnitude is given by:

G = sqrt(1/2*(p+q+sqrt((p+q)*(p+q) -4(pq-t*t))))

And the direction is given by:

1/2*atan(2*t/(p-q))

If [mode] is 0 (default) then the operator returns the image of the
magnitude, if [mode] is 1 then the image returns the gradient
orientation (a value between 0 and 360).

<B>NOTE:</B> In Python the two operators are separated. For the
direction use the <B>diZenzodirection</B>.

The gradients are calculated by the Deriche filter using the [alpha]
as parameter.

<B>Types supported:</B> byte 2D

<B>Category:</B> signal
\ingroup  signal

\author Laurent Najman
*/

/* The diZenzo color gradient is recursively based on the Deriche filter. */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldiZenzo.h>

/* =============================================================== */
int main(int argc, char **argv) 
/* =============================================================== */
{
  struct xvimage * imageR;
  struct xvimage * imageV;
  struct xvimage * imageB;
  struct xvimage * result;
  double alpha;
  int32_t mode;

  if ((argc < 4) || (argc > 5))
  {
    fprintf(stderr, "usage: %s inRVB.ppm alpha [mode] out.pgm \n", 
            argv[0]);
    exit(1);
  }

  if (readrgbimage(argv[1], &imageR, &imageV, &imageB) == 0)
  {
    fprintf(stderr, "diZenzo: readrgbimage failed\n");
    exit(1);
  }

  alpha = atof(argv[2]);
  if (argc == 5) {
    mode = atoi(argv[3]);
  } else mode = 0;

  result = allocimage(NULL, rowsize(imageR), colsize(imageR), depth(imageR), VFF_TYP_4_BYTE);
  if (result == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }

  
  if (mode == 0) {
    
    if (! ldiZenzoGradient(imageR, imageV, imageB, alpha, result))
    {
      fprintf(stderr, "deriche: function lderiche failed\n");
      exit(1);
    }
    writeimage(result, argv[argc-1]);
  } else {
    ldiZenzoDirection(imageR, imageV, imageB, alpha, result );
    writeimage(result, argv[argc-1]);
  }
  
  freeimage(result);
  freeimage(imageR);
  freeimage(imageV);
  freeimage(imageB);

  return 0;
} /* main */


