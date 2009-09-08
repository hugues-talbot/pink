/* $Id: lhisto.c,v 1.3 2009-09-08 09:06:02 mcouprie Exp $ */
/* histogramme d'une image en niveaux de gris */
/* histogramme bivariable */
/* Michel Couprie - juillet 1996, novembre 1999 */
/* update 6/4/2006 John Chaussard : cor. bug */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <lhisto.h>

/* ==================================== */
int32_t lhisto(struct xvimage *image, struct xvimage *mask, uint32_t *histo)
/* ==================================== */
{
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nombre plans */
  int32_t N = rs * cs * d;         /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint8_t *M;

  for (i = 0; i <= NDG_MAX; i++) histo[i] = 0;

  if (mask == NULL)
    for (x = 0; x < N; x++) histo[SOURCE[x]] += 1;
  else
  {
    M = UCHARDATA(mask);
    for (x = 0; x < N; x++) if (M[x]) histo[SOURCE[x]] += 1;
  }
  return(1);
} /* lhisto() */

/* ==================================== */
int32_t lhisto1(struct xvimage *image, uint32_t *histo)
/* ==================================== */
{
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nombre plans */
  int32_t N = rs * cs * d;         /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */

  for (i = 0; i <= NDG_MAX; i++) histo[i] = 0;
  for (x = 0; x < N; x++) histo[SOURCE[x]] += 1;
  return(1);
} /* lhisto1() */

/* ==================================== */
int32_t lhisto2(struct xvimage *image1, struct xvimage *image2, 
            struct xvimage *mask, struct xvimage *histo)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lhisto2"
  int32_t i;
  int32_t x;                        /* index muet de pixel */
  int32_t rs = rowsize(image1);     /* taille ligne */
  int32_t cs = colsize(image1);     /* taille colonne */
  int32_t d = depth(image1);        /* nombre plans */
  int32_t N = rs * cs * d;          /* taille image */
  uint8_t *SOURCE1 = UCHARDATA(image1);      /* l'image de depart 1 */
  uint8_t *SOURCE2 = UCHARDATA(image2);      /* l'image de depart 2 */
  uint8_t *M;
  uint32_t *H = ULONGDATA(histo);
  int32_t rsh = rowsize(histo);
  int32_t nh = rsh * colsize(histo);

  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "%s: incompatible image sizes\n", F_NAME);
    return 0;
  }
  if ((datatype(image1) != VFF_TYP_1_BYTE) || (datatype(image2) != VFF_TYP_1_BYTE))
  {
    fprintf(stderr, "%s: bad image type(s)\n", F_NAME);
    return 0;
  }

  for (i = 0; i < nh; i++) H[i] = 0;

  if (mask == NULL)
    for (x = 0; x < N; x++) H[SOURCE2[x] * rsh + SOURCE1[x]] += 1;
  else
  {
    M = UCHARDATA(mask);
    for (x = 0; x < N; x++) if (M[x]) H[SOURCE2[x] * rsh + SOURCE1[x]] += 1;
  }
  return(1);
}  /* lhisto2() */

/* ==================================== */
int32_t lhistolong(struct xvimage *image, struct xvimage *mask, uint32_t **histo, int32_t *size)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lhistolong"
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nombre plans */
  int32_t N = rs * cs * d;         /* taille image */
  uint32_t *F = ULONGDATA(image);      /* l'image de depart */
  uint8_t *M;
  int32_t s;

  s = F[0];
  for (i = 1; i < N; i++) 
    if ((int32_t)(F[i]) > s) 
      s = (int32_t)(F[i]);
  s += 1;      /* pour la valeur 0 */
  *size = s;

  *histo = (uint32_t *)calloc(s, sizeof(int32_t));
  if (*histo == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", F_NAME);
    return 0;
  }

  if (mask == NULL)
  {
    for (x = 0; x < N; x++) (*histo)[F[x]] += 1;
  }
  else
  {
    M = UCHARDATA(mask);
    for (x = 0; x < N; x++) if (M[x]) (*histo)[F[x]] += 1;
  }

  return(1);
} // histolong()

/* ==================================== */
int32_t lhistofloat(struct xvimage *image, struct xvimage *mask, uint32_t **histo, int32_t *size, 
		float *Sincr, float *Smin, float *Smax)
/* ==================================== */
{
#undef F_NAME
#define F_NAME "lhistofloat"
#define NBINS 256
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nombre plans */
  int32_t N = rs * cs * ds;        /* taille image */
  float *F = FLOATDATA(image);      /* l'image de depart */
  uint8_t *M;
  float smin, smax, s, sincr;

  if (mask == NULL)
  {
    smin = smax = F[0];
    for (i = 1; i < N; i++) 
      if (F[i] > smax) smax = F[i]; else
	if (F[i] < smin) smin = F[i];
  }
  else
  {
    M = UCHARDATA(mask);
    for (i = 0; i < N; i++) 
      if (M[i])
      { smin = smax = F[i]; break; }
    for (; i < N; i++) 
      if (M[i])
      {
	if (F[i] > smax) smax = F[i]; else
	  if (F[i] < smin) smin = F[i];
      }
  } // else mask == NULL
  
  *size = NBINS;
  *Sincr = sincr = (smax - smin) / NBINS;
  *Smin = smin;
  *Smax = smax;

  *histo = (uint32_t *)calloc(NBINS, sizeof(int32_t));
  if (*histo == NULL)
  {
    fprintf(stderr, "%s: calloc failed\n", F_NAME);
    return 0;
  }

  if (mask == NULL)
    for (x = 0; x < N; x++)
    {
      for (s = smin, i = 0; i < NBINS; s += sincr, i++)
	if (F[x] < s) break;
      (*histo)[i-1] += 1;
    }
  else
  {
    M = UCHARDATA(mask);
    for (x = 0; x < N; x++)
    {
      if (M[x])
      {
	for (s = smin, i = 0; i < NBINS; s += sincr, i++)
	  if (F[x] < s) break;
	(*histo)[i-1] += 1;
      }
    }
  }

  return(1);
} // lhistofloat()

/* ==================================== */
void lhistcompact(uint32_t *histo, int32_t n)
/* ==================================== */
/* n niveaux de gris consecutifs sont compactes en un seul */
{
  int32_t i, t, i0 = -1;

  for (i = NDG_MIN; i <= NDG_MAX; i++) 
  {
    if ((i % n) == 0) 
    { 
      if (i0 != -1) histo[i0] = t; 
      t = histo[i]; 
      i0++; 
      histo[i0] = 0;
    } 
    else
      t += histo[i]; 
  }
  histo[i0] += t;
  for (i = i0 + 1; i <= NDG_MAX; i++) histo[i] = 0;
}

/* ==================================== */
int32_t lhistsum(uint32_t *histo)
/* ==================================== */
{
  int32_t i, t;

  t = 0;
  for (i = NDG_MIN; i <= NDG_MAX; i++) t += histo[i]; 
  return t;
}

/* ==================================== */
int32_t lhistdilat(uint32_t *histo, int32_t n)
/* ==================================== */
/* dilatation numerique par un es. plan, centre, de taille 2n+1 */
{
#undef F_NAME
#define F_NAME "lhistdilat"
  int32_t i, j, m;
  uint32_t *h;

  h = (uint32_t *)calloc(1,(NDG_MAX + 1) * sizeof(int32_t));
  if (h == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return 0;
  }

  for (i = NDG_MIN; i <= NDG_MAX; i++) 
  {
    m = histo[i];
    for (j = 1; j <= n; j++)
    {
      if (((i - j) >= NDG_MIN) && (histo[i - j] > m)) m = histo[i - j];
      if (((i + j) <= NDG_MAX) && (histo[i + j] > m)) m = histo[i + j];
    }
    h[i] = m; 
  }
  for (i = NDG_MIN; i <= NDG_MAX; i++) histo[i] = h[i];
  free(h);
  return 1;
}

/* ==================================== */
void labelextr1d(int32_t *F, int32_t n, uint8_t *E)
/* ==================================== */
/*
  renvoie dans E les etiquettes des points de F : 
    1 : MAX
    2 : MIN
    3 : ni MAX ni MIN
*/
{
  int32_t i;
  for (i = 0; i < n; i++) E[i] = 0;
  E[0] = NONMIN | NONMAX;
  for (i = 1; i < n; i++) 
    if (F[i] < F[i-1])      E[i] |= NONMAX;
    else if (F[i] > F[i-1]) E[i] |= NONMIN;
    else                    E[i] |= E[i-1];
  E[n-1] = NONMIN | NONMAX;
  for (i = n-2; i >= 0; i--)
    if (F[i] < F[i+1])      E[i] |= NONMAX;
    else if (F[i] > F[i+1]) E[i] |= NONMIN;
    else                    E[i] |= E[i+1];
} /* labelextr1d() */

/* ==================================== */
int32_t lhisto2image(uint32_t *histo, int32_t size, struct xvimage **image)
/* ==================================== */
/*
  Genere une image 2d representant un histogramme. 
  Les barres verticales representent les "bins",
  les barres horizontales donnent l'echelle (1 barre tous les 1000, 10000... pixels)
*/
{
#undef F_NAME
#define F_NAME "lhisto2image"
  int32_t i, j, hmax, hbin, tailleclasse;
  int32_t rs, cs;
  uint8_t *I;
  int32_t interbar;
  const int32_t SizeX=512;
  const int32_t SizeY=256;

  // Creation d'une image pour l'histogramme
  if (size<SizeX)
    rs = size;
  else
    rs = SizeX;
  cs = SizeY;
  *image = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
  if (*image == NULL)
  {   fprintf(stderr,"%s: allocimage failed\n", F_NAME);
      return 0;
  }
  I = UCHARDATA((*image));
  memset(I, NDG_MAX, rs*cs);

  // calcule la taille d'une classe
  tailleclasse=size/rs;
  // Compactifie l'histogramme (diminue le nb de classes)
  if (tailleclasse != 1) {
    for (i=0; i<rs; i++) {
      for (j=0; j<tailleclasse; j++) {
	histo[i] = histo[i*tailleclasse+j];
      }
    }
    size = rs;
  }

  // calcule la hauteur max d'un bin
  hmax = 0;
  for (i = 0; i < size; i++) if (histo[i] > hmax) hmax = histo[i];
  if (hmax <= 50000) interbar = 1000;
  else if (hmax <= 500000) interbar = 10000;
  else interbar = 100000;

  for (i = 0; i < size; i++) // trace les bins
  {
    hbin = (int32_t)(((double)histo[i] * (double)(SizeY-6)) / (double)hmax);

    for (j = 0; j < hbin; j++)
      I[(cs-j-1)*rs + i] = NDG_MIN;
  } // for (i = 0; i <= NDG_MAX; i++)
  
  i = 1;
  hbin = (int32_t)(((double)interbar * (double)i * (double)(SizeY-6)) / (double)hmax);
  while (hbin < SizeY)
  {
    for (j = 0; j <size; j++)
    I[(cs-hbin-1)*rs + j] = NDG_MIN;
    i++;
    hbin = (int32_t)(((double)interbar * (double)i * (double)(SizeY-6)) / (double)hmax);
  }

#ifdef VERBOSE
  printf("interbar = %d pixels\n", interbar);
#endif

  return(1);
} /* lhisto2image() */
