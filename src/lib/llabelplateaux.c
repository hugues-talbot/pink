/* $Id: llabelplateaux.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mclifo.h>
#ifdef PERF
#include <mcchrono.h>
#endif
#include <mcutil.h>
#include <llabelplateaux.h>

/****************************************************************
*
* Routine Name: llabelplateaux - library call for labelplateaux
*
* Purpose: etiquetage des regions connexes d'altitude constante
  d'une image par des labels differents
*
* Input:
* Output:
* Written By: Michel Couprie
*
****************************************************************/
/*
nlabels = 0
pour tout pixel x dans l'ordre raster faire
  si LABEL[x] == 0 alors
    nlabels += 1
    LABEL[x] = nlabels      
    LifoPush(LIFO, x)
    // propage le label dans la region connexe a altitude constante
    tq non LifoVide(LIFO) faire
      w = LifoPop(LIFO)
      pour tout y 4-voisin de w tq SOURCE[w] == SOURCE[y] et LABEL[y] == 0
      faire
        LABEL[y] = nlabels      
        LifoPush(LIFO, y)    
      fin pour
    fin tq    
  finsi
fin pour
*/

/* ==================================== */
int32_t llabelplateaux(
        struct xvimage *img,
        int32_t connex,
        struct xvimage *lab,
        int32_t *nlabels)
/* ==================================== */
{
  int32_t k;
  int32_t w;
  int32_t x;
  int32_t y;
  uint8_t *F = UCHARDATA(img);
  uint32_t *FL = ULONGDATA(img);
  uint32_t *LABEL =  ULONGDATA(lab);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t n = rs * cs;          /* taille plan */
  int32_t N = n * d;            /* taille image */
  Lifo * LIFO;
#ifdef PERF
  chrono chrono1;
#endif

#ifdef PERF
  /* pour l'analyse de performances */
  start_chrono(&chrono1);
#endif


  if (datatype(lab) != VFF_TYP_4_BYTE) 
  {
    fprintf(stderr, "llabelplateaux: le resultat doit etre de type VFF_TYP_4_BYTE\n");
    return 0;
  }

  if ((rowsize(lab) != rs) || (colsize(lab) != cs) || (depth(lab) != d))
  {
    fprintf(stderr, "llabelplateaux: incompatible image sizes\n");
    return 0;
  }

  /* le LABEL initialement est mis a 0 (minimum) */
  for (x = 0; x < N; x++) LABEL[x] = 0;

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "llabelplateaux() : CreeLifoVide failed\n");
      return(0);
  }

  *nlabels = 0;

if (datatype(img) == VFF_TYP_1_BYTE) 
{
  for (x = 0; x < N; x++)
  {
    if (LABEL[x] == 0)
    {
      *nlabels += 1;
      LABEL[x] = *nlabels;
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        w = LifoPop(LIFO);
        switch (connex)
        {
	  case 4:
            for (k = 0; k < 8; k += 2) /* parcourt les voisins en 4-connexite */
            {
              y = voisin(w, k, rs, N);
              if ((y != -1) && (LABEL[y] == 0) && (F[y] == F[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 6:
            for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
            {
              y = voisin6(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (F[y] == F[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 8:
            for (k = 0; k < 8; k += 1) /* parcourt les voisins en 8-connexite */
            {
              y = voisin(w, k, rs, N);
              if ((y != -1) && (LABEL[y] == 0) && (F[y] == F[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 18:
            for (k = 0; k < 18; k += 1) /* parcourt les 18 voisins */
            {
              y = voisin18(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (F[y] == F[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 26:
            for (k = 0; k < 26; k += 1) /* parcourt les 26 voisins */
            {
              y = voisin26(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (F[y] == F[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	} /* switch (connex) */
      } /* while (! LifoVide(LIFO)) */
    } /* if (LABEL[x] == 0) */
  } /* for (x = 0; x < N; x++) */
}
else if (datatype(img) == VFF_TYP_4_BYTE) 
{
  for (x = 0; x < N; x++)
  {
    if (LABEL[x] == 0)
    {
      *nlabels += 1;
      LABEL[x] = *nlabels;
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        w = LifoPop(LIFO);
        switch (connex)
        {
	  case 4:
            for (k = 0; k < 8; k += 2) /* parcourt les voisins en 4-connexite */
            {
              y = voisin(w, k, rs, N);
              if ((y != -1) && (LABEL[y] == 0) && (FL[y] == FL[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 6:
            for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
            {
              y = voisin6(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (FL[y] == FL[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 8:
            for (k = 0; k < 8; k += 1) /* parcourt les voisins en 8-connexite */
            {
              y = voisin(w, k, rs, N);
              if ((y != -1) && (LABEL[y] == 0) && (FL[y] == FL[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 18:
            for (k = 0; k < 18; k += 1) /* parcourt les 18 voisins */
            {
              y = voisin18(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (FL[y] == FL[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	  case 26:
            for (k = 0; k < 26; k += 1) /* parcourt les 26 voisins */
            {
              y = voisin26(w, k, rs, n, N);
              if ((y != -1) && (LABEL[y] == 0) && (FL[y] == FL[w]))
              {
                LABEL[y] = *nlabels;
                LifoPush(LIFO, y);
              } /* if y ... */
            } /* for k ... */
            break;

	} /* switch (connex) */
      } /* while (! LifoVide(LIFO)) */
    } /* if (LABEL[x] == 0) */
  } /* for (x = 0; x < N; x++) */
}
else
{
  fprintf(stderr, "llabelplateaux: bad data type\n");
  return 0;
}


  LifoTermine(LIFO);

#ifdef PERF
  /* pour l'analyse de performances */
  save_time(N, read_chrono(&chrono1), "llabelplateaux", img->name);    
#endif
  return(1);
}
/* -library_code_end */
