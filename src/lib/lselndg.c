/* $Id: lselndg.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* selection de pixels ayant un niveau de gris compris entre 2 bornes */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>

/* ==================================== */
int32_t lselndg(struct xvimage * image, int32_t inf, int32_t sup)               
/* on selectionne les pixels x tels que inf <= x <= sup */
/* ==================================== */
#undef F_NAME
#define F_NAME "lselndg"
{
  int32_t i;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *pt;
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
      if ((*pt >= inf) && (*pt <= sup)) *pt = NDG_MAX; else *pt = NDG_MIN;
  }
  else
  if (datatype(image) == VFF_TYP_4_BYTE)
  {
    int32_t *pt;
    for (pt = SLONGDATA(image), i = 0; i < N; i++, pt++)
      if (!((*pt >= inf) && (*pt <= sup))) *pt = NDG_MIN;
  }
  else
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
}
