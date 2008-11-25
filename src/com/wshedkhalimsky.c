/* $Id: wshedkhalimsky.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file wshedkhalimsky.c

\brief watershed transformation in Khalimsky space (inter pixel watershed)

<B>Usage:</B> watershed in mark <bgmark|null> <roi|null> out

<B>Description:</B>
Performs the watershed transformation on the image <B>in.pgm</B>, taking the
set of markers in <B>mark.pgm</B>. 
If this parameter is present, <B>bgmark.pgm</B>
is used as a set of markers for the background.
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
All images must be previously transformed in the khalimsky space with a max strategy.
The output image is in khalimsky space too.

<B>Types supported:</B> byte 2d

<B>Category:</B> connect orders
\ingroup connect orders

\author Michel Couprie & Christophe Doublier
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>


/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * marqueursfond;
  struct xvimage * masque;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in mark <bgmark|null> <roi|null> out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);
  marqueurs = readimage(argv[2]);
  if ((image == NULL) || (marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  if (strcmp(argv[3],"null") == 0) 
    marqueursfond = NULL;
  else
  {
    marqueursfond = readimage(argv[3]);
    if (marqueursfond == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  if (strcmp(argv[4],"null") == 0) 
    masque = NULL;
  else
  {
    masque = readimage(argv[4]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

    if (! llpemeyerkhalimsky(image, marqueurs, marqueursfond, masque))
    {
      fprintf(stderr, "%s: llpemeyerkhalimsky failed\n", argv[0]);
      exit(1);
    }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (marqueursfond) freeimage(marqueursfond);
  if (masque) freeimage(masque);

  return 0;
} /* main */
