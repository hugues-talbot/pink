/* $Id: larith.c,v 1.5 2009-06-18 06:34:55 mcouprie Exp $ */
/* 
  operations arithmetiques : 
    ladd
    laddconst
    larea
    laverage
    laverage1
    linverse
    lequal
    ldiff
    ldivide
    linf
    lmax
    lmax1
    lmin
    lmin1
    lmult
    lneg
    lnormalize
    lnull
    lscale
    lsub
    lsup
    lvolume
    lxor
*/
/* Michel Couprie - juillet 1996 */
/* Camille Couprie - octobre 2002 (xor) */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <mcutil.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <larith.h>

/* ==================================== */
int32_t ladd(
  struct xvimage * image1,
  struct xvimage * image2)
/* somme de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "ladd"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = (uint8_t)min(NDG_MAX, (int32_t)*pt1 + (int32_t)*pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = *PT1 + *PT2;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = *FPT1 + *FPT2;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* ladd() */

/* ==================================== */
int32_t laddconst(struct xvimage * image1, int32_t constante)
/* ajoute une constante a une image  - seuil si depassement */
/* ==================================== */
#undef F_NAME
#define F_NAME "laddconst"
{
  int32_t i;
  uint8_t *pt1;
  uint32_t *lpt1;
  float *FPT1; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  
  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    pt1 = UCHARDATA(image1);
    for (i = 0; i < N; i++)
    {
      pt1[i] = (uint8_t)min(NDG_MAX, max(NDG_MIN, (int32_t)(pt1[i]) + constante));
    }
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    lpt1 = ULONGDATA(image1);
    for (i = 0; i < N; i++)
    {
      lpt1[i] = (uint32_t)min(((uint32_t)ULONG_MAX),max(NDG_MIN,(int32_t)(lpt1[i])+constante));
    }
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    FPT1 = FLOATDATA(image1);
    for (i = 0; i < N; i++)
    {
      FPT1[i] = FPT1[i] + (float)constante;
    }
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }
  return 1;
} /* laddconst() */

/* ==================================== */
int32_t larea(
  struct xvimage * image, 
  int32_t *area)
/* retourne le nombre de pixels non nuls */
/* ==================================== */
#undef F_NAME
#define F_NAME "larea"
{
  int32_t i, a = 0;
  uint8_t *pt;
  uint32_t *PT; 
  float *FPT; 
  int32_t rs, cs, d, N;

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++) if (*pt) a++;
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    for (PT = ULONGDATA(image), i = 0; i < N; i++, PT++) if (*PT) a++;
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    for (FPT = FLOATDATA(image), i = 0; i < N; i++, FPT++) if (*FPT != 0.0) a++;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }
  *area = a;
  return 1;
} /* larea() */

/* ==================================== */
int32_t laverage(
  struct xvimage * image1,
  struct xvimage * image2,
  double alpha)
/* moyenne ponderee de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "laverage"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = (uint8_t)((alpha * *pt1) + ((1.0 - alpha) * *pt2));
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = (uint32_t)((alpha * *PT1) + ((1.0 - alpha) * *PT2));
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = (float)((alpha * *FPT1) + ((1.0 - alpha) * *FPT2));
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* laverage() */

/* ==================================== */
double laverage1(struct xvimage * image1)
/* average of values in an image */
/* ==================================== */
#undef F_NAME
#define F_NAME "laverage1"
{
  int32_t i, rs, cs, d, N, n = 0;
  double av = 0.0;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image1);
    for (i = 1; i < N; i++) if ((double)F[i] != 0) { av += (double)F[i]; n++; }
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(image1);
    for (i = 1; i < N; i++) if ((double)F[i] != 0) { av += (double)F[i]; n++; }
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(image1);
    for (i = 1; i < N; i++) if ((double)F[i] != 0) { av += (double)F[i]; n++; }
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    exit(0);
  }

  if (n == 0) return 0.0;
  return av / n;
} /* laverage1() */

/* ==================================== */
double laverage2(struct xvimage * image1, struct xvimage * mask)
/* average of values in an image */
/* ==================================== */
#undef F_NAME
#define F_NAME "laverage2"
{
  int32_t i, rs, cs, d, N, n = 0;
  double av = 0.0;
  uint8_t *M = UCHARDATA(mask);

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;

  if ((rowsize(mask) != rs) || (colsize(mask) != cs) || (depth(mask) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if (datatype(mask) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: mask image must be byte\n", F_NAME);
    exit(0);
  }

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image1);
    for (i = 1; i < N; i++) if (M[i] != 0) { av += (double)F[i]; n++; }
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(image1);
    for (i = 1; i < N; i++) if (M[i] != 0) { av += (double)F[i]; n++; }
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(image1);
    for (i = 1; i < N; i++) if (M[i] != 0) { av += (double)F[i]; n++; }
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    exit(0);
  }

  if (n == 0) return 0.0;
  return av / n;
} /* laverage2() */

/* ==================================== */
int32_t ldiff(
  struct xvimage * image1,
  struct xvimage * image2)
/* difference en valeur absolue de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldiff"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = (uint8_t)abs((int32_t)*pt1-(int32_t)*pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = abs(*PT1-*PT2);
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = (float)fabs((float)(*FPT1)-(float)(*FPT2));
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* ldiff() */

/* ==================================== */
int32_t ldivide(
  struct xvimage * image1,
  struct xvimage * image2)
/* quotient (pixel par pixel) de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "ldivide"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt2 != 0) *pt1 = *pt1 / *pt2; else *pt1 = 0;
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      if (*PT2 != 0) *PT1 = *PT1 / *PT2; else *PT1 = 0;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      if (*FPT2 != 0.0) *FPT1 = *FPT1 / *FPT2; else *FPT1 = 0.0;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* ldivide() */

/* ==================================== */
int32_t lequal(
  struct xvimage * image1,
  struct xvimage * image2)
/* test d'egalite de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lequal"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 != *pt2) return 0;
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      if (*PT1 != *PT2) return 0;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      if (*FPT1 != *FPT2) return 0;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lequal() */

/* ==================================== */
int32_t linf(
  struct xvimage * image1,
  struct xvimage * image2)
/* prédicat inf pixelwise */
/* ==================================== */
#undef F_NAME
#define F_NAME "linf"
{
  int32_t i;
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    uint8_t *pt1, *pt2;
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 <= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    uint32_t *pt1, *pt2; 
    for (pt1 = ULONGDATA(image1), pt2 = ULONGDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 <= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    float *pt1, *pt2; 
    for (pt1 = FLOATDATA(image1), pt2 = FLOATDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 <= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* linf() */

/* ==================================== */
int32_t linverse(
  struct xvimage * image)
/* inverse d' une image */
/* ==================================== */
#undef F_NAME
#define F_NAME "linverse"
{
  int32_t i;
  uint8_t *pt;
  uint32_t *PT, vmax;
  int32_t N;

  N = rowsize(image) * colsize(image) * depth(image);

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
      *pt = NDG_MAX - *pt;
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    vmax = 0;
    for (PT = ULONGDATA(image), i = 0; i < N; i++, PT++)
      if (*PT > vmax) vmax = *PT;
    for (PT = ULONGDATA(image), i = 0; i < N; i++, PT++)
      *PT = vmax - *PT;
  }
  else
  {
    fprintf(stderr, "%s: bad image type\n", F_NAME);
    return 0;
  }

  return 1;
} /* linverse() */

/* ==================================== */
int32_t lmax(
  struct xvimage * image1,
  struct xvimage * image2)
/* max de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmax"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = max(*pt1, *pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = max(*PT1, *PT2);
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = max(*FPT1, *FPT2);
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lmax() */

/* ==================================== */
double lmax1(struct xvimage * image1)
/* maximum value in an image */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmax1"
{
  int32_t i, rs, cs, d, N;
  double maxval;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image1);
    maxval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] > maxval) maxval = (double)F[i];
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(image1);
    maxval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] > maxval) maxval = (double)F[i];
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(image1);
    maxval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] > maxval) maxval = (double)F[i];
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    exit(0);
  }

  return maxval;
} /* lmax1() */

/* ==================================== */
double lmin1(struct xvimage * image1)
/* minimum value in an image */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmin1"
{
  int32_t i, rs, cs, d, N;
  double minval;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    uint8_t *F = UCHARDATA(image1);
    minval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] < minval) minval = (double)F[i];
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    uint32_t *F = ULONGDATA(image1);
    minval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] < minval) minval = (double)F[i];
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    float *F = FLOATDATA(image1);
    minval = (double)F[0];
    for (i = 1; i < N; i++) if ((double)F[i] < minval) minval = (double)F[i];
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    exit(0);
  }

  return minval;
} /* lmin1() */

/* ==================================== */
int32_t lmin(
  struct xvimage * image1,
  struct xvimage * image2)
/* min de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmin"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = min(*pt1, *pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = min(*PT1, *PT2);
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = min(*FPT1, *FPT2);
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lmin() */

/* ==================================== */
int32_t lmult(
  struct xvimage * image1,
  struct xvimage * image2)
/* produit de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmult"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = (uint8_t)min(NDG_MAX, (int32_t)*pt1 * (int32_t)*pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = *PT1 * *PT2;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = *FPT1 * *FPT2;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lmult() */

/* ==================================== */
int32_t lneg(
  struct xvimage * image)
/* negation d' une image de booleens */
/* ==================================== */
#undef F_NAME
#define F_NAME "lneg"
{
  int32_t i;
  uint8_t *pt;
  uint32_t *PT, vmax;
  int32_t N;

  N = rowsize(image) * colsize(image) * depth(image);

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
      if (*pt) *pt = 0; else *pt = NDG_MAX;
  }
  else
  {
    fprintf(stderr, "%s: bad image type\n", F_NAME);
    return 0;
  }

  return 1;
} /* lneg() */

/* ==================================== */
int32_t lnormalize(struct xvimage * image, float nmin, float nmax)
/* ==================================== */
#undef F_NAME
#define F_NAME "lnormalize"
#define EPSILON 1e-6
{
  int32_t x, N;

  if (nmin > nmax)
  {
    fprintf(stderr, "%s: bad output range\n", F_NAME);
    return 0;
  }

  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    uint8_t *Im;
    uint8_t ndgmin, ndgmax;
    uint8_t Nmin = arrondi(nmin);
    uint8_t Nmax = arrondi(nmax);
    N = rowsize(image) * colsize(image) * depth(image);
    Im = UCHARDATA(image);
    ndgmin = ndgmax = Im[0];
    for (x = 0; x < N; x++)
    {
      if (Im[x] < ndgmin) ndgmin = Im[x];
      else if (Im[x] > ndgmax) ndgmax = Im[x];
    }
    ndgmax = ndgmax - ndgmin;
    if (ndgmax == 0) ndgmax = 1;
    for (x = 0; x < N; x++)
      Im[x] = Nmin + ((Im[x] - ndgmin) * (Nmax-Nmin)) / ndgmax;
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    uint32_t *Im;
    uint32_t ndgmin, ndgmax;
    uint32_t Nmin = arrondi(nmin);
    uint32_t Nmax = arrondi(nmax);
    N = rowsize(image) * colsize(image) * depth(image);
    Im = ULONGDATA(image);
    ndgmin = ndgmax = Im[0];
    for (x = 0; x < N; x++)
    {
      if (Im[x] < ndgmin) ndgmin = Im[x];
      else if (Im[x] > ndgmax) ndgmax = Im[x];
    }
    ndgmax = ndgmax - ndgmin;
    if (ndgmax == 0) ndgmax = 1;
    for (x = 0; x < N; x++)
      Im[x] = Nmin + ((Im[x] - ndgmin) * (Nmax-Nmin)) / ndgmax;
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    float *Im;
    float ndgmin, ndgmax;
    N = rowsize(image) * colsize(image) * depth(image);
    Im = FLOATDATA(image);
    ndgmin = ndgmax = Im[0];
    for (x = 0; x < N; x++)
    {
      if (Im[x] < ndgmin) ndgmin = Im[x];
      else if (Im[x] > ndgmax) ndgmax = Im[x];
    }
    ndgmax = ndgmax - ndgmin;
    if (ndgmax < EPSILON) ndgmax = 1.0;
    for (x = 0; x < N; x++)
      Im[x] = nmin + ((Im[x] - ndgmin) * (nmax-nmin)) / ndgmax;
  }
  else
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} // lnormalize()

/* ==================================== */
int32_t lnull(struct xvimage * image1)
/* test de nullite d'une image */
/* ==================================== */
#undef F_NAME
#define F_NAME "lnull"
{
  int32_t i;
  uint8_t *pt1;
  uint32_t *PT1; 
  float *FPT1; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;

  if (datatype(image1) == VFF_TYP_1_BYTE)
  {
    for (pt1 = UCHARDATA(image1), i = 0; i < N; i++, pt1++)
      if (*pt1) return 0;
  }
  else if (datatype(image1) == VFF_TYP_4_BYTE)
  {
    for (PT1 = ULONGDATA(image1), i = 0; i < N; i++, PT1++)
      if (*PT1) return 0;
  }
  else if (datatype(image1) == VFF_TYP_FLOAT)
  {
    for (FPT1 = FLOATDATA(image1), i = 0; i < N; i++, FPT1++)
      if (*FPT1 != 0.0) return 0;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lnull() */

/* ==================================== */
int32_t lscale(
  struct xvimage * image,
  double scale)
/* produit d' une image par un scalaire - seuil a NDG_MAX */
/* ==================================== */
#undef F_NAME
#define F_NAME "lscale"
{
  char buf[32];
  int32_t i;
  uint8_t *pt;
  uint32_t *PT;
  float *FPT;
  int32_t N;

  N = rowsize(image) * colsize(image) * depth(image);

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */
  
  if (datatype(image) == VFF_TYP_1_BYTE)
  {
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++)
    {
      *pt = (uint8_t)min(NDG_MAX, (int32_t)(*pt * scale));
    }
  }
  else if (datatype(image) == VFF_TYP_4_BYTE)
  {
    for (PT = ULONGDATA(image), i = 0; i < N; i++, PT++)
    {
      *PT = (uint32_t)(*PT * scale);
    }
  }
  else if (datatype(image) == VFF_TYP_FLOAT)
  {
    for (FPT = FLOATDATA(image), i = 0; i < N; i++, PT++)
    {
      *FPT = (float)(*FPT * scale);
    }
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }
  return 1;
} /* lscale() */

/* ==================================== */
int32_t lsub(
  struct xvimage * image1,
  struct xvimage * image2)
/* difference de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lsub"
{
  int32_t i;
  uint8_t *pt1, *pt2;
  uint32_t *PT1, *PT2; 
  float *FPT1, *FPT2; 
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      *pt1 = (uint8_t)max(NDG_MIN, (int32_t)*pt1 - (int32_t)*pt2);
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    for (PT1 = ULONGDATA(image1), PT2 = ULONGDATA(image2), i = 0; i < N; i++, PT1++, PT2++)
      *PT1 = (uint32_t)max(NDG_MIN, (int32_t)*PT1 - (int32_t)*PT2);
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    for (FPT1 = FLOATDATA(image1), FPT2 = FLOATDATA(image2), i = 0; i < N; i++, FPT1++, FPT2++)
      *FPT1 = *FPT1 - *FPT2;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lsub() */

/* ==================================== */
int32_t lsup(
  struct xvimage * image1,
  struct xvimage * image2)
/* prédicat inf pixelwise */
/* ==================================== */
#undef F_NAME
#define F_NAME "lsup"
{
  int32_t i;
  int32_t rs, cs, d, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  d = depth(image1);
  N = rs * cs * d;
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    uint8_t *pt1, *pt2;
    for (pt1 = UCHARDATA(image1), pt2 = UCHARDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 >= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else if ((datatype(image1) == VFF_TYP_4_BYTE) && (datatype(image2) == VFF_TYP_4_BYTE))
  {
    uint32_t *pt1, *pt2; 
    for (pt1 = ULONGDATA(image1), pt2 = ULONGDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 >= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else if ((datatype(image1) == VFF_TYP_FLOAT) && (datatype(image2) == VFF_TYP_FLOAT))
  {
    float *pt1, *pt2; 
    for (pt1 = FLOATDATA(image1), pt2 = FLOATDATA(image2), i = 0; i < N; i++, pt1++, pt2++)
      if (*pt1 >= *pt2) *pt1 = NDG_MAX; else *pt1 = NDG_MIN;
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lsup() */

/* ==================================== */
int32_t lvolume(
  struct xvimage * image, 
  double *vol)
/* retourne la somme des valeurs de pixels */
/* ==================================== */
#undef F_NAME
#define F_NAME "lvolume"
{
  int32_t i;
  uint8_t *pt;
  uint32_t *PT; 
  float *FPT; 
  double fvolume = 0.0; 
  int32_t rs, cs, d, N;

  rs = rowsize(image);
  cs = colsize(image);
  d = depth(image);
  N = rs * cs * d;

  if (datatype(image) == VFF_TYP_1_BYTE)
    for (pt = UCHARDATA(image), i = 0; i < N; i++, pt++) fvolume += (double)*pt;
  else 
  if (datatype(image) == VFF_TYP_4_BYTE)
    for (PT = ULONGDATA(image), i = 0; i < N; i++, PT++) fvolume += (double)*PT;
  else 
  if (datatype(image) == VFF_TYP_FLOAT)
    for (FPT = FLOATDATA(image), i = 0; i < N; i++, FPT++) fvolume += (double)*FPT;
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }
  *vol = fvolume;
  return 1;
} /* lvolume() */

/* ==================================== */
int32_t lxor(
  struct xvimage * image1,
  struct xvimage * image2)
/* xor de 2 images */
/* ==================================== */
#undef F_NAME
#define F_NAME "lxor"
{
  int32_t i;
  uint8_t *F1, *F2;
  int32_t rs, cs, ds, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  N = rs * cs * ds;
  F1 = UCHARDATA(image1);
  F2 = UCHARDATA(image2);
  
  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != ds))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    exit(0);
  }

  if ((datatype(image1) == VFF_TYP_1_BYTE) && (datatype(image2) == VFF_TYP_1_BYTE))
  {
    for (i = 0; i < N; i++)
      {
        if (((F1[i] == 0) && (F2[i] == 0)) || ((F1[i] != 0) && (F2[i] != 0)))
	  F1[i]=0;
        else 
     	  F1[i]=255;
      }
  }
  else 
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  return 1;
} /* lxor() */
