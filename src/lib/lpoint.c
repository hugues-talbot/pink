/* $Id: lpoint.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* modifie la valeur d'un point dans une image */
/* Michel Couprie - avril 2000 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lpoint.h>

/* ==================================== */
int32_t lpoint(struct xvimage * image1, int32_t x, int32_t y, int32_t z, float v)
/* ==================================== */
#undef F_NAME
#define F_NAME "lpoint"
{
  int32_t i;
  int32_t rs, cs, d, n;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  n = rs * cs;
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *pt1 = UCHARDATA(image1);
    if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < d) &&
        (v >= NDG_MIN) && (v <= NDG_MAX))
      pt1[z * n + y * rs + x] = (uint8_t)v;
    else
    {
      fprintf(stderr, "%s : out of range\n", F_NAME);
      return 0;
    }
  }
  else
  if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *pt1 = ULONGDATA(image1);
    if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < d) &&
        (v >= 0))
      pt1[z * n + y * rs + x] = (uint32_t)v;
    else 
    {
      fprintf(stderr, "%s : out of range\n", F_NAME);
      return 0;
    }
  }
  else
  if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *pt1 = FLOATDATA(image1);
    if ((x >= 0) && (x < rs) && (y >= 0) && (y < cs) && (z >= 0) && (z < d))
      pt1[z * n + y * rs + x] = v;
    else 
    {
      fprintf(stderr, "%s : out of range\n", F_NAME);
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "%s: bad data type\n", F_NAME);
    return 0;
  }

  return 1;
} // lpoint()

/* ==================================== */
void lshowpoint(struct xvimage * image1, int32_t x, int32_t y, int32_t z)
/* ==================================== */
#undef F_NAME
#define F_NAME "lshowpoint"
{
  int32_t i;
  int32_t rs, cs, d, n;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  n = rs * cs;
  
  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *pt1 = UCHARDATA(image1);
    printf("%d\n", pt1[z * n + y * rs + x]);
  }
  else
  if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *pt1 = ULONGDATA(image1);
    printf("%ld\n", pt1[z * n + y * rs + x]);
  }
  else
  if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *pt1 = FLOATDATA(image1);
    printf("%g\n", pt1[z * n + y * rs + x]);
  }
  else
  {
    fprintf(stderr, "%s: bad data type\n", F_NAME);
    return;
  }

} // lshowpoint()
