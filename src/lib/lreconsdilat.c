/* $Id: lreconsdilat.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* operateur de reconstruction par dilatation de g sous f */
/* methode : propagation des changements par fifo */
/* d'apres la these de Michel Grimaud (pp 22) */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcfifo.h>
#include <mcindic.h>
#include <mcutil.h>
#include <lreconsdilat.h>

/* ==================================== */
int32_t lreconsdilat(
        struct xvimage *g,
        struct xvimage *f,
        uint8_t *mask) 
/* reconstruction de g sous f */
/* g : image marqueur */
/* f : image masque */
/* mask : masque du 8-voisinage representant l'element structurant */
/* resultat dans g */
/* ==================================== */
{
  int32_t nbchang, iter;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(g);         /* taille ligne */
  int32_t cs = colsize(g);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *G = UCHARDATA(g);      /* l'image marqueur */
  uint8_t *F = UCHARDATA(f);      /* l'image masque */
  uint8_t *H;                     /* image de travail */
  uint8_t *temp;
  uint8_t sup;
  Fifo * FIFO[2];

  if ((rowsize(f) != rs) || (colsize(f) != cs))
  {
    fprintf(stderr, "lreconsdilat: tailles incompatibles\n");
    return 0;
  }

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lreconsdilat: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  FIFO[0] = CreeFifoVide(N);
  FIFO[1] = CreeFifoVide(N);
  if ((FIFO[0] == NULL) || (FIFO[1] == NULL))
  {   fprintf(stderr,"lreconseros() : CreeFifoVide failed\n");
      return(0);
  }

  IndicsInit(N);

  for (x = 0; x < N; x++)      /* mise en fifo initiale de tous les points */    
  {
    FifoPush(FIFO[1], x);
    Set(x, 1);
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"lreconsdilat() : malloc failed for H\n");
      return(0);
  }

  iter = 0;
  do
  {
    iter += 1;
    nbchang = 0;
    while (! FifoVide(FIFO[iter % 2]))
    {
      x = FifoPop(FIFO[iter % 2]);
      UnSet(x, iter % 2);
      sup = G[x];
      for (k = 0; k < 8; k += 1)
      {
        if (mask[k])                      /* element structurant */
	{
          y = voisin(x, k, rs, N);
          if ((y != -1) && (G[y] > sup)) sup = G[y];
        }
      } /* for k */

      sup = min(sup, F[x]);
      if (G[x] != sup) /* changement: on enregistre x ainsi que ses voisins */
      {
        nbchang += 1;
        if (! IsSet(x, (iter + 1) % 2))
        {
          FifoPush(FIFO[(iter + 1) % 2], x);
          Set(x, (iter + 1) % 2);
	}
        for (k = 0; k < 8; k += 1)
        {
          y = voisin(x, k, rs, N);
          if ((y != -1) && (! IsSet(y, (iter + 1) % 2)))
          {
            FifoPush(FIFO[(iter + 1) % 2], y);
            Set(y, (iter + 1) % 2);
          }
        } /* for k */
      }
      H[x] = sup;

    } /* while ! FifoVide */
    
    /* echange les roles de G et H */
    temp = G;
    G = H;
    H = temp;

#ifdef VERBOSE
    printf("iteration %d, nbchang %d\n", iter, nbchang);
#endif
  } while (nbchang != 0);

  /* remet le resultat dans g si necessaire */
  if (G != UCHARDATA(g))
  {
    for (x = 0; x < N; x++)
      (UCHARDATA(g))[x] = G[x];
    free(G);
  }
  else
    free(H);

  FifoTermine(FIFO[0]);
  FifoTermine(FIFO[1]);
  return 1;
}











