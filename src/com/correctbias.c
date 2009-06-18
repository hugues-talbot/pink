/* $Id: correctbias.c,v 1.1 2009-06-18 06:34:55 mcouprie Exp $ */
/*! \file correctbias.c

\brief correction of a luminosity bias expressed by a linear function

<B>Usage:</B> correctbias in.pgm a type [xc yc] out.pgm

<B>Description:</B>
The bias can be vertical (\b type = v), horizontal (\b type = h), or 
radial (\b type = r, center \b xc, \b yc). 
Parameter \b a is the "slope" of the bias.
Principle of the correction: for a 1D signal, add -ax to the value of 
the element of abcissa x.

<B>Types supported:</B> byte 2D

<B>Category:</B> arith
\ingroup arith

\author Michel Couprie
*/

/* 
  Michel Couprie - juin 2009
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  char type;
  double xc, yc, a, T, R;
  int32_t rs, cs, N, x, y;
  uint8_t *I;

  if ((argc != 5) && (argc != 7))
  {
    fprintf(stderr, "usage: %s in.pgm a type [xc yc] out.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  I = UCHARDATA(image);
  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;

  a = atof(argv[2]);
  type = argv[3][0];
  if (argc == 7)
  {
    xc = atof(argv[4]);
    yc = atof(argv[5]);
  }

  if (type == 'r')
  {
    for (x = 0; x < rs; x++)
    for (y = 0; y < cs; y++)
    {
      R = sqrt((xc-x)*(xc-x) + (yc-y)*(yc-y));
      T = (double)(I[y*rs + x]) - (a * R);
      if (T > 255) T = 255; 
      if (T < 0) T = 0; 
      I[y*rs + x] = arrondi(T);
    }
  } 
  else
  if (type == 'h')
  {
    fprintf(stderr, "%s: type '%c' not yet implemented\n", argv[0], type);
    exit(1);
  } 
  else
  if (type == 'v')
  {
    fprintf(stderr, "%s: type '%c' not yet implemented\n", argv[0], type);
    exit(1);
  } 
  else
  {
    fprintf(stderr, "%s: bad type: '%c'\n", argv[0], type);
    exit(1);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);


  return 0;
}

