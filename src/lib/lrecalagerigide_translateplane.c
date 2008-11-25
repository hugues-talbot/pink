/* $Id: lrecalagerigide_translateplane.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* 
  recalage rigide de contours simples (courbes fermees)

  Michel Couprie - juillet 2002 - avril 2005
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
double Delta(double *X, int n, double *Y, int m)
/* ==================================== */
/*! \fn double Delta(double *X, int n, double *Y, int m)
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
  int i, j;
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
void Tgamma(double *X, int n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma(double *X, int n, double *Y, int m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la transformation affine linéaire définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int i, j;
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
double *lrecalagerigide2d(double *X, int n, double *Y, int m)
/* ==================================== */
/*! \fn double *lrecalagerigide2d(double *X, int n, double *Y, int m)
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
  int i, ret;
  double BX1, BX2, BY1, BY2; // barycentres
  ensemble ens;
  const int MAXITER = 500;

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
double Delta3d(double *X, int n, double *Y, int m)
/* ==================================== */
/*! \fn double Delta3d(double *X, int n, double *Y, int m)
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
  int i, j;
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
void Tgamma3d(double *X, int n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma3d(double *X, int n, double *Y, int m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la transformation affine linéaire définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int i;

  // translations
  for (i = 0; i < n; i++)
  {
    R[i*3 + 0] = X[i*3 + 0] + Gamma[0]; // tx
    R[i*3 + 1] = X[i*3 + 1] + Gamma[1]; // ty
    R[i*3 + 2] = X[i*3 + 2] ; // translation en z = 0
  } // for (i = 0; i < n; i++)
} // Tgamma3d() 


/* ==================================== */
void Tgamma3dbis(double *X, int n, double *Gamma, double *R)
/* ==================================== */
/*! \fn double Tgamma3d(double *X, int n, double *Y, int m)
    \param X (entrée) : matrice n * 2 contenant un ensemble de points
    \param n (entrée) : nombre de points dans X
    \param Gamma (entrée) : paramètres d'une transformation affine linéaire
    \param R (sortie) : résultat
    \brief applique la translation définie par Gamma aux points de X
    \warning R doit etre initialise aux memes dimensions que X (n,2)
*/
{
  int i;

  // translations
  for (i = 0; i < n; i++)
  {
    R[i*3 + 0] = X[i*3 + 0] + Gamma[0]; // tx
    R[i*3 + 1] = X[i*3 + 1] + Gamma[1]; // ty
    R[i*3 + 2] = X[i*3 + 2] + Gamma[2] ; 
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
double *lrecalagerigide3d_translateplane(double *X, int n, double *Y, int m)
/* ==================================== */
/*! \fn double *lrecalagerigide3d(double *X, int n, double *Y, int m)
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
  double G[3] = {0.0, 0.0,0.0}; // tx, ty,
  int i, ret;
  double BX1, BX2, BX3, BY1, BY2, BY3; // barycentres
  ensemble ens;
  const int MAXITER = 500;

  // identifie les barycentres et normalise les coordonnées
  BX1 = BX2 = BX3 = BY1 = BY2 = BY3 = 0.0;
  for (i = 0; i < n; i++) { BX1 += X[3*i]; BX2 += X[3*i + 1]; BX3 += X[3*i + 2]; }
  for (i = 0; i < m; i++) { BY1 += Y[3*i]; BY2 += Y[3*i + 1]; BY3 += Y[3*i + 2]; }
  BX1 /= n; BX2 /= n; BX3 /= n; BY1 /= m; BY2 /= m; BY3 /= m;

  printf("BX3 = %lg, BY3 = %lg \n",BX3,BY3 );

  for (i = 0; i < n; i++) { X[3*i] -= BX1; X[3*i + 1] -= BX2; X[3*i + 2] -= BX3;}
  for (i = 0; i < m; i++) { Y[3*i] -= BY1; Y[3*i + 1] -= BY2; Y[3*i + 2] -= BY3;}

  ens.X = X; ens.n = n; ens.Y = Y; ens.m = m;
  ens.Tmp = (double *)calloc(1, 3 * max(n,m) * sizeof(double));
  

  //  powell(F3d, &ens, G, 2, 1e-6, 0.1, MAXITER, &fmin);
  powell(F3d, &ens, G, 2, 1.0, 0.1, MAXITER, &fmin);
  printf("les params : %lf %lf \n", G[0], G[1]);

  G[0] += BY1; G[1] += BY2; G[2] += BY3;
  Tgamma3dbis(X, n, G, ens.Tmp);
  G[0] -= BX1; G[1] -= BX2; 
  printf("les params : %lf %lf \n", G[0], G[1]);
  
  for (i = 0; i < 3*n; i++) X[i] = ens.Tmp[i]; 
  
  Gamma = (double *)calloc(1, 2 * sizeof(double));
  memcpy(Gamma, G, 2 * sizeof(double));
  free(ens.Tmp);
  return Gamma;
} // lrecalagerigide3d()


/* ==================================== */
double *lrecalagerigide3d_translate(double *X, int n, double *Y, int m)
/* ==================================== */
/*! \fn double *lrecalagerigide3d(double *X, int n, double *Y, int m)
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
  double G[3] = {0.0, 0.0,0.0}; // tx, ty,tz
  int i, ret;
  double BX1, BX2, BX3, BY1, BY2, BY3; // barycentres
  ensemble ens;
  const int MAXITER = 500;

  // identifie les barycentres et normalise les coordonnées
  BX1 = BX2 = BX3 = BY1 = BY2 = BY3 = 0.0;
  for (i = 0; i < n; i++) { BX1 += X[3*i]; BX2 += X[3*i + 1]; BX3 += X[3*i + 2]; }
  for (i = 0; i < m; i++) { BY1 += Y[3*i]; BY2 += Y[3*i + 1]; BY3 += Y[3*i + 2]; }
  BX1 /= n; BX2 /= n; BX3 /= n; BY1 /= m; BY2 /= m; BY3 /= m;

  printf("BX3 = %lg, BY3 = %lg \n",BX3,BY3 );

  for (i = 0; i < n; i++) { X[3*i] -= BX1; X[3*i + 1] -= BX2; X[3*i + 2] -= BX3;}
  for (i = 0; i < m; i++) { Y[3*i] -= BY1; Y[3*i + 1] -= BY2; Y[3*i + 2] -= BY3;}

  ens.X = X; ens.n = n; ens.Y = Y; ens.m = m;
  ens.Tmp = (double *)calloc(1, 3 * max(n,m) * sizeof(double));
  

  //  powell(F3d, &ens, G, 2, 1e-6, 0.1, MAXITER, &fmin);
  powell(F3d, &ens, G, 3, 1.0, 0.1, MAXITER, &fmin);
  printf("les params : %lf %lf \n", G[0], G[1]);

  G[0] += BY1; G[1] += BY2; G[2] += BY3;
  Tgamma3dbis(X, n, G, ens.Tmp);
  G[0] -= BX1; G[1] -= BX2; G[2] -= BX3;
  printf("les params : %lf %lf %lf \n", G[0], G[1], G[2]);
  printf("Cout de la transformation : %lf\n", fmin);
  for (i = 0; i < 3*n; i++) X[i] = ens.Tmp[i]; 
  
  Gamma = (double *)calloc(1, 3 * sizeof(double));
  memcpy(Gamma, G, 3 * sizeof(double));
  free(ens.Tmp);
  return Gamma;
} // lrecalagerigide3d_translate()
