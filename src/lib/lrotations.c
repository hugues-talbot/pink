/* $Id: lrotations.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/*
   Rotations 

   Michel Couprie  -  mai 2006

   lquasishear
   lrotationRT
   lrotationInter
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lrotations.h>

int32_t HQS_x(int32_t x, int32_t y, int32_t a, int32_t b, int32_t c)
{
  return x + (int32_t)floor((double)(a*y + c) / b);
}

int32_t VQS_y(int32_t x, int32_t y, int32_t a, int32_t b, int32_t c)
{
  return y + (int32_t)floor((double)(a*x + c) / b);
}

/* ==================================== */
int32_t lquasishear(struct xvimage * image, double theta, double xc, double yc)
/* ==================================== */
#undef F_NAME
#define F_NAME "lquasishear"
{
  uint8_t *I1, *I2;
  int32_t rs, cs, n, x, y, xx, yy;
  int32_t i, j, k, a, aa, bb, bb2, omega, omega2;

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s() : 3d not yet implemented\n", F_NAME);
    return(0);
  }
  rs = rowsize(image);
  cs = colsize(image);
  n = rs * cs;
  I1 = UCHARDATA(image);

  I2 = (uint8_t *)malloc(n * sizeof(uint8_t));
  if (I2 == NULL)
  {
    fprintf(stderr, "%s() : malloc failed\n", F_NAME);
    return(0);
  }
  memcpy(I2, I1, n);
  memset(I1, 0, n);

  omega = 32768;
  omega2 = omega / 2;
  a = (int32_t)floor(omega*sin(theta));
  aa = (int32_t)floor(omega*sin(theta/2));
  bb = (int32_t)floor(omega*cos(theta/2));
  bb2 = (int32_t)floor((double)bb / 2);

  for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
    {
      xx = HQS_x(x-floor(xc), y-floor(yc), -aa, bb, bb2);
      yy = y-floor(yc);

      //      xx = xx;
      yy = VQS_y(xx, yy, a, omega, omega2);

      xx = HQS_x(xx, yy, -aa, bb, bb2);
      //      yy = yy;

      xx += floor(xc);
      yy += floor(yc);
      if ((xx >= 0) && (yy >= 0) && (xx < rs) && (yy < cs))
	I1[yy*rs + xx] = I2[y*rs + x];
    } // for x for y

  free(I2);
  return 1;
} // lquasishear()

void QSR(int32_t x, int32_t y, int32_t *xx, int32_t *yy, 
	 double xc, double yc, int32_t omega, int32_t omega2, 
	 int32_t a, int32_t aa, int32_t bb, int32_t bb2)
{
      *xx = HQS_x(x-floor(xc), y-floor(yc), -aa, bb, bb2);
      *yy = y-floor(yc);
      *yy = VQS_y(*xx, *yy, a, omega, omega2);
      *xx = HQS_x(*xx, *yy, -aa, bb, bb2);
      *xx += floor(xc);
      *yy += floor(yc);
}

/* ==================================== */
struct xvimage * lquasishear2(struct xvimage * image, double theta)
// version avec allocation d'une image de taille suffisante
/* ==================================== */
#undef F_NAME
#define F_NAME "lquasishear2"
{
  int32_t rs, cs, n, rs2, cs2, n2, x, y, xx, yy, xmax, xmin, ymax, ymin;
  int32_t i, j, k, a, aa, bb, bb2, omega, omega2;
  struct xvimage *image2;
  uint8_t *I1, *I2;

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s() : 3d not yet implemented\n", F_NAME);
    return(0);
  }
  rs = rowsize(image);
  cs = colsize(image);
  n = rs * cs;
  I1 = UCHARDATA(image);

  omega = 32768;
  omega2 = omega / 2;
  a = (int32_t)floor(omega*sin(theta));
  aa = (int32_t)floor(omega*sin(theta/2));
  bb = (int32_t)floor(omega*cos(theta/2));
  bb2 = (int32_t)floor((double)bb / 2);

  QSR(0, 0, &xx, &yy, 0, 0, omega, omega2, a, aa, bb, bb2);
  xmax = xmin = xx; ymax = ymin = yy;
  QSR(rs-1, 0, &xx, &yy, 0, 0, omega, omega2, a, aa, bb, bb2);
  if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
  if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
  QSR(0, cs-1, &xx, &yy, 0, 0, omega, omega2, a, aa, bb, bb2);
  if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
  if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
  QSR(rs-1, cs-1, &xx, &yy, 0, 0, omega, omega2, a, aa, bb, bb2);
  if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
  if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
  rs2 = xmax - xmin + 1;
  cs2 = ymax - ymin + 1;
  n2 = rs2 * cs2;

  image2 = allocimage(NULL, rs2, cs2, 1, datatype(image));
  if (image2 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  I2 = UCHARDATA(image2);
  memset(I2, 0, n2);

  for (y = 0; y < cs; y++)
    for (x = 0; x < rs; x++)
    {
      xx = HQS_x(x, y, -aa, bb, bb2);
      yy = y;

      //      xx = xx;
      yy = VQS_y(xx, yy, a, omega, omega2);

      xx = HQS_x(xx, yy, -aa, bb, bb2);
      //      yy = yy;

      I2[(yy-ymin)*rs2 + xx-xmin] = I1[y*rs + x];
    } // for x for y

  return image2;
} // lquasishear2()

/* ==================================== */
struct xvimage * lrotationRT(struct xvimage * image, double theta, double xc, double yc, uint8_t resize)
/* ==================================== */
/*
Rotates the input image of the angle theta (in radians) 
around the point (xc,yc).
Method: truncated real rotation.
If the boolean resize is 0, the image size is left unchanged 
(hence parts of image may be lost). 
Otherwise, the resulting image size is computed such that no loss 
of information occurs.
*/
#undef F_NAME
#define F_NAME "lrotationRT"
{
  int32_t rs, cs, n, rs2, cs2, x, y, xx, yy, xmax, xmin, ymax, ymin;
  struct xvimage *image2;
  uint8_t *I1, *I2;
  double cost = cos(theta);
  double sint = sin(theta);

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s() : 3d not yet implemented\n", F_NAME);
    return(0);
  }
  rs = rowsize(image);
  cs = colsize(image);
  n = rs * cs;
  I1 = UCHARDATA(image);

  if (resize)
  {
    ymax = ymin = xmax = xmin = 0;
    xx = (int32_t)floor(cost*(rs-1) - sint*(0));
    yy = (int32_t)floor(sint*(rs-1) + cost*(0));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)floor(cost*(0) - sint*(cs-1));
    yy = (int32_t)floor(sint*(0) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)floor(cost*(rs-1) - sint*(cs-1));
    yy = (int32_t)floor(sint*(rs-1) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;

    xx = (int32_t)ceil(cost*(rs-1) - sint*(0));
    yy = (int32_t)ceil(sint*(rs-1) + cost*(0));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)ceil(cost*(0) - sint*(cs-1));
    yy = (int32_t)ceil(sint*(0) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)ceil(cost*(rs-1) - sint*(cs-1));
    yy = (int32_t)ceil(sint*(rs-1) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;

    rs2 = xmax - xmin + 1;
    cs2 = ymax - ymin + 1;
  }
  else 
  {
    xmax = rs2 = rs;
    ymax = cs2 = cs; 
    xmin = ymin = 0;
  }

  image2 = allocimage(NULL, rs2, cs2, 1, datatype(image));
  if (image2 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  I2 = UCHARDATA(image2);
  razimage(image2);

  for (yy = ymin; yy < ymax; yy++)
    for (xx = xmin; xx < xmax; xx++)
    {
      x = (int32_t)floor( cost*(xx-xc) + sint*(yy-yc) + 0.5 + xc);
      y = (int32_t)floor(-sint*(xx-xc) + cost*(yy-yc) + 0.5 + yc);
      if ((x >= 0) && (y >= 0) && (x < rs) && (y < cs))
	I2[(yy-ymin)*rs2 + xx-xmin] = I1[y*rs + x];
    } // for xx for yy

  return image2;
} // lrotationRT()

/* ==================================== */
struct xvimage * lrotationInter(struct xvimage * image, double theta, double xc, double yc, uint8_t resize)
/* ==================================== */
/*
Rotates the grayscale input image of the angle theta (in radians) 
around the point (xc,yc).
Method: interpolation.
If the boolean resize is 0, the image size is left unchanged 
(hence parts of image may be lost). 
Otherwise, the resulting image size is computed such that no loss 
of information occurs.
*/
#undef F_NAME
#define F_NAME "lrotationInter"
{
  int32_t rs, cs, n, rs2, cs2, xx, yy, xmax, xmin, ymax, ymin;
  int32_t xm, xM, ym, yM;
  struct xvimage *image2;
  uint8_t *I1, *I2;
  double cost = cos(theta);
  double sint = sin(theta);
  double x, y, txm, txM, t;

  if (depth(image) != 1)
  {
    fprintf(stderr, "%s() : 3d not yet implemented\n", F_NAME);
    return(0);
  }
  rs = rowsize(image);
  cs = colsize(image);
  n = rs * cs;
  I1 = UCHARDATA(image);

  if (resize)
  {
    ymax = ymin = xmax = xmin = 0;
    xx = (int32_t)floor(cost*(rs-1) - sint*(0));
    yy = (int32_t)floor(sint*(rs-1) + cost*(0));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)floor(cost*(0) - sint*(cs-1));
    yy = (int32_t)floor(sint*(0) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)floor(cost*(rs-1) - sint*(cs-1));
    yy = (int32_t)floor(sint*(rs-1) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;

    xx = (int32_t)ceil(cost*(rs-1) - sint*(0));
    yy = (int32_t)ceil(sint*(rs-1) + cost*(0));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)ceil(cost*(0) - sint*(cs-1));
    yy = (int32_t)ceil(sint*(0) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;
    xx = (int32_t)ceil(cost*(rs-1) - sint*(cs-1));
    yy = (int32_t)ceil(sint*(rs-1) + cost*(cs-1));
    if (xx > xmax) xmax = xx;  if (xx < xmin) xmin = xx;
    if (yy > ymax) ymax = yy;  if (yy < ymin) ymin = yy;

    rs2 = xmax - xmin + 1;
    cs2 = ymax - ymin + 1;
  }
  else 
  {
    xmax = rs2 = rs;
    ymax = cs2 = cs; 
    xmin = ymin = 0;
  }

  image2 = allocimage(NULL, rs2, cs2, 1, datatype(image));
  if (image2 == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  I2 = UCHARDATA(image2);
  razimage(image2);

  for (yy = ymin; yy < ymax; yy++)
    for (xx = xmin; xx < xmax; xx++)
    {
      x =  cost * (xx-xc) + sint * (yy-yc) + xc;
      y = -sint * (xx-xc) + cost * (yy-yc) + yc;
      xm = (int32_t)floor(x);
      xM = xm + 1;
      ym = (int32_t)floor(y);
      yM = ym + 1;
      if ((xm >= 0) && (ym >= 0) && (xm < rs) && (ym < cs) &&
	  (xM >= 0) && (yM >= 0) && (xM < rs) && (yM < cs))
      {
	txm = I1[ym*rs + xm] * (xM-x) + I1[ym*rs + xM] * (x-xm);
	txM = I1[yM*rs + xm] * (xM-x) + I1[yM*rs + xM] * (x-xm);
	t = txm * (yM - y) + txM * (y - ym); 
	I2[(yy-ymin)*rs2 + xx-xmin] = arrondi(t);
      }
    } // for xx for yy

  return image2;
} // lrotationInter()
