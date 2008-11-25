/* $Id: watershedMeyer4D.c,v 1.1.1.1 2008-11-25 08:01:38 mcouprie Exp $ */
/*! \file watershedMeyer4D.c

\brief watershed transformation (Meyer's algorithm)

<B>Usage:</B> watershedMeyer4D prefix_in first last prefix_mark <prefix_bgmark|null> <prefix_roi|null> connex prefix_out

<B>Description:</B>
Performs the watershed transformation on the 4D image made of the concatenation of the 3D images <B>prefix_inxxxx.pgm</B> (where nnnn is a four digit decimal integer and where <B>first</B> and <B>last</B> specify respectively the first and the last volume).The watershed is computed taking the
set of markers in <B>prefix_markxxxx.pgm</B>. 
If this parameter is present, <B>prefix_bgmarkxxxx.pgm</B>
is used as a set of markers for the background.
If this parameter is present, <B>prefix_roixxxx.pgm</B>
indicates the region of interest on which the operation is performed.
The parameter <B>connex</B> gives the adjacency relation (8 in 4D) 
for the makers.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Jean Cousty - fevrier 2005
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <llpemeyer4d.h>

/* =============================================================== */
int32_t main(argc, argv) 
/* =============================================================== */
  int32_t argc; char **argv; 
{
  struct xvimage4D * image;
  struct xvimage4D * marqueurs;
  struct xvimage4D * marqueursfond;
  struct xvimage4D * masque;
  int32_t connex, nb_images;

  if (argc != 9)
  {
    fprintf(stderr, "usage: %s prefix_in first last prefix_mark <prefix_bgmark|null> <roi|null> connex out\n", argv[0]);
    exit(1);
  }
  image = readimage4D(argv[1], atoi(argv[2]), atoi(argv[3]));
  if(image == NULL)
  {
    fprintf(stderr, "%s: readimage4D failed \n", argv[0]);
    exit(1);
  }
  marqueurs = readimage4D(argv[4], atoi(argv[2]), atoi(argv[3]));
  if ((marqueurs == NULL))
  {
    fprintf(stderr, "%s: readimage4D failed\n", argv[0]);
    exit(1);
  }
  
  if (strcmp(argv[5],"null") == 0) 
    marqueursfond = NULL;
  else
  {
    marqueursfond = readimage4D(argv[5], atoi(argv[2]), atoi(argv[3]));
    if (marqueursfond == NULL)
    {
      fprintf(stderr, "%s: readimage4D failed\n", argv[0]);
      exit(1);
    }
  }

  if (strcmp(argv[6],"null") == 0) 
    masque = NULL;
  else
  {
    masque = readimage4D(argv[6], atoi(argv[2]), atoi(argv[3]));
    if (masque == NULL)
    {
      fprintf(stderr, "%s: readimage4D failed\n", argv[0]);
      exit(1);
    }
  }
  connex = atoi(argv[7]);
  
  if (connex == 8)
  {
    llpemeyer4d(image, marqueurs, marqueursfond, masque, connex);
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }  
  writeimage4D(image, argv[argc - 1], atoi(argv[2]), atoi(argv[3]) );
  
  freeimage4D(image);
  freeimage4D(marqueurs);
  if (marqueursfond) freeimage4D(marqueursfond);
  if (masque) freeimage4D(masque);
  return 0;
} /* main */
