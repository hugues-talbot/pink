/* $Id: lcomphisto.c,v 1.1.1.1 2008-11-25 08:01:44 mcouprie Exp $ */
/* operateur: compression d'histogramme */
/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <lcomphisto.h>

/* ==================================== */
int32_t lcomphisto(struct xvimage *image, int32_t n)
/* n: nombre de niveaux de gris desires */
/* ==================================== */
{
  int32_t i;
  int32_t j;
  int32_t k;
  int32_t x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t N = rs * cs * d;         /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint32_t * histo;
  uint8_t * tabcor;
  int32_t nndg;
  double p, np;

  histo = (uint32_t *)calloc(1,(NDG_MAX - NDG_MIN + 1) * sizeof(int32_t));
  if (histo == NULL)
  {
    fprintf(stderr, "lcomphisto: malloc failed for histo\n");
    return(0);
  }

  tabcor = (uint8_t *)calloc(NDG_MAX - NDG_MIN + 1,  sizeof(char));
  if (tabcor == NULL)
  {
    fprintf(stderr, "lcomphisto: calloc failed for tabcor\n");
    return(0);
  }

  if (! lhisto(image, histo))
  {
    fprintf(stderr, "lcomphisto: function lhisto failed\n");
    return(0);
  }  

  nndg = 0;
  for (i = NDG_MIN; i <= NDG_MAX; i++) 
    if (histo[i])
      nndg++;

  if (n > nndg)
  {
    fprintf(stderr, "lcomphisto: required histogram larger than actual one\n");
    return(0);
  }  

  p = (double)nndg / (double)n;      
  np = 0;

  i = NDG_MIN;   /* niveau de gris */
  k = 0;         /* nombre de niveaux non nuls deja traites */
  while (i <= NDG_MAX)
  {
    while ((i <= NDG_MAX) && (histo[i] == 0)) i++; /* saute les niveaux nuls */
    if ((double)k >= np) { j = i; np += p; }
    tabcor[i] = j;
    i++;
    k++;
  }

  free(histo);

  for (x = 0; x < N; x++) SOURCE[x] = tabcor[SOURCE[x]];
  return(1);
}
/* -library_code_end */
