/* $Id: lremspnoise.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* operateur de detection de bruit de type "salt and peper"
   avec correction par moyennage local */
/* d'apres Kak et Rosenfeld */
/* Michel Couprie - janvier 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t lremspnoise(
        struct xvimage *image,
        int32_t g,
        int32_t k)
/* g : nombre de niveaux d'ecart */
/* k : nombre de voisins consideres */
/* ==================================== */
{
  int32_t i, v, n, j, m, nv;
  uint8_t *pt;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;
  uint8_t *imagetmp;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lhtkern: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  pt = UCHARDATA(image);

  imagetmp = (uint8_t *)calloc(1,N*sizeof(char));
  if (imagetmp == NULL)
  {   printf("lremspnoise() : malloc failed\n");
      return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  for (i = 0; i < N; i++)
  {
    /* detection du bruit : compte le nombre de voisins qui 
       different de plus de g niveaux
    */
    n = 0;
    nv = 0;
    m = 0;
    for (v = 0; v < 8; v++)  /* 8-voisins */
      if ((j = voisin(i, v, rs, N)) != -1)
      {
        if (abs(((int32_t)pt[i] - (int32_t)pt[j]) >= g)) n++;
        nv++;        /* compte les voisins */
        m += pt[j];  /* pour le calcul de la moyenne */
      }
    
    if (n >= k)
    {
      /* moyennage : remplace la valeur du point par la moyenne de ses
         8 voisins 
      */
      imagetmp[i] = (uint8_t)(m / nv);
    }
    else
      imagetmp[i] = pt[i];
  }

  for (i = 0; i < N; i++)
    pt[i] = imagetmp[i];

  free(imagetmp);
  return 1;
}
