#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <lhisto.h>
#include <mcsort.h>

struct xvimage* llong2byte(struct xvimage*imagelong, int32_t mode, int32_t nbnewval)
{
  static char *name="llong2byte";
  
  int32_t *L;
  uint8_t *B;
  int32_t i;
  int32_t Max;
  index_t *histo;
  index_t *newvals;
  int32_t nbval;
  int32_t *index;
  double t;
  index_t x, rs, cs, ds, N;
  
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagelong) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: image type must be int32_t\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagelong));
    return NULL;
  }

  rs = rowsize(imagelong);
  cs = colsize(imagelong);
  ds = depth(imagelong);
  N = rs * cs * ds;
  L = SLONGDATA(imagelong);
  
  struct xvimage *imagebyte = allocimage(imagelong->name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  B = UCHARDATA(imagebyte);
  imagebyte->xdim = imagelong->xdim;
  imagebyte->ydim = imagelong->ydim;
  imagebyte->zdim = imagelong->zdim;
  
  switch(mode)
  {
    case 0:
      for (x = 0; x < N; x++)
        B[x] = (uint8_t)mcmax(mcmin(L[x],255),0);
      break;
    case 1:
      for (x = 0; x < N; x++)
        B[x] = (uint8_t)(L[x] % 256);
      break;
    case 2:
      Max = L[0];
      for (x = 0; x < N; x++) if (L[x] > Max) Max = L[x];
      if (Max > 255)
        for (x = 0; x < N; x++) 
          B[x] = (uint8_t)(L[x] * 255 / Max);
      else
        for (x = 0; x < N; x++) 
          B[x] = (uint8_t)L[x];
      break;
    case 3:
      if (! lhistolong(imagelong, NULL, &histo, &nbval))
      {
        fprintf(stderr, "%s: function lhistolong failed\n", name);
	freeimage(imagebyte);
        return NULL;
      }
      index = (int32_t *)calloc(1, nbval * sizeof(int32_t));
      if (index == NULL)
      {
        fprintf(stderr, "%s: malloc failed\n", name);
	freeimage(imagebyte);
        return NULL;
      }
      for (i = 0; i < nbval; i++) index[i] = i;
      i_TriRapideStochastique (index, histo, 1, nbval-1);
      newvals = histo; /* reutilisation de la place memoire allouee pour histo */
      for (i = 0; i < nbval; i++) newvals[i] = 0;
      for (i = 1; i < mcmin(nbval,nbnewval); i++) 
	newvals[index[i]] = i;
      for (x = 0; x < N; x++) 
        B[x] = (uint8_t)(newvals[L[x]]);
      free(histo);
      free(index);      
      break;
    case 4:
      for (x = 0; x < N; x++)
      {
        t = sqrt((double)(L[x]));
        if (t > 255) t = 255;
        B[x] = (uint8_t)arrondi(t);
      }
      break;
    case 5:
      for (x = 0; x < N; x++)
      {
        t = log((double)(L[x]));
        if (t > 255) t = 255;
        B[x] = (uint8_t)arrondi(t);
      }
      break;
    default:
      fprintf(stderr, "usage: %s(imagelong, mode=0, nbfirst=255)\n", name);
      fprintf(stderr, "mode = 0 (trunc) | 1 (modulo 256) | 2 (scale) | 3 (trunchisto)\n");
      fprintf(stderr, "       4 (square root) | 5 (log)\n");
      freeimage(imagebyte);
      return NULL;
  } /* switch(mode) */

  return imagebyte;
}

struct xvimage* lfloat2byte(struct xvimage*imagefloat, int32_t mode)
{
  static char *name="lfloat2byte";

  float *L;
  uint8_t *B;
  int32_t tmp;
  float Min, Max, t;
  double T;
  index_t x, rs, cs, ds, N;

  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagefloat) != VFF_TYP_FLOAT)
  {
    fprintf(stderr, "%s: image type must be float\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagefloat));
    return NULL;
  }

  rs = rowsize(imagefloat);
  cs = colsize(imagefloat);
  ds = depth(imagefloat);
  N = rs * cs * ds;
  L = FLOATDATA(imagefloat);
  
  struct xvimage *imagebyte = allocimage(imagefloat->name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  B = UCHARDATA(imagebyte);
  imagebyte->xdim = imagefloat->xdim;
  imagebyte->ydim = imagefloat->ydim;
  imagebyte->zdim = imagefloat->zdim;

  switch(mode)
  {
    case 0:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)mcmin(tmp,255);
      }
      break;
    case 1:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)(tmp % 256);
      }
      break;
    case 2:
      Min = Max = L[0];
      for (x = 0; x < N; x++) 
        if (L[x] > Max) Max = L[x]; else if (L[x] < Min) Min = L[x];
      for (x = 0; x < N; x++) 
      {
        t = ((L[x]-Min) * 255.0) / (float)(Max-Min);
        tmp = arrondi(t);
        B[x] = (uint8_t)mcmin(255,tmp);
      }
      break;
    case 4:
      for (x = 0; x < N; x++)
      {
        T = sqrt((double)(L[x]));
        tmp = arrondi(T);
        tmp = mcmin(255,tmp);
        tmp = mcmax(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    case 5:
      for (x = 0; x < N; x++)
      {
        T = log((double)(L[x]));
        tmp = arrondi(T);
        tmp = mcmin(255,tmp);
        tmp = mcmax(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    default:
      fprintf(stderr, "usage: %s(imagefloat, mode=0)\n", name);
      fprintf(stderr, "mode = 0 (trunc) | 1 (modulo) | 2 (scale) | \n");
      fprintf(stderr, "       4 (square root) | 5 (log)\n");
      freeimage(imagebyte);
      return NULL;
  } /* switch(mode) */

  return imagebyte;
}

struct xvimage* ldouble2byte(struct xvimage*imagedouble, int32_t mode)
{
  static char *name="ldouble2byte";

  double *L;
  uint8_t *B;
  int32_t tmp;
  double Min, Max, t;
  double T;
  index_t x, rs, cs, ds, N;

  if (imagedouble == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagedouble) != VFF_TYP_DOUBLE)
  {
    fprintf(stderr, "%s: image type must be double\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagedouble));
    return NULL;
  }

  rs = rowsize(imagedouble);
  cs = colsize(imagedouble);
  ds = depth(imagedouble);
  N = rs * cs * ds;
  L = DOUBLEDATA(imagedouble);
  
  struct xvimage *imagebyte = allocimage(imagedouble->name, rs, cs, ds, VFF_TYP_1_BYTE);
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  B = UCHARDATA(imagebyte);
  imagebyte->xdim = imagedouble->xdim;
  imagebyte->ydim = imagedouble->ydim;
  imagebyte->zdim = imagedouble->zdim;

  switch(mode)
  {
    case 0:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)mcmin(tmp,255);
      }
      break;
    case 1:
      for (x = 0; x < N; x++)
      {
        tmp = arrondi(L[x]);
        B[x] = (uint8_t)(tmp % 256);
      }
      break;
    case 2:
      Min = Max = L[0];
      for (x = 0; x < N; x++) 
        if (L[x] > Max) Max = L[x]; else if (L[x] < Min) Min = L[x];
      for (x = 0; x < N; x++) 
      {
        t = ((L[x]-Min) * 255.0) / (double)(Max-Min);
        tmp = arrondi(t);
        B[x] = (uint8_t)mcmin(255,tmp);
      }
      break;
    case 4:
      for (x = 0; x < N; x++)
      {
        T = sqrt((double)(L[x]));
        tmp = arrondi(T);
        tmp = mcmin(255,tmp);
        tmp = mcmax(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    case 5:
      for (x = 0; x < N; x++)
      {
        T = log((double)(L[x]));
        tmp = arrondi(T);
        tmp = mcmin(255,tmp);
        tmp = mcmax(0,tmp);
        B[x] = (uint8_t)tmp;
      }
      break;
    default:
      fprintf(stderr, "usage: %s(imagedouble, mode=0)\n", name);
      fprintf(stderr, "mode = 0 (trunc) | 1 (modulo) | 2 (scale) | \n");
      fprintf(stderr, "       4 (square root) | 5 (log)\n");
      freeimage(imagebyte);
      return NULL;
  } /* switch(mode) */

  return imagebyte;
}

struct xvimage* llong2float(struct xvimage*imagelong)
{
  static char *name="llong2float";
  float *L;
  int32_t *B;
  int32_t x;
  int32_t rs, cs, ds, N;
  
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagelong) != VFF_TYP_4_BYTE)
  {
    fprintf(stderr, "%s: image type must be long\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagelong));
    return NULL;
  }

  rs = rowsize(imagelong);
  cs = colsize(imagelong);
  ds = depth(imagelong);
  N = rs * cs * ds;
  B = SLONGDATA(imagelong);
  
  struct xvimage *imagefloat = allocimage(imagelong->name, rs, cs, ds, VFF_TYP_FLOAT);
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  L = FLOATDATA(imagefloat);
  imagefloat->xdim = imagelong->xdim;
  imagefloat->ydim = imagelong->ydim;
  imagefloat->zdim = imagelong->zdim;

  for (x = 0; x < N; x++)
    L[x] = (float)B[x];

  return imagefloat;
}

struct xvimage* lbyte2float(struct xvimage*imagebyte)
{
  static char *name="lbyte2float";
  float *L;
  uint8_t *B;
  int32_t x;
  int32_t rs, cs, ds, N;
  
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagebyte) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: image type must be byte\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagebyte));
    return NULL;
  }

  rs = rowsize(imagebyte);
  cs = colsize(imagebyte);
  ds = depth(imagebyte);
  N = rs * cs * ds;
  B = UCHARDATA(imagebyte);
  
  struct xvimage *imagefloat = allocimage(imagebyte->name, rs, cs, ds, VFF_TYP_FLOAT);
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  L = FLOATDATA(imagefloat);
  imagefloat->xdim = imagebyte->xdim;
  imagefloat->ydim = imagebyte->ydim;
  imagefloat->zdim = imagebyte->zdim;

  for (x = 0; x < N; x++)
    L[x] = (float)B[x];

  return imagefloat;
}

struct xvimage* lbyte2long(struct xvimage*imagebyte)
{
  static char *name="lbyte2long";
  int32_t *L;
  uint8_t *B;
  int32_t x;
  int32_t rs, cs, ds, N;
  
  if (imagebyte == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagebyte) != VFF_TYP_1_BYTE)
  {
    fprintf(stderr, "%s: image type must be byte\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagebyte));
    return NULL;
  }

  rs = rowsize(imagebyte);
  cs = colsize(imagebyte);
  ds = depth(imagebyte);
  N = rs * cs * ds;
  B = UCHARDATA(imagebyte);
  
  struct xvimage *imagelong = allocimage(imagebyte->name, rs, cs, ds, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  L = SLONGDATA(imagelong);
  imagelong->xdim = imagebyte->xdim;
  imagelong->ydim = imagebyte->ydim;
  imagelong->zdim = imagebyte->zdim;

  for (x = 0; x < N; x++)
    L[x] = (int32_t)B[x];

  return imagelong;
}

struct xvimage* lfloat2long(struct xvimage*imagefloat, double offset, double factor)
{
  static char *name="lfloat2long";
  int32_t *L;
  uint8_t *B;
  int32_t x;
  int32_t rs, cs, ds, N;
  
  if (imagefloat == NULL)
  {
    fprintf(stderr, "%s: image is NULL.\n", name);
    return NULL;
  }

  if (datatype(imagefloat) != VFF_TYP_FLOAT)
  {
    fprintf(stderr, "%s: image type must be float\n", name);
    fprintf(stderr, "type found: %d\n", datatype(imagefloat));
    return NULL;
  }

  rs = rowsize(imagefloat);
  cs = colsize(imagefloat);
  ds = depth(imagefloat);
  N = rs * cs * ds;
  B = UCHARDATA(imagefloat);
  
  struct xvimage *imagelong = allocimage(imagefloat->name, rs, cs, ds, VFF_TYP_4_BYTE);
  if (imagelong == NULL)
  {
    fprintf(stderr, "%s: allocimage failed\n", name);
    return NULL;
  }

  L = SLONGDATA(imagelong);
  imagelong->xdim = imagefloat->xdim;
  imagelong->ydim = imagefloat->ydim;
  imagelong->zdim = imagefloat->zdim;

  for (x = 0; x < N; x++) {
    double T = offset + (factor * (double)(B[x]));
    L[x] = (int32_t)arrondi(T);
  }
  
  return imagelong;
}
