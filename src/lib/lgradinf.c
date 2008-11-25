/* $Id: lgradinf.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* gradient inf */
/* Michel Couprie - aout 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mctopo.h>

/* ==================================== */
int32_t lgradinf(struct xvimage *image)
/* ==================================== */
{
  int32_t i;
  uint8_t *pt;
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;
  uint8_t *imagetmp;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lgradinf: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  imagetmp = (uint8_t *)calloc(1,N*sizeof(char));
  if (imagetmp == NULL)
  {   printf("lgradinf() : malloc failed\n");
      return(0);
  }

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  pt = UCHARDATA(image);
  for (i = 0; i < N; i++)
    imagetmp[i] = pt[i] - beta8m(pt, i, rs, N);

  for (i = 0; i < N; i++)
    pt[i] = imagetmp[i];

  free(imagetmp);
  return 1;
}
