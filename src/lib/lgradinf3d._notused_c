/* $Id: lgradinf3d.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* gradient inf 3d */
/* Michel Couprie - juillet 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mctopo.h>

/* ==================================== */
int32_t lgradinf3d(struct xvimage *image)
/* ==================================== */
{
  int32_t i;
  uint8_t *pt;
  int32_t rs = rowsize(image);
  int32_t cs = colsize(image);
  int32_t d = depth(image);
  int32_t n = rs * cs;
  int32_t N = d * n;
  uint8_t *imagetmp;

  imagetmp = (uint8_t *)calloc(1,N*sizeof(char));
  if (imagetmp == NULL)
  {   printf("lgradinf3d() : malloc failed\n");
      return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  pt = UCHARDATA(image);
  for (i = 0; i < N; i++)
    imagetmp[i] = pt[i] - beta26m(pt, i, rs, n, N);

  for (i = 0; i < N; i++)
    pt[i] = imagetmp[i];

  free(imagetmp);
  return 1;
}
