/* $Id: lpetoporeg.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file lpetoporeg.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpetoporeg.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t trace;
  int32_t seuil;
  char * nameout;

  if ((argc < 4) || (argc > 5) || ((argc == 5) && (strcmp(argv[3], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm seuil [trace] fileout.pgm\n", argv[0]);
    exit(1);
  }

  if (argc == 4)
  {
    trace = 0;
    nameout = argv[3];
  }
  else
  {
    trace = 1;
    nameout = argv[4];
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "lpetoporeg: readimage failed\n");
    exit(1);
  }

  seuil = atoi(argv[2]);

  if (! llpetoporeg(image, seuil, trace))
  {
    fprintf(stderr, "lpetoporeg: llpetoporeg failed\n");
    exit(1);
  }

  writeimage(image, nameout);
  freeimage(image);

  return 0;
} /* main */
