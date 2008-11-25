/* $Id: lnbvois.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* compte le nombre de voisins de chaque point dans une image binaire */
/* remarque : on peut operer directement dans l'image de depart ! */
/* Michel Couprie - juin 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <lnbvois.h>

/* ==================================== */
int32_t lnbvois(
  struct xvimage * image,
  int32_t connex)
/* ==================================== */
{
  int32_t nvois, k;
  int32_t p, q;
  int32_t rs = rowsize(image);
  int32_t cs = colsize(image);
  int32_t ds = depth(image);
  int32_t ps = rs * cs;          /* taille plan */
  int32_t N = ps * ds;           /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */

  switch (connex)
  {
    case 4: 
      for (p = 0; p < N; p++)
        if (F[p])
        {
          nvois = 0;                /* compte le nombre de voisins non nuls */
          for (k = 0; k < 8; k += 2)
          {
            q = voisin(p, k, rs, N);
            if ((q != -1) && (F[q])) nvois++;
          }
          F[p] = nvois;
        }
      break;
    case 8: 
      for (p = 0; p < N; p++)
        if (F[p])
        {
          nvois = 0;                /* compte le nombre de voisins non nuls */
          for (k = 0; k < 8; k += 1)
          {
            q = voisin(p, k, rs, N);
            if ((q != -1) && (F[q])) nvois++;
          }
          F[p] = nvois;
        }
      break;
    case 6: 
      for (p = 0; p < N; p++)
        if (F[p])
        {
          nvois = 0;                /* compte le nombre de voisins non nuls */
          for (k = 0; k <= 10; k += 2)
          {
            q = voisin6(p, k, rs, ps, N);
            if ((q != -1) && (F[q])) nvois++;
          }
          F[p] = nvois;
        }
      break;
    case 18: 
      for (p = 0; p < N; p++)
        if (F[p])
        {
          nvois = 0;                /* compte le nombre de voisins non nuls */
          for (k = 0; k < 18; k += 1)
          {
            q = voisin18(p, k, rs, ps, N);
            if ((q != -1) && (F[q])) nvois++;
          }
          F[p] = nvois;
        }
      break;
    case 26: 
      for (p = 0; p < N; p++)
        if (F[p])
        {
          nvois = 0;                /* compte le nombre de voisins non nuls */
          for (k = 0; k < 26; k += 1)
          {
            q = voisin26(p, k, rs, ps, N);
            if ((q != -1) && (F[q])) nvois++;
          }
          F[p] = nvois;
        }
      break;
    default: 
      fprintf(stderr, "lnbvois: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */
  return 1;
}
