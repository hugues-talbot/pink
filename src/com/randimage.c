/* $Id: randimage.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file randimage.c

\brief random image generation

<B>Usage:</B> randimage rowsize colsize depth valmax [seed] out.pgm

<B>Description:</B> Generates an image with pixel values uniformly randomly
distributed between 0 and \b valmax (included). 
If given, the parameter \b seed is used to 
initialize the random generator, otherwise the clock is used instead.

<B>Types supported:</B> byte 2D, byte 3D, int32_t 2D, int32_t 3D

<B>Category:</B> signal
\ingroup  signal

\author Michel Couprie
*/
/* genere une image aleatoire - bruit blanc */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <mccodimage.h>
#include <mcimage.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t i;
  int32_t rs, cs, ds, N, valmax;

  if ((argc != 6) && (argc != 7))
  {
    fprintf(stderr, "usage: %s rowsize colsize depth valmax [seed] out.pgm \n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  valmax = atoi(argv[4]);
  if (argc == 7) srand(atoi(argv[5])); else srand(time(NULL));

  if (valmax <= 255)
  {
    uint8_t *Im;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    Im = UCHARDATA(image);
    N = rs * cs * ds;
    for (i = 0; i < N; i++)
    {
      Im[i] = (int32_t) ((valmax + 1.0)*rand() / (RAND_MAX + 1.0));
    }
  }
  else
  {
    uint32_t *Im;
    image = allocimage(NULL, rs, cs, ds, VFF_TYP_4_BYTE);
    if (image == NULL)
    {
      fprintf(stderr, "%s: allocimage failed\n", argv[0]);
      exit(1);
    }

    Im = ULONGDATA(image);
    N = rs * cs * ds;
    for (i = 0; i < N; i++)
    {
      Im[i] = (int32_t) ((valmax + 1.0)*rand() / (RAND_MAX + 1.0));
    }
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
