/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/*! \file asf.c

\brief alternate sequential filter

<B>Usage:</B> asf in.pgm [rmin] rmax out.pgm

<B>Description:</B>
Alternate sequential filter for grayscale images (for binary images use preferably asfbin). 
Disc-shaped structuring elements of increasing radius,
ranging from 1 (or <B>rmin</B> if this parameter is specified) to <B>rmax</B>,
are generated. Let D1...Dn be these structuring elements, sorted by increasing radius.
Let I0 = <B>in.pgm</B>, the ith intermediate result Ii is obtained by the closing of
the opening of Ii-1 by the structuring element Di. 
The result <B>out.pgm</B> contains the final result In.
Giving a value 0 for the optional parameter <B>rmin</B> has the effect of beginning 
by a closing instead of an opening.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie

*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <ldilateros.h>

#define VERBOSE

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  int32_t rayonmin, rayonmax, rayon;
  int32_t d, xc, yc, zc, x, y, z, nptb;
  int32_t *tab_es_x, *tab_es_y, *tab_es_z;
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

  if (depth(image) == 1)
  {
    d = 2 * rayonmax + 1;
    tab_es_x = (int32_t *)calloc(1,d * d * sizeof(int32_t));
    tab_es_y = (int32_t *)calloc(1,d * d * sizeof(int32_t));
    if ((tab_es_x == NULL) || (tab_es_y == NULL))
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }
  
    for (rayon = rayonmin; rayon <= rayonmax; rayon++)
    {
  #ifdef VERBOSE
      fprintf(stderr, "%s: rayon = %d\n", argv[0], rayon);
  #endif
      /* genere l'element structurant circulaire sous forme de liste des coord. des points */
      yc = xc = rayon; 
      d = 2 * rayon + 1;
      nptb = 0;
      for (x = 0; x < d; x++)
        for (y = 0; y < d; y++)
          if (((x - xc) * (x - xc) + (y - yc) * (y - yc)) <= (rayon * rayon))
  	{
            tab_es_x[nptb] = x;
            tab_es_y[nptb] = y;
            nptb++;
  	}
  
      if (FERMETURE_EN_1)
      {
        /* fermeture par l'element structurant */
        if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
        /* ouverture par l'element structurant */
        if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
      } 
      else 
      {
        /* ouverture par l'element structurant */
        if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
        /* fermeture par l'element structurant */
        if (! ldilat2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! leros2(image, nptb, tab_es_x, tab_es_y, xc, yc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
      }  
    } /* for (rayon = 1; rayon <= rayonmax; rayon++) */
    free(tab_es_x);
    free(tab_es_y);
  }
  else
  {
    d = 2 * rayonmax + 1;
    tab_es_x = (int32_t *)calloc(1,d * d * d * sizeof(int32_t));
    tab_es_y = (int32_t *)calloc(1,d * d * d * sizeof(int32_t));
    tab_es_z = (int32_t *)calloc(1,d * d * d * sizeof(int32_t));
    if ((tab_es_x == NULL) || (tab_es_y == NULL) || (tab_es_z == NULL))
    {
      fprintf(stderr, "%s: malloc failed\n", argv[0]);
      exit(1);
    }
  
    for (rayon = rayonmin; rayon <= rayonmax; rayon++)
    {
  #ifdef VERBOSE
      fprintf(stderr, "%s: rayon = %d\n", argv[0], rayon);
  #endif
      /* genere l'element structurant circulaire sous forme de liste des coord. des points */
      zc = yc = xc = rayon; 
      d = 2 * rayon + 1;
      nptb = 0;
      for (x = 0; x < d; x++)
        for (y = 0; y < d; y++)
          for (z = 0; z < d; z++)
          if (((x - xc) * (x - xc) + (y - yc) * (y - yc) + (z - zc) * (z - zc)) <= (rayon * rayon))
  	{
            tab_es_x[nptb] = x;
            tab_es_y[nptb] = y;
            tab_es_z[nptb] = z;
            nptb++;
  	}
  
      if (FERMETURE_EN_1)
      {
        /* fermeture par l'element structurant */
        if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
        /* ouverture par l'element structurant */
        if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
      } 
      else 
      {
        /* ouverture par l'element structurant */
        if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
        /* fermeture par l'element structurant */
        if (! ldilat3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function ldilat2 failed\n", argv[0]);
          exit(1);
        }
  
        if (! leros3(image, nptb, tab_es_x, tab_es_y, tab_es_z, xc, yc, zc))
        {
          fprintf(stderr, "%s: function leros2 failed\n", argv[0]);
          exit(1);
        }
      }
    } /* for (rayon = 1; rayon <= rayonmax; rayon++) */
    free(tab_es_x);
    free(tab_es_y);
    free(tab_es_z);
  }

  writeimage(image, argv[argc-1]);
  freeimage(image);

  return 0;
} /* main */
