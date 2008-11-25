/* $Id: ldraw.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* graphic primitives */
/* Michel Couprie - octobre 2002 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lbresen.h>
#include <ldraw.h>

/* ==================================== */
static double dist3(double x1, double y1, double z1, double x2, double y2, double z2)
/* ==================================== */
{
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) + (z2 - z1) * (z2 - z1));
}

/* ==================================== */
void ldrawline(struct xvimage * image1, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
/* ==================================== */
/* draws a straight line segment between two points */
#undef F_NAME
#define F_NAME "ldrawline"
{
  int32_t i, rs, cs;
  uint8_t *F;

  rs = rowsize(image1);
  cs = colsize(image1);
  F = UCHARDATA(image1);
  
  if ((x1 < 0) || (x1 >= rs) ||  (y1 < 0) || (y1 >= cs) ||
      (x2 < 0) || (x2 >= rs) ||  (y2 < 0) || (y2 >= cs)) 
    return;

  lbresen(F, rs, x1, y1, x2, y2);
} // ldrawline()

/* ==================================== */
void ldrawline3d(struct xvimage * image1, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2)
/* ==================================== */
/* draws a 3D straight line segment between two points */
/* NAIVE ALGORITHM - TO IMPROVE !!!! */

#undef F_NAME
#define F_NAME "ldrawline3d"
{
  int32_t i, rs, cs, ds, ps, x, y, z;
  uint8_t *F;

  double len =  dist3(x1, y1, z1, x2, y2, z2);
  int32_t NBSAMPLES = (int32_t)(10 * len)
;
#ifdef DEBUG_DL3
  printf("%s: %d %d %d   %d %d %d\n", F_NAME, x1, y1, z1, x2, y2, z2);
#endif

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs * cs;
  F = UCHARDATA(image1);
  
  if ((x1<0) || (x1>=rs) || (y1<0) || (y1>=cs) || (z1<0) || (z1>=ds) ||
      (x2<0) || (x2>=rs) || (y2<0) || (y2>=cs) || (z2<0) || (z2>=ds))
    return;
  
  for (i = 0; i <= NBSAMPLES; i++)
  {
    x = x1 + (i * (x2 - x1)) / NBSAMPLES;
    y = y1 + (i * (y2 - y1)) / NBSAMPLES;
    z = z1 + (i * (z2 - z1)) / NBSAMPLES;
    F[z*ps + y*rs + x] = NDG_MAX;
  }
} // ldrawline3d()

/* ==================================== */
void ldrawline2(struct xvimage * image1)
/* ==================================== */
/* draws a straight line segment between the two first points found in image */
#undef F_NAME
#define F_NAME "ldrawline2"
{
  int32_t i, first = -1, last;
  uint8_t *F;
  int32_t rs, cs, N;

  rs = rowsize(image1);
  cs = colsize(image1);
  N = rs * cs;
  F = UCHARDATA(image1);
  
  for (i = 0; i < N; i++)
  {
    if (F[i])
    {
      if (first == -1) first = i;
      else {last = i; break;}
    }
  }
  lbresen(F, rs, first%rs, first/rs, last%rs, last/rs);
} // ldrawline2()

/* =============================================================== */
void ldrawfilledellipse(struct xvimage * image, double R, double S, double T, double U, double V, double Z)
/* =============================================================== */
/*
    \param image (entrée/sortie) : l'image où dessiner l'ellipse 
    \param R,S,T,U,V,Z (entrée) : parametres de l'équation de l'ellipse (Rxx + Syy + 2Txy + 2Ux + 2Vy + Z = 0)
*/
#undef F_NAME
#define F_NAME "ldrawfilledellipse"
{
  int32_t rs, cs, N, i, j;
  uint8_t *F;
  rs = rowsize(image);
  cs = colsize(image);
  N = rs * cs;
  F = UCHARDATA(image);
  
  memset(F, 0, N);
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    if (R*i*i + S*j*j + 2*T*i*j + 2*U*i + 2*V*j + Z <= 0)
      F[j * rs + i] = NDG_MAX;
  }

} // ldrawfilledellipse()

/* ==================================== */
void ldrawcubic1(struct xvimage * image1, double *x, double *y, int32_t nseg, double sx, double sy)
/* ==================================== */
/* draws a cubic line segment */
#undef F_NAME
#define F_NAME "ldrawcubic1"
{
  int32_t i, rs, cs, x1, y1, x2, y2;
  uint8_t *F;
  double X, Y, r = 1.0 / nseg, t = 0.0, t2, t3;

  rs = rowsize(image1);
  cs = colsize(image1);
  F = UCHARDATA(image1);
  
  X = x[0]*sx;
  Y = y[0]*sy;
  x1 = arrondi(X);
  y1 = arrondi(Y);
  for (i = 0; i < nseg; i++)
  {
    t += r; t2 = t*t; t3 = t2 * t;
    X = sx * (x[0] + t*x[1] +  t2*x[2] +  t3*x[3]);
    Y = sy * (y[0] + t*y[1] +  t2*y[2] +  t3*y[3]);
    x2 = arrondi(X);
    y2 = arrondi(Y);
    ldrawline(image1, x1, y1, x2, y2);
    x1 = x2; y1 = y2;
  }
} // ldrawcubic1()

/* ==================================== */
void ldrawcubic2(struct xvimage * image1, double *x, double *y, int32_t nseg, double tmin, double tmax)
/* ==================================== */
/* draws a cubic line segment */
#undef F_NAME
#define F_NAME "ldrawcubic2"
{
  int32_t i, rs, cs, x1, y1, x2, y2;
  uint8_t *F;
  double X, Y, r = (tmax - tmin) / nseg, t = tmin, t2, t3;

  rs = rowsize(image1);
  cs = colsize(image1);
  F = UCHARDATA(image1);
  
  t2 = t*t; t3 = t2 * t;
  X = x[0] + t*x[1] +  t2*x[2] +  t3*x[3];
  Y = y[0] + t*y[1] +  t2*y[2] +  t3*y[3];
  x1 = arrondi(X);
  y1 = arrondi(Y);
  for (i = 0; i < nseg; i++)
  {
    t += r; t2 = t*t; t3 = t2 * t;
    X = x[0] + t*x[1] +  t2*x[2] +  t3*x[3];
    Y = y[0] + t*y[1] +  t2*y[2] +  t3*y[3];
    x2 = arrondi(X);
    y2 = arrondi(Y);
    ldrawline(image1, x1, y1, x2, y2);
    x1 = x2; y1 = y2;
  }
} // ldrawcubic2()

/* ==================================== */
void ldrawcubic3d(struct xvimage * image1, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax)
/* ==================================== */
/* draws a cubic line segment */
#undef F_NAME
#define F_NAME "ldrawcubic3d"
{
  int32_t i, rs, cs, ds, x1, y1, z1, x2, y2, z2;
  uint8_t *F;
  double X, Y, Z, r = (tmax - tmin) / nseg, t = tmin, t2, t3;

#ifdef DEBUG_DC3
  printf("%s: %d %d\n", F_NAME, tmin, tmax);
#endif

  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  F = UCHARDATA(image1);
  
  t2 = t*t; t3 = t2 * t;
  X = x[0] + t*x[1] +  t2*x[2] +  t3*x[3];
  Y = y[0] + t*y[1] +  t2*y[2] +  t3*y[3];
  Z = z[0] + t*z[1] +  t2*z[2] +  t3*z[3];
  x1 = arrondi(X);
  y1 = arrondi(Y);
  z1 = arrondi(Z);
  for (i = 0; i < nseg; i++)
  {
    t += r; t2 = t*t; t3 = t2 * t;
    X = x[0] + t*x[1] +  t2*x[2] +  t3*x[3];
    Y = y[0] + t*y[1] +  t2*y[2] +  t3*y[3];
    Z = z[0] + t*z[1] +  t2*z[2] +  t3*z[3];
    x2 = arrondi(X);
    y2 = arrondi(Y);
    z2 = arrondi(Z);
    ldrawline3d(image1, x1, y1, z1, x2, y2, z2);
    x1 = x2; y1 = y2; z1 = z2;
  }
} // ldrawcubic3d()

/* ==================================== */
void ldrawball(struct xvimage * image1, double r, double xc, double yc, double zc)
/* ==================================== */
/* draws a euclidean ball */
#undef F_NAME
#define F_NAME "ldrawball"
{
  int32_t i, j, k, rs, cs, ds, ps;
  uint8_t *F;
  double r2, x, y, z;
  
  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs*cs;
  F = UCHARDATA(image1);
  
  r2 = r * r;
  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = xc - i; 
    y = yc - j; 
    z = zc - k; 
    if (x * x + y * y + z * z <= r2)
      F[k * ps + j * rs + i] = NDG_MAX;
  }
} // ldrawball()

/* ==================================== */
void ldrawtorus(struct xvimage * image1, double c, double a, double xc, double yc, double zc)
/* ==================================== */
/* draws a torus of equation (c-sqrt((xc-x)^2+(yc-y)^2))^2+(zc-z)^2 <= a^2 */
#undef F_NAME
#define F_NAME "ldrawball"
{
  int32_t i, j, k, rs, cs, ds, ps;
  uint8_t *F;
  double t, a2, x, y, z;
  
  rs = rowsize(image1);
  cs = colsize(image1);
  ds = depth(image1);
  ps = rs*cs;
  F = UCHARDATA(image1);
  
  a2 = a * a;
  for (k = 0; k < ds; k++)
  for (j = 0; j < cs; j++)
  for (i = 0; i < rs; i++)
  {
    x = xc - i; 
    y = yc - j; 
    z = zc - k;
    t = c - sqrt(x*x + y*y);
    if (t*t + z*z <= a2)
      F[k * ps + j * rs + i] = NDG_MAX;
  }
} // ldrawtorus()
