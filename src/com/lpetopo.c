/* $Id: lpetopo.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file lpetopo.c

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
#include <llpetopo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  int32_t trace;
  char * nameout;

  if ((argc < 4) || (argc > 5) || ((argc == 5) && (strcmp(argv[3], "trace") != 0)))
  {
    fprintf(stderr, "usage: %s filein.pgm marqueurs.pgm [trace] fileout.pgm\n", argv[0]);
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
    fprintf(stderr, "lpetopo: readimage failed\n");
    exit(1);
  }

  marqueurs = readimage(argv[2]);
  if (marqueurs == NULL)
  {
    fprintf(stderr, "lpetopo: readimage failed\n");
    exit(1);
  }

  if (! llpetopo(image, marqueurs, trace))
  {
    fprintf(stderr, "lpetopo: llpetopo failed\n");
    exit(1);
  }

  writeimage(image, nameout);
  freeimage(image);
  freeimage(marqueurs);

  return 0;
} /* main */
