/* $Id: lderiche3d.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/****************************************************************
*
* Routine Name: lderiche3d - library call for deriche3d
*
* Purpose:     Filtre lineaire general recursif de Deriche en 3D
*
* Input:       Image en niveau de gris 3D
* Output:      Image en niveau de gris 3D
* Written By:  Michel Couprie - mars 2000
*
****************************************************************/

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
#include <lderiche3d.h>
/*
#define DEBUG
*/

#define EPSILON 1E-20
#define DIRMAX  31

/* ==================================== */
void deriche3dgen(double *f,               /* image a traiter */
                int32_t rs,                  /* taille ligne */
                int32_t cs,                  /* taille colonne */
                int32_t ds,                  /* nombre plans */
                double *g1,              /* zone temporaire de taille max(rs,cs,ds) */
                double *g2,              /* zone temporaire de taille max(rs,cs,ds) */
                double *g,               /* stocke un resultat temporaire, puis le resultat final */ 
                double a1, double a2, double a3, double a4, double b1, double b2,   /* param. dir. z */
                double a5, double a6, double a7, double a8, double b3, double b4,   /* param. dir. y */
                double a9, double a10, double a11, double a12, double b5, double b6 /* param. dir. x */
               )
/* ==================================== */
{
  int32_t x, y, z, ps = rs * cs;

  for (y = 0; y < cs; y++)     /* filtrage dans la direction z (f -> g) */
  for (x = 0; x < rs; x++)
  {
    /* filtre causal en z */
#ifdef BORD_ZERO
    g1[0] = a1 * f[x+rs*y+ps*0];
    g1[1] = a1 * f[x+rs*y+ps*1] + a2 * f[x+rs*y+ps*0] + b1 * g1[0];
#else
    g1[0] = ((a1 + a2) / (1.0 - b1 - b2)) * f[x+rs*y+ps*0];
    g1[1] = a1 * f[x+rs*y+ps*1] + a2 * f[x+rs*y+ps*0] + (b1 + b2) * g1[0];
#endif
    for (z = 2; z < ds; z++)
      g1[z] = a1*f[x+rs*y+ps*z] + a2*f[x+rs*y+ps*(z-1)] + b1*g1[z-1] + b2*g1[z-2];

    /* filtre anticausal en z */
#ifdef BORD_ZERO
    g2[ds-1] = 0;
    g2[ds-2] = a3 * f[x+rs*y+ps*(ds-1)] + b1 * g2[ds-1];
#else
    g2[ds-1] = ((a3 + a4) / (1.0 - b1 - b2)) * f[x+rs*y+ps*(ds-1)];
    g2[ds-2] = (a3 + a4) * f[x+rs*y+ps*(ds-1)] + (b1 + b2) * g2[ds-1];
#endif
    for (z = ds-3; z >= 0; z--)
      g2[z] = a3*f[x+rs*y+ps*(z+1)] + a4*f[x+rs*y+ps*(z+2)] + b1*g2[z+1] + b2*g2[z+2];

    for (z = 0; z < ds; z++)
      g[x+rs*y+ps*z] = g1[z] + g2[z];
  }

  for (z = 0; z < ds; z++)     /* filtrage dans la direction y (g -> g) */
  for (x = 0; x < rs; x++)
  {
    /* filtre causal en y */
#ifdef BORD_ZERO
    g1[0] = a5 * g[x+rs*0+ps*z];
    g1[1] = a5 * g[x+rs*1+ps*z] + a6 * g[x+rs*0+ps*z] + b3 * g1[0];
#else
    g1[0] = ((a5 + a6) / (1.0 - b3 - b4)) * g[x+rs*0+ps*z];
    g1[1] = a5 * g[x+rs*1+ps*z] + a6 * g[x+rs*0+ps*z] + (b3 + b4) * g1[0];
#endif
    for (y = 2; y < cs; y++)
      g1[y] = a5*g[x+rs*y+ps*z] + a6*g[x+rs*(y-1)+ps*z] + b3*g1[y-1] + b4*g1[y-2];

    /* filtre anticausal en y */
#ifdef BORD_ZERO
    g2[cs-1] = 0;
    g2[cs-2] = a7 * g[x+rs*(cs-1)+ps*z] + b3 * g2[cs-1];
#else
    g2[cs-1] = ((a7 + a8) / (1.0 - b3 - b4)) * g[x+rs*(cs-1)+ps*z];
    g2[cs-2] = (a7 + a8) * g[x+rs*(cs-1)+ps*z] + (b3 + b4) * g2[cs-1];
#endif
    for (y = cs-3; y >= 0; y--)
      g2[y] = a7*g[x+rs*(y+1)+ps*z] + a8*g[x+rs*(y+2)+ps*z] + b3*g2[y+1] + b4*g2[y+2];

    for (y = 0; y < cs; y++)
      g[x+rs*y+ps*z] = g1[y] + g2[y];
  }

  for (z = 0; z < ds; z++)     /* filtrage dans la direction x (g -> g) */
  for (y = 0; y < cs; y++)
  {
    /* filtre causal en x */
#ifdef BORD_ZERO
    g1[0] = a9 * g[0+rs*y+ps*z];
    g1[1] = a9 * g[1+rs*y+ps*z] + a10 * g[0+rs*y+ps*z] + b5 * g1[0];
#else
    g1[0] = ((a9 + a10) / (1.0 - b5 - b6)) * g[0+rs*y+ps*z];
    g1[1] = a9 * g[1+rs*y+ps*z] + a10 * g[0+rs*y+ps*z] + (b5 + b6) * g1[0];
#endif
    for (x = 2; x < rs; x++)
      g1[x] = a9 * g[x+rs*y+ps*z] + a10 * g[x-1+rs*y+ps*z] + b5 * g1[x-1] + b6 * g1[x-2];

    /* filtre anticausal en x */
#ifdef BORD_ZERO
    g2[rs-1] = 0;
    g2[rs-2] = a11 * g[rs-1+rs*y+ps*z] + b5 * g2[rs-1];
#else
    g2[rs-1] = ((a11 + a12) / (1.0 - b5 - b6)) * g[rs-1+rs*y+ps*z];
    g2[rs-2] = (a11 + a12) * g[rs-1+rs*y+ps*z] + (b5 + b6) * g2[rs-1];
#endif
    for (x = rs-3; x >= 0; x--)
      g2[x] = a11 * g[x+1+rs*y+ps*z] + a12 * g[x+2+rs*y+ps*z] + b5 * g2[x+1] + b6 * g2[x+2];

    for (x = 0; x < rs; x++)
      g[x+rs*y+ps*z] = g1[x] + g2[x];
  }

} /* deriche3dgen() */

/* ==================================== */
int32_t lderiche3d(struct xvimage *image, double alpha, int32_t function, double l)
/* ==================================== */
/*
    alpha : parametre (1/taille) du filtre
    function : 0 = module du gradient lisse'
               1 = direction du gradient lisse' 
               2 = laplacien (normalise - 127 represente le 0)
               3 = f - l * laplacien(f)
               4 = lisseur
*/
{ 
  int32_t i;
  uint8_t *ima = UCHARDATA(image);
  int32_t rs = rowsize(image);
  int32_t cs = colsize(image);
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;
  double *Im1;    /* image intermediaire */
  double *Im2;    /* image intermediaire */
  double *Im3;    /* image intermediaire */
  double *Imd;    /* image intermediaire */
  double *buf1;   /* buffer ligne ou colonne */
  double *buf2;   /* buffer ligne ou colonne */
  double k;       /* constante de normalisation pour le lisseur */
  double kp;      /* constante de normalisation pour le derivateur */
  double kpp;     /* constante de normalisation pour le laplacien */
  double e_a;     /* stocke exp(-alpha) */
  double e_2a;    /* stocke exp(-2alpha) */
  double a1, a2, a3, a4, a5, a6, a7, a8;
  double b1, b2, b3, b4;
  double t1, t2, t3;
  double sbuf;

  Im1 = (double *)calloc(1,N * sizeof(double));
  Imd = (double *)calloc(1,N * sizeof(double));
  if ((Im1==NULL) || (Imd==NULL))
  {   fprintf(stderr,"lderiche3d() : malloc failed\n");
      return(0);
  }
  if (function == 0)
  {
    Im2 = (double *)calloc(1,N * sizeof(double));
    Im3 = (double *)calloc(1,N * sizeof(double));
    if ((Im2==NULL) || (Im3==NULL))
    {   fprintf(stderr,"lderiche3d() : malloc failed\n");
        return(0);
    }
  }
  sbuf = max((max(rs,cs)),ds);
  buf1 = (double *)calloc(1,sbuf * sizeof(double));
  buf2 = (double *)calloc(1,sbuf * sizeof(double));
  if ((buf1==NULL) || (buf2==NULL))
  {   fprintf(stderr,"lderiche3d() : malloc failed\n");
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
      a1 = k;                         /* lisseur */
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;

      a5 = 0.0;                       /* derivateur */
      a6 = kp * e_a;
      a7 = - kp * e_a;
      a8 = 0.0;

      b1 = 2 * e_a;
      b2 = - e_2a;

      deriche3dgen(Imd, rs, cs, ds, buf1, buf2, Im1,
                   a5, a6, a7, a8, b1, b2,           /* derive en x */
                   a1, a2, a3, a4, b1, b2,           /* lisse en y */
                   a1, a2, a3, a4, b1, b2);          /* lisse en z */

      deriche3dgen(Imd, rs, cs, ds, buf1, buf2, Im2,
                   a1, a2, a3, a4, b1, b2,           /* lisse en x */
                   a5, a6, a7, a8, b1, b2,           /* derive en y */
                   a1, a2, a3, a4, b1, b2);          /* lisse en z */

      deriche3dgen(Imd, rs, cs, ds, buf1, buf2, Im3,
                   a1, a2, a3, a4, b1, b2,           /* lisse en x */
                   a1, a2, a3, a4, b1, b2,           /* lisse en y */
                   a5, a6, a7, a8, b1, b2);          /* derive en z */

      for (i = 0; i < N; i++)
      {
        t1 = Im1[i];
        t2 = Im2[i];
        t3 = Im3[i];
        t2 = sqrt((t1 * t1) + (t2 * t2) + (t3 * t3));
        if (t2 <= 255.0)
          ima[i] = (uint8_t)t2;
        else
          ima[i] = 255;
      }
      break;

    case 1:  /* direction du gradient lisse' */
        fprintf(stderr, "Not Yet Implemented\n");
        return 0;

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
      fprintf(stderr, "Not Yet Implemented\n");
      /*
      deriche3dgen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      deriche3dgen(Imd, rs, cs, buf1, buf2, Im2,
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
      */
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
      fprintf(stderr, "Not Yet Implemented\n");
      /*
      deriche3dgen(Imd, rs, cs, buf1, buf2, Im1,
                 a1, a2, a3, a4, a5, a6, a7, a8, b1, b2, b3, b4);
      deriche3dgen(Imd, rs, cs, buf1, buf2, Im2,
                 a5, a6, a7, a8, a1, a2, a3, a4, b1, b2, b3, b4);

      for (i = 0; i < N; i++)      
      {
        t1 = -(Im1[i] + Im2[i]);
        t2 = (double)(ima[i]) - l * t1;
        if (t2 < 0.0) t2 = 0.0;
        if (t2 > 255.0) t2 = 255.0;
        ima[i] = (uint8_t)floor(t2);
      }
      */
      break;

    case 4:  /* lisseur */

      a1 = k;
      a2 = k * e_a * (alpha - 1.0);
      a3 = k * e_a * (alpha + 1.0);
      a4 = - k * e_2a;

      b1 = 2 * e_a;
      b2 = - e_2a;

      deriche3dgen(Imd, rs, cs, ds, buf1, buf2, Im1,
                   a1, a2, a3, a4, b1, b2,           /* lisse en x */
                   a1, a2, a3, a4, b1, b2,           /* lisse en y */
                   a1, a2, a3, a4, b1, b2);          /* lisse en z */

      for (i = 0; i < N; i++)      
      {
        t1 =  Im1[i];
        if (t1 < 0.0) t1 = 0.0;
        if (t1 > 255.0) t1 = 255.0;
        ima[i] = (uint8_t)floor(t1);
      }
      break;

      default: 
        fprintf(stderr, "lderiche3d : fonction %d inexistante ; utiliser : \n", function);
        fprintf(stderr, "  0 : module du gradient lisse'\n");
        fprintf(stderr, "  1 : direction du gradient lisse'\n");
        fprintf(stderr, "  2 : laplacien lisse'\n");
        fprintf(stderr, "  3 : f - l * laplacien(f)\n");
        fprintf(stderr, "  4 : lisseur\n");
        return 0;
  } /* switch (function) */


  free(Im1);
  free(Imd);
  if (function == 1)
  {
    free(Im2);
    free(Im3);
  }
  free(buf1);
  free(buf2);
  return 1;
}

/* ==================================== */
void deriche3dgenb(uint8_t *f,     /* image a traiter */
                int32_t rs,                  /* taille ligne */
                int32_t cs,                  /* taille colonne */
                int32_t ds,                  /* nombre plans */
                double *g1,              /* zone temporaire de taille max(rs,cs,ds) */
                double *g2,              /* zone temporaire de taille max(rs,cs,ds) */
                double *g,               /* stocke un resultat temporaire, puis le resultat final */ 
                double a1, double a2, double a3, double a4, double b1, double b2,   /* param. dir. z */
                double a5, double a6, double a7, double a8, double b3, double b4,   /* param. dir. y */
                double a9, double a10, double a11, double a12, double b5, double b6 /* param. dir. x */
               )
/* ==================================== */
//  traite directement une image d'entiers
{
  int32_t x, y, z, ps = rs * cs;

  for (y = 0; y < cs; y++)     /* filtrage dans la direction z (f -> g) */
  for (x = 0; x < rs; x++)
  {
    /* filtre causal en z */
#ifdef BORD_ZERO
    g1[0] = a1 * (double)(f[x+rs*y+ps*0]);
    g1[1] = a1 * (double)(f[x+rs*y+ps*1]) + a2 * (double)(f[x+rs*y+ps*0]) + b1 * g1[0];
#else
    g1[0] = ((a1 + a2) / (1.0 - b1 - b2)) * (double)(f[x+rs*y+ps*0]);
    g1[1] = a1 * (double)(f[x+rs*y+ps*1]) + a2 * (double)(f[x+rs*y+ps*0]) + (b1 + b2) * g1[0];
#endif
    for (z = 2; z < ds; z++)
      g1[z] = a1*(double)(f[x+rs*y+ps*z]) + a2*(double)(f[x+rs*y+ps*(z-1)]) + b1*g1[z-1] + b2*g1[z-2];

    /* filtre anticausal en z */
#ifdef BORD_ZERO
    g2[ds-1] = 0;
    g2[ds-2] = a3 * (double)(f[x+rs*y+ps*(ds-1)]) + b1 * g2[ds-1];
#else
    g2[ds-1] = ((a3 + a4) / (1.0 - b1 - b2)) * (double)(f[x+rs*y+ps*(ds-1)]);
    g2[ds-2] = (a3 + a4) * (double)(f[x+rs*y+ps*(ds-1)]) + (b1 + b2) * g2[ds-1];
#endif
    for (z = ds-3; z >= 0; z--)
      g2[z] = a3*(double)(f[x+rs*y+ps*(z+1)]) + a4*(double)(f[x+rs*y+ps*(z+2)]) + b1*g2[z+1] + b2*g2[z+2];

    for (z = 0; z < ds; z++)
      g[x+rs*y+ps*z] = g1[z] + g2[z];
  }

  for (z = 0; z < ds; z++)     /* filtrage dans la direction y (g -> g) */
  for (x = 0; x < rs; x++)
  {
    /* filtre causal en y */
#ifdef BORD_ZERO
    g1[0] = a5 * g[x+rs*0+ps*z];
    g1[1] = a5 * g[x+rs*1+ps*z] + a6 * g[x+rs*0+ps*z] + b3 * g1[0];
#else
    g1[0] = ((a5 + a6) / (1.0 - b3 - b4)) * g[x+rs*0+ps*z];
    g1[1] = a5 * g[x+rs*1+ps*z] + a6 * g[x+rs*0+ps*z] + (b3 + b4) * g1[0];
#endif
    for (y = 2; y < cs; y++)
      g1[y] = a5*g[x+rs*y+ps*z] + a6*g[x+rs*(y-1)+ps*z] + b3*g1[y-1] + b4*g1[y-2];

    /* filtre anticausal en y */
#ifdef BORD_ZERO
    g2[cs-1] = 0;
    g2[cs-2] = a7 * g[x+rs*(cs-1)+ps*z] + b3 * g2[cs-1];
#else
    g2[cs-1] = ((a7 + a8) / (1.0 - b3 - b4)) * g[x+rs*(cs-1)+ps*z];
    g2[cs-2] = (a7 + a8) * g[x+rs*(cs-1)+ps*z] + (b3 + b4) * g2[cs-1];
#endif
    for (y = cs-3; y >= 0; y--)
      g2[y] = a7*g[x+rs*(y+1)+ps*z] + a8*g[x+rs*(y+2)+ps*z] + b3*g2[y+1] + b4*g2[y+2];

    for (y = 0; y < cs; y++)
      g[x+rs*y+ps*z] = g1[y] + g2[y];
  }

  for (z = 0; z < ds; z++)     /* filtrage dans la direction x (g -> g) */
  for (y = 0; y < cs; y++)
  {
    /* filtre causal en x */
#ifdef BORD_ZERO
    g1[0] = a9 * g[0+rs*y+ps*z];
    g1[1] = a9 * g[1+rs*y+ps*z] + a10 * g[0+rs*y+ps*z] + b5 * g1[0];
#else
    g1[0] = ((a9 + a10) / (1.0 - b5 - b6)) * g[0+rs*y+ps*z];
    g1[1] = a9 * g[1+rs*y+ps*z] + a10 * g[0+rs*y+ps*z] + (b5 + b6) * g1[0];
#endif
    for (x = 2; x < rs; x++)
      g1[x] = a9 * g[x+rs*y+ps*z] + a10 * g[x-1+rs*y+ps*z] + b5 * g1[x-1] + b6 * g1[x-2];

    /* filtre anticausal en x */
#ifdef BORD_ZERO
    g2[rs-1] = 0;
    g2[rs-2] = a11 * g[rs-1+rs*y+ps*z] + b5 * g2[rs-1];
#else
    g2[rs-1] = ((a11 + a12) / (1.0 - b5 - b6)) * g[rs-1+rs*y+ps*z];
    g2[rs-2] = (a11 + a12) * g[rs-1+rs*y+ps*z] + (b5 + b6) * g2[rs-1];
#endif
    for (x = rs-3; x >= 0; x--)
      g2[x] = a11 * g[x+1+rs*y+ps*z] + a12 * g[x+2+rs*y+ps*z] + b5 * g2[x+1] + b6 * g2[x+2];

    for (x = 0; x < rs; x++)
      g[x+rs*y+ps*z] = g1[x] + g2[x];
  }

} /* deriche3dgenb() */

/* ==================================== */
int32_t llisseurrec3d(struct xvimage *image, double alpha)
/* ==================================== */
/*
    alpha : parametre (1/taille) du filtre
*/
{ 
  int32_t i;
  uint8_t *ima = UCHARDATA(image);
  int32_t rs = rowsize(image);
  int32_t cs = colsize(image);
  int32_t ds = depth(image);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;
  double *Im1;    /* image intermediaire */
  double *buf1;   /* buffer ligne ou colonne */
  double *buf2;   /* buffer ligne ou colonne */
  double k;       /* constante de normalisation pour le lisseur */
  double kp;      /* constante de normalisation pour le derivateur */
  double kpp;     /* constante de normalisation pour le laplacien */
  double e_a;     /* stocke exp(-alpha) */
  double e_2a;    /* stocke exp(-2alpha) */
  double a1, a2, a3, a4;
  double b1, b2;
  double t1;

  Im1 = (double *)calloc(1,N * sizeof(double));
  if (Im1==NULL)
  {   fprintf(stderr,"lderiche3d() : malloc failed\n");
      return(0);
  }
  buf1 = (double *)calloc(1,max(max(rs,cs),ds) * sizeof(double));
  buf2 = (double *)calloc(1,max(max(rs,cs),ds) * sizeof(double));
  if ((buf1==NULL) || (buf2==NULL))
  {   fprintf(stderr,"lderiche3d() : malloc failed\n");
      return(0);
  }

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

  a1 = k;
  a2 = k * e_a * (alpha - 1.0);
  a3 = k * e_a * (alpha + 1.0);
  a4 = - k * e_2a;
  b1 = 2 * e_a;
  b2 = - e_2a;

  deriche3dgenb(ima, rs, cs, ds, buf1, buf2, Im1,
                a1, a2, a3, a4, b1, b2,           /* lisse en x */
                a1, a2, a3, a4, b1, b2,           /* lisse en y */
                a1, a2, a3, a4, b1, b2);          /* lisse en z */

  for (i = 0; i < N; i++)      
  {
    t1 =  Im1[i];
    if (t1 < 0.0) t1 = 0.0;
    if (t1 > 255.0) t1 = 255.0;
    ima[i] = (uint8_t)floor(t1);
  }

  free(Im1);
  free(buf1);
  free(buf2);
  return 1;
} /* llisseurrec3d() */

