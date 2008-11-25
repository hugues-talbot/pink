/* $Id: lrotate.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/*
   Rotations d'angles multiples de PI/2 autour des axes x, y, z

OBSOLETE - see lisometry.c et lrotations.c

   Michel Couprie  -  Novembre 1999
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lrotate.h>

/* ==================================== */
int32_t lrotate(struct xvimage * image1, char *mode, struct xvimage **im2)
/* ==================================== */
#undef F_NAME
#define F_NAME "lrotate"
{
  struct xvimage *image2;
  uint8_t *I1, *I2;
  int32_t rs, cs, d, ps, nps;
  int32_t i, j, k;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  ps = rs * cs;
  I1 = UCHARDATA(image1);

  if (strcmp(mode, "xzy") == 0)
  {
    image2 = allocimage(NULL, rs, d, cs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = rs * d;
    I2 = UCHARDATA(image2);
    for (k = 0; k < cs; k++)
      for (j = 0; j < d; j++)
        for (i = 0; i < rs; i++)
          I2[k * nps + j * rs + i] = I1[j * ps + k * rs + i]; 
  }
  else
  if (strcmp(mode, "yxz") == 0)
  {
    image2 = allocimage(NULL, cs, rs, d, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = rs * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < d; k++)
      for (j = 0; j < rs; j++)
        for (i = 0; i < cs; i++)
          I2[k * nps + j * cs + i] = I1[k * ps + i * rs + j]; 
  }
  else
  if (strcmp(mode, "yzx") == 0)
  {
    image2 = allocimage(NULL, cs, d, rs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < rs; k++)
      for (j = 0; j < d; j++)
        for (i = 0; i < cs; i++)
          I2[k * nps + j * cs + i] = I1[j * ps + i * rs + k]; 
  }
  else
  if (strcmp(mode, "zxy") == 0)
  {
    image2 = allocimage(NULL, d, rs, cs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * rs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < cs; k++)
      for (j = 0; j < rs; j++)
        for (i = 0; i < d; i++)
          I2[k * nps + j * d + i] = I1[i * ps + k * rs + j]; 
  }
  else
  if (strcmp(mode, "zyx") == 0)
  {
    image2 = allocimage(NULL, d, cs, rs, VFF_TYP_1_BYTE);
    if (image2 == NULL)
    {   
      fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
    }
    nps = d * cs;
    I2 = UCHARDATA(image2);
    for (k = 0; k < rs; k++)
      for (j = 0; j < cs; j++)
        for (i = 0; i < d; i++)
          I2[k * nps + j * d + i] = I1[i * ps + j * rs + k]; 
  }
  else
  {   
    fprintf(stderr,"%s: bad mode : %s\n", F_NAME, mode);
    fprintf(stderr,"available modes: xzy, yxz, yzx, zxy, zyx\n");
    return 0;
  }
  *im2 = image2;
  return 1;
} // lrotate()


/* ==================================== */
int32_t lrotategen(
  struct xvimage * in,
  double x,
  double y,
  double theta)
/* ==================================== */



// PAS FINI





#undef F_NAME
#define F_NAME "lrotategen"
{
  int32_t x2, y2, z2, xs, ys, zs, xi, yi, zi;
  uint8_t *I;
  uint8_t *T;
  int32_t rs, cs, ds, ps;
  struct xvimage * tmp,

  rs = in->row_size;
  cs = in->col_size;
  ds = depth(in);
  ps = rs * cs;
  I = UCHARDATA(in);

  if (datatype(in) != VFF_TYP_1_BYTE)
  {  
    fprintf(stderr,"%s : bad data type\n", F_NAME);
    return 0;
  }

  tmp = copyimage(in);

  if (tmp == NULL)
  {
    fprintf(stderr,"%s : allocimage failed\n", F_NAME);
    return 0;
  }
  T = UCHARDATA(tmp);

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  memset(I, 0, N2);
  if (ds == 1)
  {
    double f, f1, f2;
    for (y2 = 0; y2 < cs2; y2++)
    {
      for (x2 = 0; x2 < rs2; x2++)
      {
        x = x2 / zoomx;
        y = y2 / zoomy;
        xi = (int32_t)floor(x); xs = xi + 1;
        yi = (int32_t)floor(y); ys = yi + 1;
        if ((xi >= 0) && (yi >= 0) && (xs < rs) && (ys < cs))
        {
          f1 = (x - xi) * I[yi*rs + xs] + (xs - x) * I[yi*rs + xi];
          f2 = (x - xi) * I[ys*rs + xs] + (xs - x) * I[ys*rs + xi];
          f = (y - yi) * f2 + (ys - y) * f1;
          T[y2*rs2 + x2] = arrondi(f);
        }
      } // for x2
    } // for y2

  } /* if (ds == 1) */
  else
  {
    fprintf(stderr,"%s: 3D not yet implemented\n", F_NAME);
    return 0;
  } /* if (ds != 1) */

  return 1;
} /* lrotategen */





