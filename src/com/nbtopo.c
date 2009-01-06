/* $Id: nbtopo.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file nbtopo.c

\brief connectivity numbers

<B>Usage:</B> nbtopo filein.pgm connex <PP|P|M|MM> fileout.pgm

<B>Description:</B>
For each point p of the input grayscale image, compute the connectivity number T++,
T+, T- or T-- according to the given option (resp. PP, P, M, MM).
Refs: [BEC97, CBB01].

<B>Types supported:</B> byte 2D, byte 3D

<B>Category:</B> topogray
\ingroup  topogray

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lnbtopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t connex;
  int32_t function;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s filein.pgm connex <PP|P|M|MM> fileout.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  connex = atoi(argv[2]);

  if (strcmp(argv[3], "PP") == 0) function = PP; else
  if (strcmp(argv[3], "P") == 0) function = P; else
  if (strcmp(argv[3], "M") == 0) function = M; else
  if (strcmp(argv[3], "MM") == 0) function = MM; else
  {
    fprintf(stderr, "usage: %s filein.pgm connex <PP|P|M|MM> fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (! lnbtopo(image, connex, function))
  {
    fprintf(stderr, "%s: lnbtopo failed\n", argv[0]);
    exit(1);
  }

  writeimage(image, argv[4]);
  freeimage(image);

  return 0;
} /* main */
