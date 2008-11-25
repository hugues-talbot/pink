/* $Id: lderiche.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/****************************************************************
*
* Routine Name: lderiche - library call for deriche
*
* Purpose:     Filtre lineaire general recursif de Deriche
*
* Input:       Image en niveau de gris
* Output:      Image en niveau de gris
* Written By:  Michel Couprie - janvier 1998
*
****************************************************************/

/* 
le filtre lineaire recursif general de Deriche est defini par :
   x : le signal a filtrer
   y : le signal resultat
   y1, y2, r : resultats intermediaires
   alpha : parametre donnant la "taille" du filtre
   a1..8 : parametres calcules a partir de alpha, differents selon les applications
   b1..4 : parametres calcules a partir de alpha, differents selon les applications

   y1[m,n] = a1 x[m,n] + a2 x[m,n-1] + b1 y1[m,n-1] + b2 y1[m,n-2]
     x[m,-1] = 0
     y1[m,-2] = y1[m,-1] = 0
   y2[m,n] = a3 x[m,n+1] + a4 x[m,n+2] + b1 y2[m,n+1] + b2 y2[m,n+2]
     x[m,N] = x[m,N+1] = 0
     y2[m,N] = y2[m,N+1] = 0
   r[m,n] = y1[m,n] + y2[m,n]

   y1[m,n] = a5 r[m,n] + a6 r[m-1,n] + b3 y1[m-1,n] + b4 y1[m-2,n]
     r[-1,n] = 0
     y1[-2,n] = y1[-1,n] = 0
   y2[m,n] = a7 r[m+1,n] + a8 r[m+2,n] + b3 y2[m+1,n] + b4 y2[m+2,n]
     r[M,n] = r[M+1,n] = 0
     y2[M,n] = y2[M+1,n] = 0
   y[m,n] = y1[m,n] + y2[m,n]
*/

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lderiche.h>
/*
#define DEBUG
*/

#define EPSILON 1E-20
#define DIRMAX  31

/* ==================================== */
void derichegen(double *x,               /* image a traiter */
                int32_t M,                   /* taille ligne */
                int32_t N,                   /* taille colonne */
                double *y1,              /* zone temporaire de la taille d'une colonne */
                double *y2,              /* zone temporaire de la taille d'une ligne */
                double *y,               /* stocke un resultat temporaire, puis le resultat final */ 
                double a1, double a2, double a3, double a4, 
                double a5, double a6, double a7, double a8, 
                double b1, double b2, double b3, double b4)
/* ==================================== */
{
  int32_t n, m;

  for (m = 0; m < M; m++)     /* filtrage vertical sur toutes les colonnes */
  {
    /* filtre causal vertical */
#ifdef BORD_ZERO
    y1[0] = a1 * x[m+M*0];
    y1[1] = a1 * x[m+M*1] + a2 * x[m+M*0] + b1 * y1[0];
#else
    y1[0] = ((a1 + a2) / (1.0 - b1 - b2)) * x[m+M*0];
    y1[1] = a1 * x[m+M*1] + a2 * x[m+M*0] + (b1 + b2) * y1[0];
#endif
    for (n = 2; n < N; n++)
      y1[n] = a1*x[m+M*n] + a2*x[m+M*(n-1)] + b1*y1[n-1] + b2*y1[n-2];

    /* filtre anticausal vertical */
#ifdef BORD_ZERO
    y2[N-1] = 0;
    y2[N-2] = a3 * x[m+M*(N-1)];
#else
    y2[N-1] = ((a3 + a4) / (1.0 - b1 - b2)) * x[m+M*(N-1)];
    y2[N-2] = (a3 + a4) * x[m+M*(N-1)] + (b1 + b2) * y2[N-1];
#endif
    for (n = N-3; n >= 0; n--)
      y2[n] = a3*x[m+M*(n+1)] + a4*x[m+M*(n+2)] + b1*y2[n+1] + b2*y2[n+2];

    for (n = 0; n < N; n++)
      y[m+M*n] = y1[n] + y2[n];
  }

  for (n = 0; n < N; n++)     /* filtrage horizontal sur toutes les lignes */
  {
    /* filtre causal horizontal */
#ifdef BORD_ZERO
    y1[0] = a5 * y[0+M*n];
    y1[1] = a5 * y[1+M*n] + a6 * y[0+M*n] + b3 * y1[0];
#else
    y1[0] = ((a5 + a6) / (1.0 - b3 - b4)) * y[0+M*n];
    y1[1] = a5 * y[1+M*n] + a6 * y[0+M*n] + (b3 + b4) * y1[0];
#endif
    for (m = 2; m < M; m++)
      y1[m] = a5 * y[m+M*n] + a6 * y[m-1+M*n] + b3 * y1[m-1] + b4 * y1[m-2];

    /* filtre anticausal horizontal */
#ifdef BORD_ZERO
    y2[M-1] = 0;
    y2[M-2] = a7 * y[M-1+M*n] + b3 * y2[M-1];
#else
    y2[M-1] = ((a7 + a8) / (1.0 - b3 - b4)) * y[M-1+M*n];
    y2[M-2] = (a7 + a8) * y[M-1+M*n] + (b3 + b4) * y2[M-1];
#endif
    for (m = M-3; m >= 0; m--)
      y2[m] = a7 * y[m+1+M*n] + a8 * y[m+2+M*n] + b3 * y2[m+1] + b4 * y2[m+2];

    for (m = 0; m < M; m++)
      y[m+M*n] = y1[m] + y2[m];
  }

} /* derichegen() */

/* ==================================== */
int32_t lderiche(struct xvimage *image, double alpha, int32_t function, double l)
/* ==================================== */
/*
    alpha : parametre (1/taille) du filtre
    function : 0 = module du gradient lisse'
               1 = direction du gradient lisse'
               2 = laplacien (normalise - 127 represente le 0)
               3 = f - l * laplacien(f)
               4 = lisseur
               5 = module du gradient en x
               6 = module du gradient en y
*/
{ 
  int32_t i, j;
  uint8_t *ima = UCHARDATA(image);
  int32_t rs = image->row_size;
  int32_t cs = image->col_size;
  int32_t N = rs * cs;
  double *Im1;    /* image intermediaire */
  double *Im2;    /* image intermediaire */
  double *Imd;    /* image intermediaire */
  double *buf1;   /* buffer ligne ou colonne */
  double *buf2;   /* buffer ligne ou colonne */
  double k;       /* constante de normalisation pour le lisseur */
  double kp;      /* constante de normalisation pour le derivateur */
  double kpp;     /* constante de normalisation pour le laplacien */
  double e_a;     /* stocke exp(-alpha) */
  double e_2a;    /* stocke exp(-2alpha) */
  double a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4;
  double t1, t2;
  double lmax, lmin;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lderiche: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  Im1 = (double *)calloc(1,N * sizeof(double));
  Im2 = (double *)calloc(1,N * sizeof(double));
  Imd = (double *)calloc(1,N * sizeof(double));
  buf1 = (double *)calloc(1,max(rs, cs) * sizeof(double));
  buf2 = (double *)calloc(1,max(rs, cs) * sizeof(double));
  if ((Im1==NULL) || (Im2==NULL) || (Imd==NULL) || (buf1==NULL) || (buf2==NULL))
  {   fprintf(stderr,"lderiche() : malloc failed\n");
      return(0);
  }

  for (i = 0; i < N; i++) Imd[i] = (double)ima[i];

  e_a = exp(- alpha);
  e_2a = exp(- 2.0 * alpha);
  k = 1.0 - e_a;
  k = k * k / (1.0 + (2 * alpha * e_a) - e_2a);
  kp = 1.0 - e_a;
  kp = - kp * kp / e_a;
  kpp = (1.0 - e_2a) / (2 * alpha * e_a);

#ifdef DEBUG
printf("alpha = %g , e_a = %g , e_2a = %g , k = %g\n", alpha, e_a, e_2a, k);
#endif

  switch (function)
  {
    case 0: /* module du gradient lisse' */

      /* valeurs de parametres pour filtre lisseur-derivateur */
      a1 = k;
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;

      a5 = 0.0;
      a6 = kp * e_a;
      a7 = - kp * e_a;
      a8 = 0.0;

      b1 = b3 = 2 * e_a;
      b2 = b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      derichegen(Imd, rs, cs, buf1, buf2, Im2,
                 a5, a6, a7, a8, a1, a2, a3, a4, b1, b2, b3, b4);

      for (i = 0; i < N; i++)
      {
        t1 = Im1[i];
        t2 = Im2[i];
        t2 = sqrt(((t1 * t1) + (t2 * t2)) / 2);
        if (t2 <= 255.0)
          ima[i] = (uint8_t)t2;
        else
          ima[i] = 255;
      }
      break;

    case 1:  /* direction du gradient lisse' */

      /* valeurs de parametres pour filtre lisseur-derivateur */
      a1 = k;
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;

      a5 = 0.0;
      a6 = kp * e_a;
      a7 = - kp * e_a;
      a8 = 0;

      b1 = b3 = 2 * e_a;
      b2 = b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      derichegen(Imd, rs, cs, buf1, buf2, Im2,
                 a5, a6, a7, a8, a1, a2, a3, a4, b1, b2, b3, b4);


      for (i = 0; i < N; i++)
      {
        t1 = Im1[i];
        t2 = Im2[i];
        if (abs(t1) >= EPSILON)
          ima[i] = (uint8_t)((DIRMAX * (atan(t2/t1) + M_PI_2)) / M_PI);
        else
          ima[i] = DIRMAX;
      }
      break;

    case 2:  /* laplacien lisse' */

      /* valeurs de parametres pour filtre lisseur-laplacien */
      a1 = k;
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;
      a5 = 1.0;
      a6 = - e_a * (kpp * alpha + 1.0);
      a7 = e_a * (1.0 - kpp * alpha);
      a8 = - e_2a;
      b1 = b3 = 2 * e_a;
      b2 = b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      derichegen(Imd, rs, cs, buf1, buf2, Im2,
                 a5, a6, a7, a8, a1, a2, a3, a4, b1, b2, b3, b4);


      lmin = lmax = 0.0;
      for (i = 0; i < N; i++)
      {
        Im1[i] = t2 = - (Im1[i] + Im2[i]);
        if (t2 > lmax) lmax = t2;
        if (t2 < lmin) lmin = t2;
      }        
      lmax = max(lmax, -lmin);
      for (i = 0; i < N; i++)      
        ima[i] = 127 + (uint8_t)(Im1[i] * 128.0 / lmax);

      break;

    case 3:  /* f - l * laplacien(f) */

      /* valeurs de parametres pour filtre lisseur-laplacien */
      a1 = k;
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;

      a5 = 1.0;
      a6 = - e_a * (kpp * alpha + 1.0);
      a7 = e_a * (1.0 - kpp * alpha);
      a8 = - e_2a;

      b1 = b3 = 2 * e_a;
      b2 = b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      derichegen(Imd, rs, cs, buf1, buf2, Im2,
                 a5, a6, a7, a8, a1, a2, a3, a4, b1, b2, b3, b4);

      for (i = 0; i < N; i++)      
      {
        t1 = -(Im1[i] + Im2[i]);
        t2 = (double)(ima[i]) - l * t1;
        if (t2 < 0.0) t2 = 0.0;
        if (t2 > 255.0) t2 = 255.0;
        ima[i] = (uint8_t)floor(t2);
      }

      break;

    case 4:  /* lisseur */

      a5 = a1 = k;
      a6 = a2 = k * e_a * (alpha - 1.0);
      a7 = a3 = k * e_a * (alpha + 1.0);
      a8 = a4 = - k * e_2a;

      b1 = b3 = 2 * e_a;
      b2 = b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);

      for (i = 0; i < N; i++)      
      {
        t1 =  Im1[i];
        if (t1 < 0.0) t1 = 0.0;
        if (t1 > 255.0) t1 = 255.0;
        ima[i] = (uint8_t)floor(t1);
      }

      break;

    case 5: /* module du gradient en x */

      /* valeurs de parametres pour filtre derivateur */
      a1 = 1;
      a2 = 0;
      a3 = 0;
      a4 = 0;

      a5 = 0.0;
      a6 = kp * e_a;
      a7 = - kp * e_a;
      a8 = 0.0;

      b1 = b2 = 0;
      b3 = 2 * e_a;
      b4 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);

      for (i = 0; i < N; i++)
      {
        t1 = Im1[i];
        t2 = abs(t1);
        if (t2 <= 255.0)
          ima[i] = (uint8_t)t2;
        else
          ima[i] = 255;
      }
      break;

    case 6: /* module du gradient en y */

      /* valeurs de parametres pour filtre derivateur */
      a5 = 1;
      a6 = 0;
      a7 = 0;
      a8 = 0;

      a1 = 0.0;
      a2 = kp * e_a;
      a3 = - kp * e_a;
      a4 = 0.0;

      b3 = b4 = 0;
      b1 = 2 * e_a;
      b2 = - e_2a;

      derichegen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);

      for (i = 0; i < N; i++)
      {
        t1 = Im1[i];
        t2 = abs(t1);
        if (t2 <= 255.0)
          ima[i] = (uint8_t)t2;
        else
          ima[i] = 255;
      }
      break;

      default: 
        fprintf(stderr, "lderiche : fonction %d inexistante ; utiliser : \n", function);
        fprintf(stderr, "  0 : module du gradient lisse'\n");
        fprintf(stderr, "  1 : direction du gradient lisse'\n");
        fprintf(stderr, "  2 : laplacien lisse'\n");
        fprintf(stderr, "  3 : f - l * laplacien(f)\n");
        fprintf(stderr, "  4 : lisseur\n");
        return 0;
  } /* switch (function) */


  free(Im1);
  free(Im2);
  free(Imd);
  free(buf1);
  free(buf2);
  return 1;
}

