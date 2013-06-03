/* $Id: profil.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
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
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
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


