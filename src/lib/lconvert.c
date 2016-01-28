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
