/* $Id: lmoments.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <lmoments.h>
#include <llabelextrema.h>

/****************************************************************
*
* Routine Name: lmoments - library call for moments
*
* Purpose: calcul des indicateurs de forme bases sur les moments
* Input:
* Output:
* Written By: Michel Couprie - novembre 1998
*
* Update april 1999 : version lmomentslab pour une image de labels
*
****************************************************************/

/*
#define DEBUG
#define VERBOSE
#define DEBUGEXCEN
#define DEBUGORIEN
*/

#define EPSILON 1E-6

/* ==================================== */
int32_t limagemoments(
        struct xvimage *img,    /* image de depart */
        Indicateur * indic)     /* resultat */
/* ==================================== */
{
  int32_t i, x, y, z;
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  int32_t area;
  double mx1, my1, mxy1, mx2, my2, mxy2;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  double xx, yy, xc, yc, a;
  int32_t sign;
  double theta;
  double gdiam;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "lmoments: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  area = 0; mx1 = my1 = mxy1 = mx2 = my2 = mxy2 = 0.0;
  for (i = 0; i < N; i++)
  {
     area++; mx1 += i%rs; my1 += i/rs; mxy1 += ((i%rs) * (i/rs));
     mxy2 += (i%rs) * (i/rs); mx2 += (i%rs) * (i%rs); my2 += (i/rs) * (i/rs);

     indic->area = area;
     xc = indic->xcentre = mx1/area;
     yc = indic->ycentre = my1/area;

     /* moments centres d'ordre 1 */
     Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
     /* moments centres d'ordre 2 */
     Mx2 = mx2 - mx1 * mx1 / area;  
     My2 = my2 - my1 * my1 / area;  
     Mxy2 = mxy2 - mx1 * my1 / area;

     /* calcul des longueurs des demi-axes */
     delta = (Mx2 - My2) * (Mx2 - My2) + 4 * Mxy2 * Mxy2;
     lambda1 = sqrt(2 * (Mx2 + My2 + sqrt(delta)) / area);
     lambda2 = sqrt(2 * (Mx2 + My2 - sqrt(delta)) / area);
#ifdef DEBUG
printf("Mx2 = %g ; My2 = %g ; Mxy2 = %g ; delta = %g\n", Mx2, My2, Mxy2, delta);
printf("lambda1 = %g ; lambda2 = %g\n", lambda1, lambda2);
#endif
     if (lambda2 < 0.0)
     {
       fprintf(stderr, "lmoments : valeur propre negative : %g\n", lambda2);
       return 0;
     }

     if (abs(Mx2 - My2) < EPSILON) 
       theta = 0.0;
     else
     {
       double mi1, mi2, theta2;
       theta = 0.5 * atan( 2 * Mxy1 / (Mx2 - My2));
       /* la direction est determinee a pi/2 pres */
       /* on calcule les moments d'inertie pour theta et theta + pi/2 */
       /* pour lever l'ambiguite */
       mi1 = cos(theta) * cos(theta) * My2 + 
             sin(theta) * sin(theta) * Mx2 -
             2 *sin(theta) * cos(theta) * Mxy1;
       theta2 = theta + (M_PI / 2);
       mi2 = cos(theta2) * cos(theta2) * My2 + 
             sin(theta2) * sin(theta2) * Mx2 - 
             2 * sin(theta2) * cos(theta2) * Mxy1;
#ifdef DEBUG
printf("mi1 = %g ; mi2 = %g ; theta1 = %g ; theta2 = %g\n", 
        mi1, mi2, theta * 180 / M_PI, theta2 * 180 / M_PI);
#endif
       if (mi2 < mi1) theta = theta2;
     }
  } //   for (i = 0; i < N; i++)
  indic->gaxe = lambda1;
  indic->excen = lambda2 / lambda1;
  indic->orien = theta;

  gdiam = 0.0; /* calcule le plus grand demi-diametre */
  for (i = 0; i < N; i++)
  {
    xx = (double)(i%rs);
    yy = (double)(i/rs);
    theta = (xc - xx)*(xc - xx) + (yc - yy)*(yc - yy);
    if (theta > gdiam) gdiam = theta;
  } // for (i = 0; i < N; i++)
  indic->gdiam = sqrt(gdiam);

  return(1);
} // limagemoments

/* ==================================== */
int32_t lmoments(
        struct xvimage *img,    /* image de depart */
        int32_t connex,             /* 4, 8  */
        int32_t minimum,            /* booleen */
        Indicateur ** tabindic, /* tableau des resultats */
        int32_t *nlabels)           /* resultat: nombre d'extrema traites */
/* ==================================== */
{
  int32_t k, l;
  int32_t w, x, y, z;
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mxy1, mx2, my2, mxy2;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  int32_t sign;
  double theta;
  double gdiam;
  double gaxe;
  Indicateur * tab;
  int32_t n;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "lmoments: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "lmoments: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "lmoments: allocimage failed\n");
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, minimum, labels, &n))
  {
    fprintf(stderr, "lmoments: llabelextrema failed\n");
    return 0;
  }

  tab = (Indicateur *)calloc(1,(n - 1) * sizeof(Indicateur));
  if (tab == NULL)
  {
    fprintf(stderr, "lmoments: malloc failed\n");
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lattribute() : CreeLifoVide failed\n");
      return(0);
  }

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("\npoint %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mxy1 = mx2 = my2 = mxy2 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += ((w%rs) * (w/rs));
        mxy2 += (w%rs) * (w/rs); mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == label))
	  {
            LABEL[y] = n+1;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      tab[label-1].area = area;
      xc = tab[label-1].xcentre = mx1/area;
      yc = tab[label-1].ycentre = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy2 - mx1 * my1 / area;

      /* calcul des longueurs des demi-axes */
      delta = (Mx2 - My2) * (Mx2 - My2) + 4 * Mxy2 * Mxy2;
      lambda1 = sqrt(2 * (Mx2 + My2 + sqrt(delta)) / area);
      lambda2 = sqrt(2 * (Mx2 + My2 - sqrt(delta)) / area);
#ifdef DEBUG
printf("Mx2 = %g ; My2 = %g ; Mxy2 = %g ; delta = %g\n", Mx2, My2, Mxy2, delta);
printf("lambda1 = %g ; lambda2 = %g\n", lambda1, lambda2);
#endif
      if (lambda2 < 0.0)
      {
        fprintf(stderr, "lmoments : valeur propre negative : %g\n", lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < EPSILON) 
        theta = 0.0;
      else
      {
        double mi1, mi2, theta2;
        theta = 0.5 * atan( 2 * Mxy1 / (Mx2 - My2));
        /* la direction est determinee a pi/2 pres */
        /* on calcule les moments d'inertie pour theta et theta + pi/2 */
        /* pour lever l'ambiguite */
        mi1 = cos(theta) * cos(theta) * My2 + 
              sin(theta) * sin(theta) * Mx2 -
              2 *sin(theta) * cos(theta) * Mxy1;
        theta2 = theta + (M_PI / 2);
        mi2 = cos(theta2) * cos(theta2) * My2 + 
              sin(theta2) * sin(theta2) * Mx2 - 
              2 * sin(theta2) * cos(theta2) * Mxy1;
#ifdef DEBUG
printf("mi1 = %g ; mi2 = %g ; theta1 = %g ; theta2 = %g\n", 
        mi1, mi2, theta * 180 / M_PI, theta2 * 180 / M_PI);
#endif
        if (mi2 < mi1) theta = theta2;
      }

      tab[label-1].gaxe = lambda1;
      tab[label-1].excen = lambda2 / lambda1;
      tab[label-1].orien = theta;

      gdiam = 0.0;
      LABEL[x] = 0;
      LifoPush(LIFO, x);         /* on re-parcourt la composante en demarquant */      
      while (! LifoVide(LIFO))   /* et on calcule le plus grand demi-diametre */
      {
        w = LifoPop(LIFO);
        xx = (double)(w%rs);
        yy = (double)(w/rs);
        theta = (xc - xx)*(xc - xx) + (yc - yy)*(yc - yy);
        if (theta > gdiam) gdiam = theta;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n + 1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */          
      } /* while (! LifoVide(LIFO)) */
      tab[label-1].gdiam = sqrt(gdiam);
    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  *nlabels = n - 1; /* designe le nombre de composantes (etiquettes non nulles) */
  *tabindic = tab;

  LifoTermine(LIFO);

  freeimage(labels);

  return(1);
} // lmoments

/* ==================================== */
int32_t lmomentslab(
        struct xvimage *labels, /* image de depart (cc etiquetees) */
        int32_t connex,             /* 4, 8  */
        int32_t nlabels,            /* nombre de composantes */
        Indicateur ** tabindic) /* tableau des resultats */
/* ==================================== */
{
  int32_t k, l;
  int32_t w, x, y, z;
  int32_t rs = rowsize(labels);
  int32_t cs = colsize(labels);
  int32_t d = depth(labels);
  int32_t N = rs * cs;          /* taille image */
  uint32_t *LABEL = ULONGDATA(labels);
  uint32_t label;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mxy1, mx2, my2, mxy2;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  int32_t sign;
  double theta;
  double gdiam;
  double gaxe;
  Indicateur * tab;

  if (d != 1) 
  {
    fprintf(stderr, "lmomentslab: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "lmomentslab: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

  tab = (Indicateur *)calloc(1,nlabels * sizeof(Indicateur));
  if (tab == NULL)
  {
    fprintf(stderr, "lmomentslab: malloc failed\n");
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lattribute() : CreeLifoVide failed\n");
      return(0);
  }

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("\npoint %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = nlabels+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mxy1 = mx2 = my2 = mxy2 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += ((w%rs) * (w/rs));
        mxy2 += (w%rs) * (w/rs); mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == label))
	  {
            LABEL[y] = nlabels+1;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      tab[label-1].area = area;
      xc = tab[label-1].xcentre = mx1/area;
      yc = tab[label-1].ycentre = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy2 - mx1 * my1 / area;

      /* calcul des longueurs des demi-axes */
      delta = (Mx2 - My2) * (Mx2 - My2) + 4 * Mxy2 * Mxy2;
      lambda1 = sqrt(2 * (Mx2 + My2 + sqrt(delta)) / area);
      lambda2 = sqrt(2 * (Mx2 + My2 - sqrt(delta)) / area);
#ifdef DEBUG
printf("Mx2 = %g ; My2 = %g ; Mxy2 = %g ; delta = %g\n", Mx2, My2, Mxy2, delta);
printf("lambda1 = %g ; lambda2 = %g\n", lambda1, lambda2);
#endif
      if (lambda2 < 0.0)
      {
        fprintf(stderr, "lmomentslab : valeur propre negative : %g\n", lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < EPSILON) 
        theta = 0.0;
      else
      {
        double mi1, mi2, theta2;
        theta = 0.5 * atan( 2 * Mxy1 / (Mx2 - My2));
        /* la direction est determinee a pi/2 pres */
        /* on calcule les moment d'inertie pour theta et theta + pi/2 */
        /* pour lever l'ambiguite */
        mi1 = cos(theta) * cos(theta) * My2 + 
              sin(theta) * sin(theta) * Mx2 -
              2 *sin(theta) * cos(theta) * Mxy1;
        theta2 = theta + (M_PI / 2);
        mi2 = cos(theta2) * cos(theta2) * My2 + 
              sin(theta2) * sin(theta2) * Mx2 - 
              2 * sin(theta2) * cos(theta2) * Mxy1;
#ifdef DEBUG
printf("mi1 = %g ; mi2 = %g ; theta1 = %g ; theta2 = %g\n", 
        mi1, mi2, theta * 180 / M_PI, theta2 * 180 / M_PI);
#endif
        if (mi2 < mi1) theta = theta2;
      }

      tab[label-1].gaxe = lambda1;
      tab[label-1].excen = lambda2 / lambda1;
      tab[label-1].orien = theta;

      gdiam = 0.0;
      LABEL[x] = 0;
      LifoPush(LIFO, x);         /* on re-parcourt la composante en demarquant */      
      while (! LifoVide(LIFO))   /* et on calcule le plus grand demi-diametre */
      {
        w = LifoPop(LIFO);
        xx = (double)(w%rs);
        yy = (double)(w/rs);
        theta = (xc - xx)*(xc - xx) + (yc - yy)*(yc - yy);
        if (theta > gdiam) gdiam = theta;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == nlabels + 1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */          
      } /* while (! LifoVide(LIFO)) */
      tab[label-1].gdiam = sqrt(gdiam);
    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  *tabindic = tab;

  LifoTermine(LIFO);

  return(1);
} // lmomentslab

/* ==================================== */
void PrintIndicateur(Indicateur I)
/* ==================================== */
{
  printf("area = %d \t", I.area);
  printf("centre = (%d,%d) \t", (int32_t)I.xcentre, (int32_t)I.ycentre);
  printf("excen = %g \t", I.excen);
  printf("orien = %g \t", I.orien);
  printf("gdiam = %g \t", I.gdiam);
  printf("\n");
}
/* -library_code_end */
