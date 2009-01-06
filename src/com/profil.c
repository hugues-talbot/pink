/* $Id: profil.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file profil.c

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
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lprofil.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s filein.pgm\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (! lprofil(image))
  {
    fprintf(stderr, "%s: lprofil failed\n", argv[0]);
    exit(1);
  }
  freeimage(image);

  return 0;
} /* main */


