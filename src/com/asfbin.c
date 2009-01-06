/* $Id: asfbin.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/*! \file asfbin.c

\brief alternate sequential filter for binary images

<B>Usage:</B> asfbin in.pgm [rmin] rmax out.pgm

<B>Description:</B>
Alternate sequential filter for binary images. 

Disc-shaped structuring elements of increasing radius,
ranging from 1 (or <B>rmin</B> if this parameter is specified) to <B>rmax</B>,
are generated. 

Let D<SUB>1</SUB>...D<SUB>n</SUB> be these structuring elements, sorted by increasing radius.
Let F<SUB>0</SUB> = <B>in.pgm</B>, the ith intermediate result F<SUB>i</SUB> is obtained by the closing of
the opening of F<SUB>i-1</SUB> by the structuring element D<SUB>i</SUB>. 
The result <B>out.pgm</B> contains the final result F<SUB>n</SUB>.
Giving a value 0 for the optional parameter <B>rmin</B> has the effect of beginning 
by a closing instead of an opening.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie

*/

/*
<B>Example:</B>

asfbin cells 3 cells_asf

<table>
  <tr><td align="center"><IMG SRC="../../../Pinktests/Images/cells.gif"></td>
      <td align="center"><IMG SRC="../../../Pinktests/Images/cells_asfbin.gif"></td>
  </tr>
  <tr><td align="center">cells</td>
      <td align="center">cells_asfbin</td>
  </tr>
</table>
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcgeo.h>
#include <ldist.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayonmin, rayonmax, rayon;
  int32_t FERMETURE_EN_1 = 0;

  if ((argc != 4) && (argc != 5))
  {
    fprintf(stderr, "usage: %s in.pgm [rayonmin] rayonmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }
  if (argc == 4) 
  {
    rayonmin = 1;
    rayonmax = atoi(argv[2]);
  }
  else
  {
    rayonmin = atoi(argv[2]);
    rayonmax = atoi(argv[3]);
    if (rayonmin == 0) 
    {
      rayonmin = 1;
      FERMETURE_EN_1 = 1;
    }
  }

  for (rayon = rayonmin; rayon <= rayonmax; rayon++)
  {
#ifdef VERBOSE
    fprintf(stderr, "%s: rayon = %d\n", argv[0], rayon);
#endif

    if (FERMETURE_EN_1)
    {
      /* fermeture par l'element structurant */
      if (! ldilatball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
        exit(1);
      }
      if (! lerosball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
        exit(1);
      }
      /* ouverture par l'element structurant */
      if (! lerosball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
        exit(1);
      }
      if (! ldilatball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      /* ouverture par l'element structurant */
      if (! lerosball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
        exit(1);
      }
      if (! ldilatball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
        exit(1);
      }
      /* fermeture par l'element structurant */
      if (! ldilatball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function ldilatball failed\n", argv[0]);
        exit(1);
      }
      if (! lerosball(image, rayon, 0))
      {
        fprintf(stderr, "%s: function lerosball failed\n", argv[0]);
        exit(1);
      }
    }

  } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
