/* $Id: lrecalagerigide.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* 
  recalage rigide :
  - de contours simples (courbes fermées)
  - d'images numériques

  Michel Couprie - juillet 2002 - avril 2005 - novembre 2008 
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <mcpowell.h>
#include <lrecalagerigide.h>

#define VERBOSE

/* ==================================== */
static double dist(double x1, double y1, double x2, double y2)
/* ==================================== */
{
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

/* ==================================== */
static double distcarre(double x1, double y1, double x2, double y2)
/* ==================================== */
{
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

/* ==================================== */
double Delta(double *X, int32_t n, double *Y, int32_t m)
/* ==================================== */
/*! \fn double Delta(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 2 contenant le premier ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Y (entrée) : matrice m * 2 contenant le second ensemble de points
    \param m (entrée) : nombre de points dans X
    \return distance de X à Y
    \brief calcule une "distance" de X à Y, définie comme la somme des carrés des distances
       des points de X à l'ensemble Y.
*/
{
  double tmin, t, x1, x2, delta = 0.0;
  int32_t i, j;
  for (i = 0; i < n; i++)
  {
    x1 = X[i*2 + 0];
    x2 = X[i*2 + 1];
    tmin = distcarre(x1, x2, Y[0], Y[1]);
    for (j = 1; j < m; j++)
    {
      t = distcarre(x1, x2, Y[j*2 + 0], Y[j*2 + 1]);
      if (t < tmin) tmin = t;
    } // for (j = 1; j < m; j++)
    delta += tmin;
  } // for (i = 0; i < n; i++)
  return delta;
} // Delta()

/* ==================================== */
void Tgamma(double *X, int32_t n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la transformation affine linéaire définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int32_t i, j;
  double Rot[2][2];
  
  // rotation
  Rot[0][0] = cos(Gamma[2]);  
  Rot[0][1] = -sin(Gamma[2]);  
  Rot[1][0] = sin(Gamma[2]);  
  Rot[1][1] = cos(Gamma[2]);  
  lin_mult((double *)X, (double *)Rot, (double *)R, n, 2, 2);
  // homotheties
  for (i = 0; i < n; i++)
  {
    R[i*2 + 0] = R[i*2 + 0] * Gamma[0]; // hx
    R[i*2 + 1] = R[i*2 + 1] * Gamma[1]; // hy
  } // for (i = 0; i < n; i++)
  // translations
  for (i = 0; i < n; i++)
  {
    R[i*2 + 0] = R[i*2 + 0] + Gamma[3]; // tx
    R[i*2 + 1] = R[i*2 + 1] + Gamma[4]; // ty
  } // for (i = 0; i < n; i++)
} // Tgamma() 

/* ==================================== */
double F(double *G, ensemble *ens)
/* ==================================== */
{
  Tgamma(ens->X, ens->n, G, ens->Tmp);
  return Delta(ens->Tmp, ens->n, ens->Y, ens->m);  
} // F()

/* ==================================== */
double *lrecalagerigide2d(double *X, int32_t n, double *Y, int32_t m)
/* ==================================== */
/*! \fn double *lrecalagerigide2d(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée/sortie) : matrice n * 2 contenant le premier ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Y (entrée) : matrice m * 2 contenant le second ensemble de points
    \param m (entrée) : nombre de points dans Y
    \return vecteur des paramètres de la déformation recalant X à Y  
    \brief identifie les paramètres de la déformation affine linéaire recalant au mieux X à Y et
       applique cette déformation à X.
       Cette déformation est la composée (dans cet ordre) d'une homothétie
       dans les directions x,y de facteurs (hx, hy) ; d'une rotation d'angle theta, 
       et d'une translation de vecteur (tx, ty).
       Le résultat de l'application de cette déformation à X est stockée dans X en sortie.
*/
{
  double *Gamma, fmin;
  double G[5] = {1.0, 1.0, 0.0, 0.0, 0.0}; // hx, hy, theta, tx, ty
  int32_t i, ret;
  double BX1, BX2, BY1, BY2; // barycentres
  ensemble ens;
  const int32_t MAXITER = 500;

  // identifie les barycentres et normalise les coordonnées
  BX1 = BX2 = BY1 = BY2 = 0.0;
  for (i = 0; i < n; i++) { BX1 += X[2*i]; BX2 += X[2*i + 1]; }
  for (i = 0; i < m; i++) { BY1 += Y[2*i]; BY2 += Y[2*i + 1]; }
  BX1 /= n; BX2 /= n; BY1 /= m; BY2 /= m;

  for (i = 0; i < n; i++) { X[2*i] -= BX1; X[2*i + 1] -= BX2; }
  for (i = 0; i < n; i++) { Y[2*i] -= BY1; Y[2*i + 1] -= BY2; }

  ens.X = X; ens.n = n; ens.Y = Y; ens.m = m; 
  ens.Tmp = (double *)calloc(1, 2 * max(n,m) * sizeof(double));

  powell(F, &ens, G, 5, 1e-6, 0.1, MAXITER, &fmin);

  G[3] += BY1; G[4] += BY2;
  Tgamma(X, n, G, ens.Tmp);
  G[3] -= BX1; G[4] -= BX2;

  for (i = 0; i < n+n; i++) X[i] = ens.Tmp[i]; 

  Gamma = (double *)calloc(1,5 * sizeof(double));
  memcpy(Gamma, G, 5 * sizeof(double));
  free(ens.Tmp);
  return Gamma;
} // lrecalagerigide2d()

/* ==================================== */
static double dist3d(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
}

/* ==================================== */
static double distcarre3d(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1);
}

/* ==================================== */
double Delta3d(double *X, int32_t n, double *Y, int32_t m)
/* ==================================== */
/*! \fn double Delta3d(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 3 contenant le premier ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Y (entrée) : matrice m * 3 contenant le second ensemble de points
    \param m (entrée) : nombre de points dans X
    \return distance de X à Y
    \brief calcule une "distance" de X à Y, définie comme la somme des carrés des distances
       des points de X à l'ensemble Y.
*/
{
  double tmin, t, x1, x2, x3, delta = 0.0;
  int32_t i, j;
  for (i = 0; i < n; i++)
  {
    x1 = X[i*3 + 0];
    x2 = X[i*3 + 1];
    x3 = X[i*3 + 2];
    tmin = distcarre3d(x1, x2, x3, Y[0], Y[1], Y[2]);
    for (j = 1; j < m; j++)
    {
      t = distcarre3d(x1, x2, x3, Y[j*3 + 0], Y[j*3 + 1], Y[j*3 + 2]);
      if (t < tmin) tmin = t;
    } // for (j = 1; j < m; j++)
    delta += tmin;
  } // for (i = 0; i < n; i++)
  return delta;
} // Delta3d()

/* ==================================== */
void Tgamma3d(double *X, int32_t n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma3d(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la transformation affine linéaire définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int32_t i;
  double Rot1[3][3] = {0,0,0,0,0,0,0,0,0};
  double Rot2[3][3] = {0,0,0,0,0,0,0,0,0};
  double Rot[3][3];

  // rotation autour de l'axe z
  Rot1[0][0] = cos(Gamma[3]);  
  Rot1[0][1] = -sin(Gamma[3]);  
  Rot1[1][0] = sin(Gamma[3]);  
  Rot1[1][1] = cos(Gamma[3]);  
  Rot1[2][2] = 1.0;  
  // rotation autour de l'axe y
  Rot2[0][0] = cos(Gamma[4]);  
  Rot2[0][2] = -sin(Gamma[4]);  
  Rot2[2][0] = sin(Gamma[4]);  
  Rot2[2][2] = cos(Gamma[4]);  
  Rot2[1][1] = 1.0;  
  lin_mult((double *)Rot1, (double *)Rot2, (double *)Rot, 3, 3, 3);
  lin_mult((double *)X, (double *)Rot, (double *)R, n, 3, 3);

  // homotheties
  for (i = 0; i < n; i++)
  {
    R[i*3 + 0] = R[i*3 + 0] * Gamma[0]; // hx
    R[i*3 + 1] = R[i*3 + 1] * Gamma[1]; // hy
    R[i*3 + 2] = R[i*3 + 2] * Gamma[2]; // hz
  } // for (i = 0; i < n; i++)
  // translations
  for (i = 0; i < n; i++)
  {
    R[i*3 + 0] = R[i*3 + 0] + Gamma[5]; // tx
    R[i*3 + 1] = R[i*3 + 1] + Gamma[6]; // ty
    R[i*3 + 2] = R[i*3 + 2] + Gamma[7]; // tz
  } // for (i = 0; i < n; i++)
} // Tgamma3d() 

/* ==================================== */
double F3d(double *G, ensemble *ens)
/* ==================================== */
{
  Tgamma3d(ens->X, ens->n, G, ens->Tmp);
  return Delta3d(ens->Tmp, ens->n, ens->Y, ens->m);  
} // F3d()

/* ==================================== */
double *lrecalagerigide3d(double *X, int32_t n, double *Y, int32_t m)
/* ==================================== */
/*! \fn double *lrecalagerigide3d(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée/sortie) : matrice n * 3 contenant le premier ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Y (entrée) : matrice m * 3 contenant le second ensemble de points
    \param m (entrée) : nombre de points dans Y
    \return vecteur des paramètres de la déformation recalant X à Y  
    \brief identifie les paramètres de la déformation affine linéaire recalant au mieux X à Y et
       applique cette déformation à X.
       Cette déformation est la composée (dans cet ordre) d'une homothétie
       dans les directions x,y,z de facteurs (hx, hy, hz) ; d'une rotation d'angle theta, 
       et d'une translation de vecteur (tx, ty, tz).
       Le résultat de l'application de cette déformation à X est stockée dans X en sortie.
*/
{
  double *Gamma, fmin;
  double G[8] = {1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // hx, hy, hz, theta, phi, tx, ty, tz
  int32_t i, ret;
  double BX1, BX2, BX3, BY1, BY2, BY3; // barycentres
  ensemble ens;
  const int32_t MAXITER = 500;

  // identifie les barycentres et normalise les coordonnées
  BX1 = BX2 = BX3 = BY1 = BY2 = BY3 = 0.0;
  for (i = 0; i < n; i++) { BX1 += X[3*i]; BX2 += X[3*i + 1]; BX3 += X[3*i + 2]; }
  for (i = 0; i < m; i++) { BY1 += Y[3*i]; BY2 += Y[3*i + 1]; BY3 += Y[3*i + 2]; }
  BX1 /= n; BX2 /= n; BX3 /= n; BY1 /= m; BY2 /= m; BY3 /= m;

  for (i = 0; i < n; i++) { X[3*i] -= BX1; X[3*i + 1] -= BX2; X[3*i + 2] -= BX3; }
  for (i = 0; i < m; i++) { Y[3*i] -= BY1; Y[3*i + 1] -= BY2; Y[3*i + 2] -= BY3; }

  ens.X = X; ens.n = n; ens.Y = Y; ens.m = m;
  ens.Tmp = (double *)calloc(1, 3 * max(n,m) * sizeof(double));
  

  powell(F3d, &ens, G, 8, 1e-6, 0.1, MAXITER, &fmin);

  G[5] += BY1; G[6] += BY2; G[7] += BY3;
  Tgamma3d(X, n, G, ens.Tmp);
  G[5] -= BX1; G[6] -= BX2; G[7] -= BX3;

  for (i = 0; i < 3*n; i++) X[i] = ens.Tmp[i]; 

  Gamma = (double *)calloc(1, 8 * sizeof(double));
  memcpy(Gamma, G, 8 * sizeof(double));
  free(ens.Tmp);
  return Gamma;
} // lrecalagerigide3d()

#ifdef NUM
/* ============================================================= */
/* ============================================================= */
/* NUM */
/* ============================================================= */
/* ============================================================= */

/* ==================================== */
double Delta_num(double *X, int32_t n, double *Y, int32_t m)
/* ==================================== */
/*! \fn double Delta_num(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 2 contenant le premier ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Y (entrée) : matrice m * 2 contenant le second ensemble de points
    \param m (entrée) : nombre de points dans X
    \return distance de X à Y
    \brief calcule une "distance" de X à Y, définie comme la somme des carrés des distances
       des points de X à l'ensemble Y.
*/
{
  double tmin, t, x1, x2, delta = 0.0;
  int32_t i, j;
  for (i = 0; i < n; i++)
  {
    x1 = X[i*2 + 0];
    x2 = X[i*2 + 1];
    tmin = distcarre(x1, x2, Y[0], Y[1]);
    for (j = 1; j < m; j++)
    {
      t = distcarre(x1, x2, Y[j*2 + 0], Y[j*2 + 1]);
      if (t < tmin) tmin = t;
    } // for (j = 1; j < m; j++)
    delta += tmin;
  } // for (i = 0; i < n; i++)
  return delta;
} // Delta_num()

/* ==================================== */
void Tgamma_num(double *X, int32_t n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma_num(double *X, int32_t n, double *Y, int32_t m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la transformation affine linéaire définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int32_t i, j;
  double Rot[2][2];
  
  // rotation
  Rot[0][0] = cos(Gamma[2]);  
  Rot[0][1] = -sin(Gamma[2]);  
  Rot[1][0] = sin(Gamma[2]);  
  Rot[1][1] = cos(Gamma[2]);  
  lin_mult((double *)X, (double *)Rot, (double *)R, n, 2, 2);
  // homotheties
  for (i = 0; i < n; i++)
  {
    R[i*2 + 0] = R[i*2 + 0] * Gamma[0]; // hx
    R[i*2 + 1] = R[i*2 + 1] * Gamma[1]; // hy
  } // for (i = 0; i < n; i++)
  // translations
  for (i = 0; i < n; i++)
  {
    R[i*2 + 0] = R[i*2 + 0] + Gamma[3]; // tx
    R[i*2 + 1] = R[i*2 + 1] + Gamma[4]; // ty
  } // for (i = 0; i < n; i++)
} // Tgamma_num() 

/* ==================================== */
double F_num(double *G, ensemble *ens)
/* ==================================== */
{
  Tgamma_num(ens->X, ens->n, G, ens->Tmp);
  return Delta_num(ens->Tmp, ens->n, ens->Y, ens->m);  
} // F()

/* ==================================== */
double *lrecalagerigide2d_num(struct xvimage * image1, struct xvimage * image2)
/* ==================================== */
/*! \fn double *lrecalagerigide2d_num(struct xvimage * image1, struct xvimage * image2)
    \param image1 (entrée/sortie) : première image
    \param image2 (entrée) : seconde image
    \return vecteur des paramètres de la déformation recalant image1 à image2
    \brief identifie les paramètres de la déformation affine linéaire recalant au mieux 
       image1 à image2 et applique cette déformation à image1.
       Cette déformation est la composée (dans cet ordre) d'une rotation d'angle theta, 
       et d'une translation de vecteur (tx, ty).
       Le résultat de l'application de cette déformation à image1 est stockée dans image1 en sortie.
*/
{
  double *Gamma, fmin;
  double G[5] = {1.0, 1.0, 0.0, 0.0, 0.0}; // hx, hy, theta, tx, ty
  int32_t i, ret;
  const int32_t MAXITER = 500;



  powell(F_num, &ens, G, 5, 1e-6, 0.1, MAXITER, &fmin);

  G[3] += BY1; G[4] += BY2;
  Tgamma(X, n, G, ens.Tmp);
  G[3] -= BX1; G[4] -= BX2;

  for (i = 0; i < n+n; i++) X[i] = ens.Tmp[i]; 

  Gamma = (double *)calloc(1,5 * sizeof(double));
  memcpy(Gamma, G, 5 * sizeof(double));
  free(ens.Tmp);
  return Gamma;
} // lrecalagerigide2d_num()
#endif
