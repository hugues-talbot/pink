/* $Id: ldir.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* selectionne les points est, sud, ouest ou nord */
/* Michel Couprie - decembre 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcindic.h>
#include <ldir.h>

/* ==================================== */
int32_t ldir(struct xvimage * image1, int32_t dir)
/* ==================================== */
{
  int32_t i, v;
  uint8_t *pt1;
  int32_t rs, cs, N;

  if (depth(image1) != 1) 
  {
    fprintf(stderr, "ldir: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  rs = rowsize(image1);
  cs = colsize(image1);
  N = rs * cs;
  
  IndicsInit(N);

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  pt1 = UCHARDATA(image1);
  for (i = 0; i < N; i++)
    if (pt1[i])
    {
      v = voisin(i, dir, rs, N);
      if ((v != -1) && (pt1[v] < pt1[i]))
         Set(i,0);
    }

  for (i = 0; i < N; i++) if (IsSet(i,0)) pt1[i] = NDG_MAX; else pt1[i] = NDG_MIN;

  IndicsTermine();
  return 1;
}





