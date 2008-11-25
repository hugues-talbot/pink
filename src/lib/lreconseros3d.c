/* $Id: lreconseros3d.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* obsolete - voir lgeodesic.c */
/* operateur de reconstruction 3D par erosion de g au dessus de f */
/* methode : propagation des changements par fifo */
/* d'apres la these de Michel Grimaud (pp 22) */
/* Michel Couprie - novembre 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcfifo.h>
#include <mcindic.h>
#include <mcutil.h>
#include <lreconseros3d.h>

/* ==================================== */
int32_t lreconseros3d(
        struct xvimage *g,
        struct xvimage *f,
        uint8_t *mask) 
/* reconstruction par erosion de g au dessus de f */
/* g : image marqueur */
/* f : image masque */
/* mask : masque du 26-voisinage representant l'element structurant */
/* resultat dans g */
/* ==================================== */
{
  int32_t nbchang, iter;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(g);         /* taille ligne */
  int32_t cs = colsize(g);         /* taille colonne */
  int32_t d = depth(g);            /* nombre plans */           
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *G = UCHARDATA(g);      /* l'image marqueur (au dessus de f) */
  uint8_t *F = UCHARDATA(f);      /* l'image masque */
  uint8_t *H;                     /* image de travail */
  uint8_t *temp;
  uint8_t inf;
  Fifo * FIFO[2];

  if ((rowsize(f) != rs) || (colsize(f) != cs) || (depth(f) != d))
  {
    fprintf(stderr, "lreconseros3d: tailles incompatibles\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"lreconseros3d() : malloc failed for H\n");
      return(0);
  }

  FIFO[0] = CreeFifoVide(N);
  FIFO[1] = CreeFifoVide(N);
  if ((FIFO[0] == NULL) || (FIFO[1] == NULL))
  {   fprintf(stderr,"lreconseros3d() : CreeFifoVide failed\n");
      return(0);
  }

  IndicsInit(N);

  for (x = 0; x < N; x++)      /* mise en fifo initiale de tous les points */    
  {
    FifoPush(FIFO[1], x);
    Set(x, 1);
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
      inf = G[x];
      for (k = 0; k < 26; k += 1)
      {
        if (mask[k])                      /* element structurant */
	{
          y = voisin26(x, k, rs, n, N);
          if ((y != -1) && (G[y] < inf)) inf = G[y];
	}
      } /* for k */

      inf = max(inf, F[x]);
      if (G[x] != inf)           /* le point a change : on l'enregistre ainsi que ses voisins */
      {
        nbchang += 1;
        if (! IsSet(x, (iter + 1) % 2))
        {
          FifoPush(FIFO[(iter + 1) % 2], x);
          Set(x, (iter + 1) % 2);
	}
        for (k = 0; k < 26; k += 1)
        {
          y = voisin26(x, k, rs, n, N);
          if ((y != -1) && (! IsSet(y, (iter + 1) % 2)))
          {
            FifoPush(FIFO[(iter + 1) % 2], y);
            Set(y, (iter + 1) % 2);
          }
        } /* for k */
      }
      H[x] = inf;

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
