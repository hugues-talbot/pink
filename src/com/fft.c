/* $Id: fft.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file fft.c

\brief fast Fourier transform

<B>Usage:</B> fft in1 in2 dir out1 out2

<B>Description:</B>
computes the 2 dimensional Fast Fourier Transform of an image.
The input image must be square and the number of rows (or columns)
MUST be a power of two.

This program will compute either a forward or inverse FFT, depending on
the direction requested with the \b dir option.  A \b dir value of 0
will result in a forward FFT, and a \b dir value of 1 will result
in an inverse FFT. 

The input arguments are described as follows:

\b in1
specifies the input image, which can be of data type BYTE, LONG or FLOAT.
If the input image is of data type BYTE, then the data will first be
converted to FLOAT before proceeding.
If the first input image is of type BYTE or FLOAT, then it is assumed that 
it is the real component of the complex pair, and the imaginary component 
may be input using the optional in2 input argument.  
If no imaginary component is specified when
using a real input, then the imaginary component is assumed to be zero.  

\b in2
specifies the optional input image, which can be of data type BYTE, LONG
or FLOAT. The keyword "null" may be used.
It is assumed that this image represents the imaginary component of the
complex pair.  

\b out1
specifies the real output image, which will be a single band image of data
type FLOAT.  This image contains only the real component of the complex pair.

\b out2
specifies the imaginary output image, which will be a single band image of data
type FLOAT.  This image contains only the imaginary component of the complex
pair.

\b dir
specifies the FFT direction.  A \b dir of 0 will result in a forward FFT, 
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

<B>Types supported:</B> byte 2d, int32_t 2d, float 2d

<B>Category:</B> signal
\ingroup signal

\author Scott Wilson, Rick Bogart, Lyle Bacon
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lfft.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t dir, rs, cs, rs2, cs2, x, y;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in1 in2 dir out1 out2 \n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  if (image1 == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  rs = rowsize(image1);
  cs = colsize(image1);

  if (strcmp(argv[2],"null") == 0) 
  {
    float *L;
    image2 = allocimage(NULL, rs, cs, 1, VFF_TYP_FLOAT);
    if (image2 == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(image2);
    for (x = 0; x < rs*cs; x++) L[x] = 0.0;
  }
  else
  {
    image2 = readimage(argv[2]);
    if (image2 == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if ((rowsize(image2) != rs) || (colsize(image2) != cs))
  {
    fprintf(stderr, "%s: Input images must be of the same size\n", argv[0]);
    exit(1);
  }
  rs2 = max(rs,cs);
  cs2 = 1;
  while (cs2 < rs2) cs2 = cs2 << 1;
  rs2 = cs2;

#ifdef VERBOSE
  printf("rs=%d cs=%d ; after padding: rs2=%d cs2=%d\n", rs, cs, rs2, cs2);
#endif

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *B = UCHARDATA(image1);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image1->xdim;
    imagefloat->ydim = image1->ydim;
    imagefloat->zdim = image1->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = (float)B[y*rs + x];    
    freeimage(image1);
    image1 = imagefloat;
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    int32_t *B = SLONGDATA(image1);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image1->xdim;
    imagefloat->ydim = image1->ydim;
    imagefloat->zdim = image1->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = (float)B[y*rs + x];    
    freeimage(image1);
    image1 = imagefloat;
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *B = FLOATDATA(image1);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image1->xdim;
    imagefloat->ydim = image1->ydim;
    imagefloat->zdim = image1->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = B[y*rs + x];    
    freeimage(image1);
    image1 = imagefloat;
  }
  else
  {
    fprintf(stderr, "%s: bad datatype for image1\n", argv[0]);
    exit(1);
  }

  if (datatype(image2) == VFF_TYP_1_BYTE)
  {
    uint8_t *B = UCHARDATA(image2);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image2->xdim;
    imagefloat->ydim = image2->ydim;
    imagefloat->zdim = image2->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = (float)B[y*rs + x];    
    freeimage(image2);
    image2 = imagefloat;
  }
  else if (datatype(image2) == VFF_TYP_4_BYTE)
  {
    int32_t *B = SLONGDATA(image2);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image2->xdim;
    imagefloat->ydim = image2->ydim;
    imagefloat->zdim = image2->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = (float)B[y*rs + x];    
    freeimage(image2);
    image2 = imagefloat;
  }
  else if (datatype(image2) == VFF_TYP_FLOAT)
  {
    float *B = FLOATDATA(image2);
    struct xvimage *imagefloat = allocimage(NULL, rs2, cs2, 1, VFF_TYP_FLOAT);
    float *L;
    if (imagefloat == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }
    L = FLOATDATA(imagefloat);
    imagefloat->xdim = image2->xdim;
    imagefloat->ydim = image2->ydim;
    imagefloat->zdim = image2->zdim;
    for (y = 0; y < cs; y++) 
      for (x = 0; x < rs; x++) 
        L[y*rs2 + x] = B[y*rs + x];    
    freeimage(image2);
    image2 = imagefloat;
  }
  else
  {
    fprintf(stderr, "%s: bad datatype for image2\n", argv[0]);
    exit(1);
  }

  dir = atoi(argv[3]);
  if (! lfft(image1, image2, dir))
  {
    fprintf(stderr, "%s: function lfft failed\n", argv[0]);
    exit(1);
  }

  writeimage(image1, argv[argc-2]);
  writeimage(image2, argv[argc-1]);
  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */


