/* $Id: lconvol3.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* convolution par un masque 3x3 */
/* Michel Couprie - juillet 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>

/* ==================================== */
int32_t lconvol3(struct xvimage * image, struct xvimage * mask, int32_t normalise)
/* ==================================== */
{
  int32_t i, x, y;
  uint8_t *img;
  char *msk;                        /* LE MASQUE EST UNE IMAGE SIGNEE */
  int32_t rs, cs, N, sum;
  uint8_t *trv;                     /* image de travail */

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lconvol3: cette version ne traite pas les images volumiques\n");
    exit(0);
  }
  rs = image->row_size;
  cs = image->col_size;
  N = rs * cs;
  trv = (uint8_t *)calloc(1,N*sizeof(char));
  if (trv == NULL)
  {   fprintf(stderr,"lconvol3() : malloc failed for trv\n");
      return(0);
  }

  /* ---------------------------------------------------------- */
  /* recupere et verifie les operandes */
  /* ---------------------------------------------------------- */

  if ((mask->row_size != 3) || (mask->col_size != 3))
  {
    fprintf(stderr, "convol3: bad mask size\n");
    return 0;
  }
    
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  msk = (char *)(UCHARDATA(mask));
  img = UCHARDATA(image);
  for (i = 0; i < N; i++) trv[i] = img[i];

  sum = 0;
  for (i = 0; i < 9; i++) sum += msk[i];

  for (y = 1; y < cs-1; y++)
    for (x = 1; x < rs-1; x++)
    {
      int32_t tmp = *(trv + y*rs + x) * msk[4];
      tmp += *(trv + (y-1)*rs + x-1) * msk[0];
      tmp += *(trv + (y-1)*rs + x)   * msk[1];
      tmp += *(trv + (y-1)*rs + x+1) * msk[2];
      tmp += *(trv + y*rs + x-1)     * msk[3];
      tmp += *(trv + y*rs + x+1)     * msk[5];
      tmp += *(trv + (y+1)*rs + x-1) * msk[6];
      tmp += *(trv + (y+1)*rs + x)   * msk[7];
      tmp += *(trv + (y+1)*rs + x+1) * msk[8];
      if (normalise) tmp = tmp / sum;
      if (tmp < 0) tmp = 0; else if (tmp > 255) tmp = 255;
      *(img + y*rs + x) = (uint8_t)tmp;
    }

  free(trv);
  return 1;
}





