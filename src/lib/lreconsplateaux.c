/* $Id: lreconsplateaux.c,v 1.1.1.1 2008-11-25 08:01:44 mcouprie Exp $ */
/* operateur de reconstruction de plateaux marques par une image binaire */
/* Michel Couprie - mai 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>

#define MARQUE_ORIG 1
#define MARQUE_FIN 2

/* ==================================== */
int32_t lreconsplateaux(
        struct xvimage *f,
        struct xvimage *g,
        int32_t connex)
/* reconstruction de plateaux de f marques par g */
/* g : image marqueur */
/* f : image plateaux */
/* connex = 4|8 */
/* resultat dans f */
/* ==================================== */
{
  int32_t rs = rowsize(f);
  int32_t cs = colsize(f);
  int32_t d = depth(f);
  int32_t N = rs * cs;          /* taille image */
  uint32_t *F = SLONGDATA(f);
  uint8_t *G = UCHARDATA(g);
  Lifo * LIFO;
  int32_t incr_vois;
  int32_t i, j, k, x, y, z;
  uint32_t val;

  if (d != 1) 
  {
    fprintf(stderr, "lreconsplateaux: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  if ((rowsize(g) != rs) || (colsize(g) != cs) || (depth(g) != d))
  {
    fprintf(stderr, "lreconsplateaux: incompatible image sizes\n");
    return 0;
  }

  if (datatype(f) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "lreconsplateaux: le type du pixel doit etre VFF_TYP_4_BYTE\n");
    return 0;
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "lreconsplateaux: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lreconsplateaux() : CreeLifoVide failed\n");
      return(0);
  }

  for (x = 0; x < N; x++) if (G[x]) G[x] = MARQUE_ORIG;

  for (x = 0; x < N; x++)
  {
    if (G[x] == MARQUE_ORIG)
    {
      /* ============================================================== */
      /* parcourt la composante */
      /* ============================================================== */

      val = F[x];
      G[x] = MARQUE_FIN;
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        z = LifoPop(LIFO);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(z, k, rs, N);
          if ((y != -1) && (F[y] == val) && (G[y] != MARQUE_FIN))
	  {
            G[y] = MARQUE_FIN;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */
    } /* if (G[x] == MARQUE_ORIG) */
  } /* for (x = 0; x < N; x++) */

  LifoTermine(LIFO);

  /* ============================================================== */
  /* elimine les plateaux non marques */
  /* ============================================================== */

  for (x = 0; x < N; x++)
    if (G[x] != MARQUE_FIN) F[x] = 0;      
  return 1;
}











