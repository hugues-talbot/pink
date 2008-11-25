/* $Id: lselcomp.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur de reconstruction par dilatation de g sous f */
/* Camille Couprie - novembre 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <lselcomp.h>

/* ==================================== */
int32_t lselcomp(struct xvimage *f, int32_t connex, int32_t x, int32_t y, int32_t z) 
/* ==================================== */
{
  int32_t i, j, k, h, couleurobjet, couleurfond;                 /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(f); /* l'image originale */
  Lifo * LIFO;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lselcomp: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO[0] == NULL)
  {   fprintf(stderr,"lselcomp() : CreeLifoVide failed\n");
      return(0);
  }
  
  i = y*rs + x;
  couleurobjet = F[i];
  if (couleurobjet != 0) couleurfond = 0; else couleurfond = 255;
  Lifopush (LIFO, i);
  if (connex == 4) p = 2;
  if (connex == 8) p = 1;
  
  while ( ! LifoVide(LIFO))
  {
    i = LifoPop(LIFO);
    F[i] = 100;
    for (k = 0; k < 8; k = k + p)
    {
      j = voisin(i, k, rs, N);
      if ((j != -1) && (couleurinitiale == F[j])) LifoPush(LIFO, j);
    }  
  }
  
  for ( h = 0; h < N; h++ )
  {
    if ( F[h] == couleurobjet) F[h] = couleurfond;
    if ( F[h] == 100 )         F[h] = couleurobjet;
  }
  
  LifoTermine(LIFO);
  return 1;
}











