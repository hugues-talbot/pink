/* $Id: saliency.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file saliency.c

\brief salient watershed transformation

<B>Usage:</B> saliency in connex out

<B>Description:</B>
Performs the salient watershed transformation on the image <B>in</B>, 
The parameter <B>connex</B> gives the adjacency relation (4 or 6b in 2D).

The image out is a int32_t image (int32_t)

In 4 connectivity, the output image is a line graph in the khalimsky
grid. In 6b connectivity, the result is on the pixels themselves, with
the gammab neighborhood.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> connect
\ingroup connect

\author Laurent Najman
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <llpemeyer.h>
#include <lsaliency.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * masque;
  struct xvimage * result;
  int32_t connex;

  //if (argc != 5)
  if (argc != 4)
  {
    //fprintf(stderr, "usage: %s in <roi|null> connex out\n", argv[0]);
    fprintf(stderr, "usage: %s in con out\n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);

  if (!strcmp(argv[2], "4")) {
    result = allocimage(NULL, 2*rowsize(image)+1, 2*colsize(image)+1, depth(image), VFF_TYP_4_BYTE);
    if (result == NULL)
      {   
	fprintf(stderr, "%s: allocimage failed\n", argv[0]);
	exit(1);
      }
    /*
      if (strcmp(argv[2],"null") == 0) 
      masque = NULL;
      else
      {
      masque = readimage(argv[2]);
      if (masque == NULL)
      {
      fprintf(stderr, "%s: readimage failed\n", argv[0]);
      exit(1);
      }
      }
    */
    masque = NULL;
    //connex = atoi(argv[3]);
    connex = 4;

    if ((connex == 4) || (connex == 8))
      {
	if (! lsaliency(image, masque, result, connex))
	  {
	    fprintf(stderr, "%s: lsaliency failed\n", argv[0]);
	    exit(1);
	  }
      }
    /*
      else if ((connex == 6) || (connex == 18) || (connex == 26))
      {
      if (! llpemeyer3dsansligne(image, marqueurs, marqueursfond, masque, connex, result))
      {
      fprintf(stderr, "%s: llpemeyer3d failed\n", argv[0]);
      exit(1);
      }
      }*/
  } else if (!strcmp(argv[2], "6b")) {
    result = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
    if (result == NULL)
      {   
	fprintf(stderr, "%s: allocimage failed\n", argv[0]);
	exit(1);
      }
    masque = NULL;
    if (! lsaliency6b(image, masque, result, 0))
	  {
	    fprintf(stderr, "%s: lsaliency6b failed\n", argv[0]);
	    exit(1);
	  }
  }
  else    
  {
    fprintf(stderr, "%s: bad connexity: %d\n", argv[0], connex);
    exit(1);
  }


  writeimage(result, argv[argc - 1]);
  freeimage(image);
  freeimage(result);
  if (masque) freeimage(masque);

  return 0;
} /* main */
