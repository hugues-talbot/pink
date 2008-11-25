/* $Id: lline.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* draws a straight line between the two first points found in image */
/* Michel Couprie - octobre 2002 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lbresen.h>
#include <lline.h>

/* ==================================== */
int32_t lline(struct xvimage * image1)
/* ==================================== */
#undef F_NAME
#define F_NAME "lline"
{
  int32_t i, first = -1, last;
  uint8_t *F;
  int32_t rs, cs, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  N = rs * cs;
  F = UCHARDATA(image1);
  
  for (i = 0; i < N; i++)
  {
    if (F[i])
    {
      if (first == -1) first = i;
      else {last = i; break;}
    }
  }
  lbresen(F, rs, first%rs, first/rs, last%rs, last/rs);
  return 1;
}





