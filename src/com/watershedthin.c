/* $Id: */
/*! \file watershedthin.c

\brief Thin watershed transformation (Meyer's algorithm on  perfect fusion grid)

<B>Usage:</B> watershedthin in mark <bgmark|null> <roi|null> parite out

<B>Description:</B>

Performs the watershed transformation on the image <B>in</B>,
considered to be on a perfect fusion grid, taking the set of markers in
<B>mark</B>.  Returns the divide set.

If this parameter is present, <B>bgmark</B> is used as a
set of markers for the background.  If this parameter is present,
<B>roi</B> indicates the region of interest on which the operation is
performed.  The parameter <B>parite</B> (0 or 1) gives the 
the translation of the fusion grid.

<B>Types supported:</B> byte 2d

<B>Category:</B> connect
\ingroup connect

\author Jean Cousty
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
  int32_t parite;

  if (argc != 7)
  {
    fprintf(stderr, "usage: %s in mark <bgmark|null> <roi|null> parite out\n", argv[0]);
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

  parite = atoi(argv[5]);
  if ((parite != 0) && (parite != 1) && (parite != 14))
  {
    fprintf(stderr, "%s: bad parity: %d\n", argv[0], parite);
    exit(1);
  }
  
  //printf("lpebiconnexe \n");
  if(parite != 14)
  {
    if (! llpemeyerbiconnecte(image, marqueurs, marqueursfond, masque, parite))
    {
      fprintf(stderr, "%s: llpemeyerbiconnecte failed\n", argv[0]);
      exit(1);
    }
  }
  else
  {
    if (! llpemeyerbiconnecte3d(image, marqueurs, marqueursfond, masque))
    {
      fprintf(stderr, "%s: llpemeyerbiconnecte failed\n", argv[0]);
      exit(1);
    }
  }

  writeimage(image, argv[argc - 1]);
  freeimage(image);
  freeimage(marqueurs);
  if (marqueursfond) freeimage(marqueursfond);
  if (masque) freeimage(masque);

  return 0;
} /* main */
