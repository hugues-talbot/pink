/* $Id: watershedMeyer2.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/*! \file watershedMeyer2.c

\brief watershed transformation (Meyer's algorithm) with labelled markers  

<B>Usage:</B> watershedMeyer2 in mark <roi|null> connex out

<B>Description:</B>
Performs the watershed transformation on the image <B>in</B>, taking the
set of markers in <B>mark</B>, in the form of a label image where all the pixels
sharing the same label (even if not connected) belong to the same marker.
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
  struct xvimage * masque;
  int32_t connex;

  if (argc != 6)
  {
    fprintf(stderr, "usage: %s in mark <roi|null> connex out\n", argv[0]);
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
    masque = NULL;
  else
  {
    masque = readimage(argv[3]);
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
    }
  }

  connex = atoi(argv[4]);

  if ((connex == 4) || (connex == 8))
  {
    if (! llpemeyer2(image, marqueurs, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyer2 failed\n", argv[0]);
      exit(1);
    }
  }
  else if ((connex == 6) || (connex == 18) || (connex == 26))
  {
    if (! llpemeyer3d2(image, marqueurs, masque, connex))
    {
      fprintf(stderr, "%s: llpemeyer3d2 failed\n", argv[0]);
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
  if (masque) freeimage(masque);

  return 0;
} /* main */
