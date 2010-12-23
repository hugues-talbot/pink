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
/*! \file fft.c

\brief fast Fourier transform

<B>Usage:</B> fft in.pgm [dir] out.pgm

<B>Description:</B>
computes the 2 dimensional Fast Fourier Transform of an image.
The input image must be square and the number of rows (or columns)
MUST be a power of two.

This program will compute either a forward or inverse FFT, depending on
the direction requested with the \b dir option.  A \b dir value of 0
will result in a forward FFT, and a \b dir value of 1 will result
in an inverse FFT. 

The input arguments are described as follows:

\b in.pgm
specifies the input image, which must be of data type FLOAT, with two 
bands. The first band contains the real component, the second band contains the imaginary component.

\b out.pgm
output image, which will be a 2-band image of data type FLOAT. The first band contains the real component, the second band contains the imaginary component.

\b dir (optional)
specifies the FFT direction.  A \b dir of 0 (default) will result in a forward FFT, 
and a \b dir of 1 will result in an inverse FFT.

Note that it is the users responsibility to ensure that the correct components
of the image are used when requesting an FFT or Inverse FFT.  Unexpected 
results may occur if the user requests an inverse FFT and only inputs the real 
component of an image.  If an FFT of an image is taken, then both the real and 
imaginary components must be used to obtain a correct inverse FFT.

For a forward FFT, the input data is multiplied by (-1)**(x+y) where (x,y) is
the pixel coordinate. This has the effect of shifting the frequency domain
result so that the DC component is at (N/2,N/2) rather than (0,0). For the
inverse FFT case, the data is multiplied by (-1)**(x+y) AFTER the
FFT processing, accounting for the fact that the input frequency domain data
was center-shifted by the forward FFT. The center-shifted frequency domain
representation is much easier to visualize and filter than it would be
if not shifted. For more information on the shifting teqchnique, see
R.C. Gonsalez and P. Wintz, "Digital Image Processing, 2nd ed, sec 3.2.2,
p. 77. (1987). The center-shifting should really be an option.

For the forward FFT, there is no scaling on the data. For the inverse FFT,
the data is scaled by 1/(N*N). Thus, to generate a sinewave of amplitude
1.0 for a 64x64 complex image to be handed to the inverse FFT, there should
be two impulses at conjugate locations (symmetric about the center of the image)
each with amplitude 0.5/(64*64). Why 0.5? It's because each impulse carries
half of the power! The scaling should really be an option, but it currently
is not (perhaps in a future patch).

<B>Types supported:</B> float (complex) 2d

<B>Category:</B> signal
\ingroup signal

\author Scott Wilson, Rick Bogart, Lyle Bacon
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lfft.h>

#define VERBOSE

struct xvimage *pad_image(struct xvimage *image)
{
    struct xvimage *tmpimage;
    float *I, *T;
    int32_t i, j, cs, rs, N, cs2, rs2, N2;

    cs = colsize(image);            /* Number of rows */
    rs = rowsize(image);            /* Number of columns */
    N = rs * cs;
    I = FLOATDATA(image);

    rs2 = cs2 = 1;
    while (rs2 < rs) rs2 = rs2 << 1;
    while (cs2 < cs) cs2 = cs2 << 1;

    if ((rs2 != rs) || (cs2 != cs))
    {
      N2 = rs2 * cs2;
      tmpimage = allocmultimage(NULL, rs2, cs2, 1, 1, 2, VFF_TYP_FLOAT);
      assert(tmpimage != NULL);
      tmpimage->xdim = image->xdim;
      tmpimage->ydim = image->ydim;
      tmpimage->zdim = image->zdim;
      T = FLOATDATA(tmpimage);
      for (j = 0; j < cs; j++) 
	for (i = 0; i < rs; i++) 
	  T[j*rs2 + i] = I[j*rs + i];    
      for (j = 0; j < cs; j++) 
	for (i = 0; i < rs; i++) 
	  T[N2+ j*rs2 + i] = I[N + j*rs + i];    
#ifdef VERBOSE
      printf("rs=%d cs=%d ; after padding: rs2=%d cs2=%d\n", rs, cs, rs2, cs2);
#endif
      return tmpimage;
    }
    else
      return NULL;
}

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * image2;
  int32_t dir = 0;

  if ((argc != 3) && (argc != 4))
  {
    fprintf(stderr, "usage: %s in.pgm [dir] out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if ((datatype(image) != VFF_TYP_FLOAT) || (depth(image) != 1) || (nbands(image) != 2))
  {
    fprintf(stderr,"%s: input image type must be complex 2D\n", argv[0]);
    exit(1);
  }

  if (argc == 4) dir = atoi(argv[2]);

  image2 = pad_image(image);
  if (image2 != NULL)
  {
    freeimage(image);
    image = image2;
  }

  if (! lfft(image, dir))
  {
    fprintf(stderr, "%s: function lfft failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */


