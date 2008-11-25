/* $Id: asflin.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/*! \file asflin.c

\brief alternate sequential filter by linear structuring elements

<B>Usage:</B> asflin in.pgm <x|y|z> [rmin] rmax out.pgm

<B>Description:</B>
Alternate sequential filter by linear structuring elements for grayscale images.
Linear structuring elements in direction <B>x, y</B> or <B>z</B> of increasing radius,
ranging from 1 (or <B>rmin</B> if this parameter is specified) to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I0 = <B>in.pgm</B>, the ith intermediate result Ii is obtained by the closing of
the opening of Ii-1 by the structuring element Di. 
The result <B>out.pgm</B> contains the final result In.
Giving a value 0 for the optional parameter <B>rmin</B> has the effect of beginning 
by a closing instead of an opening.

<B>Types supported:</B> byte 2d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

#define VERBOSE

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image;
  int32_t rayonmin, rayonmax, rayon;
  int32_t d, xc, yc, zc, n, x, y, z, nptb;
  int32_t *tab_es_x, *tab_es_y, *tab_es_z;
  int32_t FERMETURE_EN_1 = 0;
  char dir;

  if ((argc != 5) && (argc != 6))
  {
    fprintf(stderr, "usage: %s in.pgm <x|y|z> [rayonmin] rayonmax out.pgm \n", argv[0]);
    exit(1);
  }

  image = readimage(argv[1]);  
  if (image == NULL)
  {
    fprintf(stderr, "%s: readimage failed\n", argv[0]);
    exit(1);
  }

  dir = tolower(argv[2][0]);

  if (argc == 5) 
  {
    rayonmin = 1;
    rayonmax = atoi(argv[3]);
  }
  else
  {
    rayonmin = atoi(argv[3]);
    rayonmax = atoi(argv[4]);
    if (rayonmin == 0) 
    {
      rayonmin = 1;
      FERMETURE_EN_1 = 1;
    }
  }

  d = 2 * rayonmax + 1;
  tab_es_x = (int32_t *)calloc(1,d * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,d * sizeof(int32_t));
  tab_es_z = (int32_t *)calloc(1,d * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
  {
    fprintf(stderr, "%s: malloc failed\n", argv[0]);
    exit(1);
  }

  for (rayon = rayonmin; rayon <= rayonmax; rayon++)
  {
#ifdef VERBOSE
    fprintf(stderr, "asflin: rayon = %d\n", rayon);      
#endif
    /* genere l'element structurant lineaire sous forme de liste des coord. des points */
    zc = yc = xc = rayon; 
    nptb = 0;
    if (dir == 'x')
    { 
      for (x = -rayon; x <= rayon; x++)
      {
        tab_es_x[nptb] = xc + x;
        tab_es_y[nptb] = yc;
        tab_es_z[nptb] = zc;
        nptb++;
      }
    }
    else if (dir == 'y')
    { 
      for (y = -rayon; y <= rayon; y++)
      {
        tab_es_x[nptb] = xc;
        tab_es_y[nptb] = yc + y;
        tab_es_z[nptb] = zc;
        nptb++;
      }
    }
    else if (dir == 'z')
    { 
      for (z = -rayon; z <= rayon; z++)
      {
        tab_es_x[nptb] = xc;
        tab_es_y[nptb] = yc;
        tab_es_z[nptb] = zc + z;
        nptb++;
      }
    }

    if (FERMETURE_EN_1)
    {
      /* fermeture par l'element structurant */
      if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function ldilat3 failed\n", argv[0]);
        exit(1);
      }

      if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function leros3 failed\n", argv[0]);
        exit(1);
      }
      /* ouverture par l'element structurant */
      if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function leros3 failed\n", argv[0]);
        exit(1);
      }

      if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function ldilat3 failed\n", argv[0]);
        exit(1);
      }
    }
    else
    {
      /* ouverture par l'element structurant */
      if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function leros3 failed\n", argv[0]);
        exit(1);
      }

      if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function ldilat3 failed\n", argv[0]);
        exit(1);
      }
      /* fermeture par l'element structurant */
      if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function ldilat3 failed\n", argv[0]);
        exit(1);
      }

      if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
      {
        fprintf(stderr, "%s: function leros3 failed\n", argv[0]);
        exit(1);
      }
    }

  } /* for (rayon = 1; rayon <= rayonmax; rayon++) */

  writeimage(image, argv[argc-1]);
  freeimage(image);
  free(tab_es_x);
  free(tab_es_y);
  free(tab_es_z);

  return 0;
} /* main */
