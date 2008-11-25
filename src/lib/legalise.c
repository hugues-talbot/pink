/* $Id: legalise.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* operateur: egalisation d'histogramme */
/*

*/
/* Michel Couprie - septembre 1996 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcfah.h>
#include <legalise.h>

/* ==================================== */
int32_t legalise(struct xvimage *image, int32_t n)
/* n : nombre de niveaux de gris desires */
/* ==================================== */
{
  int32_t i;
  int32_t k;
  int32_t x;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t N = d * rs * cs;         /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  Fah * FAH;                   /* la file d'attente hierarchique */
  double p, np;

  FAH = CreeFahVide(N+1);
  if (FAH == NULL)
  {   fprintf(stderr, "legalise() : CreeFahVide failed\n");
      return(0);
  }

  FahPush(FAH, -1, 0);               /* force la creation du niveau 0 dans la Fah. */
  for (x = 0; x < N; x++) FahPush(FAH, x, SOURCE[x]);
  (void)FahPop(FAH);

  p = (double)N / (double)n;   /* nombre de points par niveau */
  np = p;
  i = 0;                       /* nombre de points traites */
  k = 0;                       /* niveau courant */
  while (! FahVide(FAH))
  {
    x = FahPop(FAH);
    SOURCE[x] = k;
    i++;
    if ((double)i >= np) { np += p; k++; }
  } /* while (! FahVide(FAH)) */

  FahTermine(FAH);
  return(1);
}
/* -library_code_end */
