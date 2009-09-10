/* $Id: lrecons.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* operateur de reconstruction binaire */
/* 
   entrees : img, une image en niveaux de gris
             seg, une image binaire (pre-segmentation topologiquement correcte,
                                     sur-ensemble de la segmentation desiree)
             connex, 4 ou 8
   sortie : seg, la segmentation (binaire) desiree

   methode : 
     - soit ncomp le nombre de composantes connexes de seg
     - on fabrique une image d'etiquettes (entiers longs) 'label' avec:
       . ncomp regions ponctuelles, numerotees de 1 a ncomp, marquant un minimum (pour img)
         de chaque composante connexe de seg,
       . 0 pour les autres pixels des composantes connexes de seg,
       . des regions ponctuelles, marquees par des valeurs > ncomp, pour les pixels 
         du complementaire de seg.
     - on appelle fusreg avec img et label
     - on recupere les regions correspondant aux n marqueurs compris entre 1 et ncomp
*/
/* Michel Couprie -  avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <lfusreg.h>
#include <lpropgeo.h>
#include <lrecons.h>

/*
#define VERBOSE
#define DEBUG
*/

#define VAL_SEG 1
#define VAL_MARK 2
#define VAL_MIN 3
#define VAL_FINALE 255

/* ==================================== */
int32_t lrecons(
        struct xvimage *img,    /* image de depart en ndg */
        struct xvimage *seg,    /* image binaire (segmentation max) et resultat */
        int32_t connex,             /* 4, 8  */
        int32_t memoire             /* pour la fusion de regions */
        )
/* ==================================== */
{
  int32_t i, j, h, k, l;
  int32_t w, x, y, z;
  uint8_t *I = UCHARDATA(img);
  uint8_t *S = UCHARDATA(seg);
  struct xvimage * labels;
  uint32_t * LABELS;  
  struct xvimage * resfus;
  uint32_t * RESFUS;  
  uint32_t label;  
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  int32_t ncomp;
  Lifo * LIFO;
  int32_t incr_vois;
  int32_t xMin;

  if (d != 1) 
  {
    fprintf(stderr, "lrecons: cette version ne traite pas les images volumiques\n");
    exit(0);
  }
  if ((rowsize(seg) != rs)  || (colsize(seg) != cs)  || (depth(seg) != d))
  {
    fprintf(stderr, "lrecons: dimensions images incompatibles\n");
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;          /* ne marche que pour 4 */
    default: 
      fprintf(stderr, "lrecons: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lrecons() : CreeLifoVide failed\n");
      return(0);
  }

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "lrecons: allocimage failed\n");
    return 0;
  }
  LABELS = SLONGDATA(labels);
  memset(LABELS, 0, N * sizeof(int32_t));

  resfus = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (resfus == NULL)
  {   
    fprintf(stderr, "lrecons: allocimage failed\n");
    return 0;
  }
  RESFUS = SLONGDATA(resfus);

  for (x = 0; x < N; x++) if (S[x]) S[x] = VAL_SEG;

  /* ============================================================== */
  /* trouve un point xMin de ndg min dans chaque composante de seg
         qui servira de marqueur objet (on le marque VAL_MIN dans seg) */
  /* ============================================================== */

  label = 0;
  for (x = 0; x < N; x++)
  {
    if (S[x] == VAL_SEG)
    {
      label++;
#ifdef DEBUG
printf("point %d %d : nouvelle composante %d\n", x%rs, x/rs, label);
#endif
      xMin = x;
      S[x] = VAL_MARK;
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        z = LifoPop(LIFO);
        if (I[z] < I[xMin]) xMin = z;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(z, k, rs, N);
          if ((y != -1) && (S[y] == VAL_SEG))
	  {
            S[y] = VAL_MARK;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */
      LABELS[xMin] = label;
      S[xMin] = VAL_MIN;
    } /* if (S[x] == VAL_SEG) */
  } /* for (x = 0; x < N; x++) */
  ncomp = label;

#ifdef DEBUG
printf("ncomp = %d\n", ncomp);
#endif

  /* ============================================================== */
  /* etiquette les pixels du complementaire de seg */
  /* ============================================================== */

  for (x = 0; x < N; x++) if (!S[x]) LABELS[x] = ++label;

  /* ============================================================== */
  /* effectue la fusion de regions */
  /* ============================================================== */

  if (! lfusreg(img, labels, resfus, memoire))
  {
    fprintf(stderr, "lrecons: lfusreg failed\n");
    exit(0);
  }

  /* ============================================================== */
  /* segmentation finale */
  /* ============================================================== */

  for (x = 0; x < N; x++)
  {
    if (S[x] == VAL_MIN)
    {
      label = RESFUS[x];
      RESFUS[x] = 0;
      S[x] = VAL_FINALE;
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        z = LifoPop(LIFO);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(z, k, rs, N);
          if ((y != -1) && (RESFUS[y] == label))
	  {
            S[y] = VAL_FINALE;
            RESFUS[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */
    } /* if (S[x] == VAL_SEG) */
  } /* for (x = 0; x < N; x++) */

  /* menage */
  /* ---------------------------------------------------------- */

  LifoTermine(LIFO);
  freeimage(labels);
  freeimage(resfus);
  return(1);
}
