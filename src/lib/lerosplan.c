/* $Id: lerosplan.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* operateur d'erosion numerique par un element structurant plan de taille quelconque */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t lerosplan(
        struct xvimage *f,
        struct xvimage *m,  
        int32_t xc,
        int32_t yc)
/* m : masque representant l'element structurant */
/* xc, yc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;                    /* index muet */
  register int32_t k, l;                    /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t Nm = rsm * csm;
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  uint8_t inf;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t c;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lerosplan: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"lerosplan() : malloc failed for H\n");
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL))
  {  
     fprintf(stderr,"lerosplan() : malloc failed for tab_es\n");
     return(0);
  }

  k = 0;
  for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
      if (M[j * rsm + i])
      {
         tab_es_x[k] = i;
         tab_es_y[k] = j;
         k += 1;
      }

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    inf = NDG_MAX;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_es_y[c] - yc;
      k = x + tab_es_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs) && (H[l * rs + k] < inf))
	inf = H[l * rs + k];
    }
    F[y * rs + x] = inf;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  return 1;
}

/* ==================================== */
int32_t lerosplan2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t xc, int32_t yc)
/* nptb :                   nombre de points de l'e.s. */
/* tab_es_x :               liste des coord. x des points de l'e.s. */
/* tab_es_y :               liste des coord. y des points de l'e.s. */
/* xc, yc :                 coordonnees du "centre" de l'element structurant */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;           /* index muet */
  register int32_t k, l;           /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;            /* image de travail */
  uint8_t inf;
  int32_t c;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lerosplan2: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"lerosplan2() : malloc failed for H\n");
     return(0);
  }

  memcpy(H, F, N);

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    inf = NDG_MAX;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_es_y[c] - yc;
      k = x + tab_es_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs) && (H[l * rs + k] < inf))
	inf = H[l * rs + k];
    }
    F[y * rs + x] = inf;
  }

  free(H);
  return 1;
} /* lerosplan2() */







