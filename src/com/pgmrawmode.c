/* $Id: pgmrawmode.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file pgmrawmode.c

\brief converts a pgm file to raw mode

<B>Usage:</B> pgmrawmode filename [outfilename]

<B>Description:</B> Converts a pgm file to raw mode.
If only parameter \b filename is given, then the result
is also named \b filename .

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> convert
\ingroup  convert

\author Michel Couprie
*/
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
  struct xvimage * image;

  if ((argc != 2) && (argc != 3))
  {
    fprintf(stderr, "usage: %s filename [outfilename] \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {   fprintf(stderr,"%s: readimage failed\n", argv[0]);
      exit(1);
  }

  writerawimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
