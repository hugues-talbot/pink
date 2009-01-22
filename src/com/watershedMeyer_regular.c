/* $Id: watershedMeyer_regular.c,v 1.2 2009-01-22 07:05:36 mcouprie Exp $ */
/*! \file watershedMeyer.c

\brief watershed transformation (Meyer's algorithm)

<B>Usage:</B> watershedMeyer in mark <bgmark|null> <roi|null> connex out

<B>Description:</B>
Performs the watershed transformation on the image <B>in</B>, taking the
set of markers in <B>mark</B>. 
If this parameter is present, <B>bgmark</B>
is used as a set of markers for the background.
If this parameter is present, <B>roi</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (4,8 in 2D; 6,18,26 in 3D) 
for the makers.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage * image;
  struct xvimage * marqueurs;
  struct xvimage * marqueursfond;
  struct xvimage * masque;
  int32_t connex;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in mark <bgmark|null> <roi|null> connex out\n", argv[0]);
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

  connex = atoi(argv[5]);

  if ((connex == 4) || (connex == 8))
  {
    if (! llpemeyer(image, marqueurs, marqueursfond, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyer failed\n", argv[0]);
      exit(1);
    }
  }
  else if ((connex == 6) || (connex == 18) || (connex == 26))
  {
    if (! llpemeyer3d(image, marqueurs, marqueursfond, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyer3d failed\n", argv[0]);
      exit(1);
    }
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }


  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (marqueursfond) freeimage(marqueursfond);
  if (masque) freeimage(masque);

  return 0;
} /* main */
