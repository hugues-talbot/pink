/* $Id: lgeo.c,v 1.2 2009-01-06 13:18:15 mcouprie Exp $ */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mcimage.h>
#include <mccodimage.h>
#include <mclifo.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <mctopo.h>
#include <mclin.h>
#include <mcliste.h>
#include <lbresen.h>
#include <llabelextrema.h>
#include <lgeo.h>

#define INCRH 2.0
#define INCRW 2.0
#define INCRX 1.0
#define INCRY 1.0
#define INCRT (M_PI/180.0)
#define INCRR 1.0

#define PRH 0.01
#define PRW 0.01
#define PRX 0.01
#define PRY 0.01
#define PRT 0.001
#define PRR 0.01

#define MAXITER 100

/*
#define DEBUG
*/

/* ==================================== */
int32_t initrectanglearrondi(rectanglearrondi *r)
/* ==================================== */
{
  double l = r->w, L = r->h, theta = r->angle, x = r->centre.x, y = r->centre.y;
  double d, alpha;

  if (L < MCGEO_EPSILON) /* c'est un cercle : on ne remplit que r->c1 */
  {
    r->c1.x0 = x;
    r->c1.y0 = y;
    r->c1.r = l / 2;
    return 1;
  }

  /* else */

  d = sqrt(l*l + L*L) / 2.0;
  alpha = atan(l/L);

#ifdef DEBUG
printf("theta = %g, alpha = %g, x = %g, d = %g, cos = %g\n", theta, alpha, x, d, cos(theta - alpha));
#endif

  r->s1.x1 = x - d * cos(theta - alpha);  r->s1.y1 = y - d * sin(theta - alpha);
  r->s1.x2 = x + d * cos(theta + alpha);  r->s1.y2 = y + d * sin(theta + alpha);
  r->s2.x1 = x + d * cos(theta - alpha);  r->s2.y1 = y + d * sin(theta - alpha);
  r->s2.x2 = x - d * cos(theta + alpha);  r->s2.y2 = y - d * sin(theta + alpha);

#ifdef DEBUG
printf("s1 = (%g,%g),(%g,%g)  ", r->s1.x1, r->s1.y1, r->s1.x2, r->s1.y2);
printf("s2 = (%g,%g),(%g,%g)\n", r->s2.x1, r->s2.y1, r->s2.x2, r->s2.y2);
#endif

  if (equdroite(r->s1.x1, r->s1.y1, r->s1.x2, r->s1.y2, &(r->s1.a), &(r->s1.b), &(r->s1.c)) == 0) return 0;
  if (equdroite(r->s2.x1, r->s2.y1, r->s2.x2, r->s2.y2, &(r->s2.a), &(r->s2.b), &(r->s2.c)) == 0) return 0;

  r->c1.x0 = (r->s1.x2 + r->s2.x1) / 2;  r->c1.y0 = (r->s1.y2 + r->s2.y1) / 2;
  r->c2.x0 = (r->s2.x2 + r->s1.x1) / 2;  r->c2.y0 = (r->s2.y2 + r->s1.y1) / 2;
  r->c1.r = r->c2.r = l / 2;

#ifdef DEBUG
printf("c1 = %g,%g,%g  ", r->c1.x0, r->c1.y0, r->c1.r);
printf("c2 = %g,%g,%g\n", r->c2.x0, r->c2.y0, r->c2.r);
#endif

  return 1;
} /* initrectanglearrondi() */

/* ==================================== */
double distpointrectanglearrondi(point p, rectanglearrondi *r)
/* ==================================== */
/* 
   Calcule la distance d'un point a un rectangle arrondi.
*/
{
  double d, dd;
  double a, b, c, cc, x, y, det, dx, dy, x1, x2;
#ifdef DEBUG
printf("distpointrectanglearrondi: h = %g ; a,b,c = %g,%g,%g\n", r->h, r->s1.a, r->s1.b, r->s1.c);
printf("c1 = %g,%g,%g  ", r->c1.x0, r->c1.y0, r->c1.r);
printf("c2 = %g,%g,%g\n", r->c2.x0, r->c2.y0, r->c2.r);
#endif
  if (r->h < MCGEO_EPSILON) return distpointcercle(p, r->c1);

  a = r->s1.a; b = r->s1.b; c = r->s1.c; /* distance au premier cote */
  det = a * a + b * b;
  cc = b * p.x - a * p.y;
  x = (b * cc - c * a) / det;
  y = (- b * c - cc * a) / det;

  /* verifie si le point d'intersection appartient au premier cote */
  x1 = min(r->s1.x1,r->s1.x2); x2 = max(r->s1.x1,r->s1.x2);
  if ((x >= x1) && (x <= x2))
  {
    dx = x - p.x;
    dy = y - p.y;
    d = sqrt(dx * dx + dy * dy);
#ifdef DEBUG
printf("distpointrectanglearrondi: dist au 1er cote = %g\n", d);
#endif
    a = r->s2.a; b = r->s2.b; c = r->s2.c; /* distance au second cote */
    det = a * a + b * b;
    cc = b * p.x - a * p.y;
    x = (b * cc - c * a) / det;
    y = (- b * c - cc * a) / det;
    dx = x - p.x;
    dy = y - p.y;
    dd = sqrt(dx * dx + dy * dy);
#ifdef DEBUG
printf("distpointrectanglearrondi: dist au 2e cote = %g\n", dd);
#endif
    if (d < dd) return d; else return dd;
  } /* if ((x >= x1) && (x <= x2)) */
  /* else */
  d = distpointcercle(p, r->c1);
  dd = distpointcercle(p, r->c2);
#ifdef DEBUG
printf("distpointrectanglearrondi: dist aux 2 cercles = %g %g\n", d,dd);
#endif
  if (d < dd) return d; else return dd;
} /* distpointrectanglearrondi() */

/* ==================================== */
double SomCarDist(int32_t *pb, int32_t npb, rectangle *R, int32_t rs)
/* ==================================== */
/* calcule la somme des carres des distances des points du bord au rectangle */
{
  int32_t i; 
  point p; 
  double d, c1 = 0;
  for (i = 0; i < npb; i++) 
  {
    p.x = pb[i] % rs;
    p.y = pb[i] / rs;
    d = distpointrectangle(p, R);
    c1 += d * d;
  }
  return c1;
} /* SomCarDist() */

/* ==================================== */
double SomCarDistRA(int32_t *pb, int32_t npb, rectanglearrondi *R, int32_t rs)
/* ==================================== */
/* calcule la somme des carres des distances des points du bord au rectangle arrondi */
{
  int32_t i; 
  point p; 
  double d, c1 = 0;
  for (i = 0; i < npb; i++) 
  {
    p.x = pb[i] % rs;
    p.y = pb[i] / rs;
    d = distpointrectanglearrondi(p, R);
    c1 += d * d;
#ifdef DEBUG
    printf("d=%g ; c1 = %g\n", d, c1);
#endif
  }
  return c1;
} /* SomCarDistRA() */

/* ==================================== */
rectangle *AjusteRectangle(int32_t *pb, int32_t npb, double xc, double yc, double theta, double h, double w, int32_t rs)
/* ==================================== */
/*
 Ajuste les parametres (xc, yx, theta, h, w) d'un rectangle 
 pour minimiser l'ecart (au sens des moindres carres)
 entre ce rectangle et les points contenus dans la liste pb.
 Des valeurs initiales des parametres "proches" des valeurs optimales 
 doivent etre fournies en entree.
 Retourne le rectangle obtenu.
 */
{
  rectangle *R;
  double c, cp, cm, d; 
  double dh = INCRH;
  double dw = INCRW;
  double dx = INCRX;
  double dy = INCRY;
  double dt = INCRT;
  int32_t i;

  R = (rectangle *)calloc(1,sizeof(rectangle));
  R->h = h;
  R->w = w;
  R->angle = theta;
  R->centre.x = xc;
  R->centre.y = yc;

  initrectangle(R);
  c = SomCarDist(pb, npb, R, rs);

  for (i = 0; i < MAXITER; i++)
  {
#ifdef DEBUG
    printf("i = %d\n", i);
#endif
    if (dh >= PRH)
    {
      R->h += dh;
      initrectangle(R);
      cp = SomCarDist(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->h -= dh+dh;
        initrectangle(R);
        cm = SomCarDist(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->h += dh; dh = dh / 2; }
      }
    } /* if (dh >= PRH) */

    if (dw >= PRW)
    {
      R->w += dw;
      initrectangle(R);
      cp = SomCarDist(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->w -= dw+dw;
        initrectangle(R);
        cm = SomCarDist(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->w += dw; dw = dw / 2; }
      }
    } /* if (dw >= PRW) */

    if (dx >= PRX)
    {
      R->centre.x += dx;
      initrectangle(R);
      cp = SomCarDist(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->centre.x -= dx+dx;
        initrectangle(R);
        cm = SomCarDist(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->centre.x += dx; dx = dx / 2; }
      }
    } /* if (dx >= PRX) */

    if (dy >= PRY)
    {
      R->centre.y += dy;
      initrectangle(R);
      cp = SomCarDist(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->centre.y -= dy+dy;
        initrectangle(R);
        cm = SomCarDist(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->centre.y += dy; dy = dy / 2; }
      }
    } /* if (dy >= PRY) */

    if (dt >= PRT)
    {
      R->angle += dt;
      initrectangle(R);
      cp = SomCarDist(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->angle -= dt+dt;
        initrectangle(R);
        cm = SomCarDist(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->angle += dt; dt = dt / 2; }
      }
    } /* if (dt >= PRT) */

    if ((dh < PRH) && (dw < PRW) && (dx < PRX) && (dy < PRY) && (dt < PRT))
      break;

  } /* for (i = 0; i < MAXITER; i++) */

  return R;
} /* AjusteRectangle() */

/* ==================================== */
rectangle *RectangleIncluant(int32_t *pb, int32_t npb, double xc, double yc, double theta, int32_t rs)
/* ==================================== */
{
  rectangle *R;
  int32_t i, p;
  double x, y, xp, yp;
  double x1, x2, x3, x4, y1, y2, y3, y4;
  double xp1, xp2, xp3, xp4, yp1, yp2, yp3, yp4;
  double xpmin, xpmax, ypmin, ypmax;
  double costheta, sintheta;

  R = (rectangle *)calloc(1,sizeof(rectangle));
  R->angle = theta;
  R->centre.x = xc;
  R->centre.y = yc;
  costheta = cos(theta);
  sintheta = sin(theta);

  initrectangle(R);

  p = pb[0]; x = (p%rs) - xc; y = (p/rs) - yc;
  xp = x * costheta + y * sintheta;
  yp = y * costheta - x * sintheta;
  xpmin = xpmax = xp;
  ypmin = ypmax = yp;
  for (i = 1; i < npb; i++)
  {
    p = pb[i]; x = (p%rs) - xc; y = (p/rs) - yc;
    xp = x * costheta + y * sintheta;
    yp = y * costheta - x * sintheta;
    if (xp < xpmin) xpmin = xp; else if (xp > xpmax) xpmax = xp;
    if (yp < ypmin) ypmin = yp; else if (yp > ypmax) ypmax = yp;
  } /* for i */    

  xp1 = xpmin; yp1 = ypmin;
  xp2 = xpmin; yp2 = ypmax;
  xp3 = xpmax; yp3 = ypmax;
  xp4 = xpmax; yp4 = ypmin;
  x1 = xc + (xp1 * costheta) - (yp1 * sintheta);
  y1 = yc + (xp1 * sintheta) + (yp1 * costheta);
  x2 = xc + (xp2 * costheta) - (yp2 * sintheta);
  y2 = yc + (xp2 * sintheta) + (yp2 * costheta);
  x3 = xc + (xp3 * costheta) - (yp3 * sintheta);
  y3 = yc + (xp3 * sintheta) + (yp3 * costheta);
  x4 = xc + (xp4 * costheta) - (yp4 * sintheta);
  y4 = yc + (xp4 * sintheta) + (yp4 * costheta);
  R->s1.x2 = R->s2.x1 = x1;  R->s1.y2 = R->s2.y1 = y1;
  R->s2.x2 = R->s3.x1 = x2;  R->s2.y2 = R->s3.y1 = y2;
  R->s3.x2 = R->s4.x1 = x3;  R->s3.y2 = R->s4.y1 = y3;
  R->s4.x2 = R->s1.x1 = x4;  R->s4.y2 = R->s1.y1 = y4;

  return R;
} /* RectangleIncluant() */

/* ==================================== */
rectanglearrondi *AjusteRectangleArrondi(int32_t *pb, int32_t npb, double xc, double yc, 
                                  double theta, double h, double w, int32_t rs)
/* ==================================== */
{
  rectanglearrondi *R;
  double c, cp, cm, d; 
  double dh = INCRH;
  double dw = INCRW;
  double dx = INCRX;
  double dy = INCRY;
  double dt = INCRT;
  int32_t i;

  R = (rectanglearrondi *)calloc(1,sizeof(rectanglearrondi));
  R->h = h;
  R->w = w;
  R->angle = theta;
  R->centre.x = xc;
  R->centre.y = yc;

  initrectanglearrondi(R);

  c = SomCarDistRA(pb, npb, R, rs);

  for (i = 0; i < MAXITER; i++)
  {
#ifdef DEBUG
    printf("i = %d\n", i);
    printf("R: h=%g ; w=%g ; x=%g ; y=%g ; t=%g\n", R->h, R->w, R->centre.x, R->centre.y, R->angle);
#endif
    if (dh >= PRH)
    {
      R->h += dh;
      initrectanglearrondi(R);
      cp = SomCarDistRA(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->h = max((R->h-(dh+dh)),0); /* pas de valeurs negatives */
        initrectanglearrondi(R);
        cm = SomCarDistRA(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->h += dh; dh = dh / 2; }
      }
    } /* if (dh >= PRH) */

    if (dw >= PRW)
    {
      R->w += dw;
      initrectanglearrondi(R);
      cp = SomCarDistRA(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->w -= dw+dw;
        initrectanglearrondi(R);
        cm = SomCarDistRA(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->w += dw; dw = dw / 2; }
      }
    } /* if (dw >= PRW) */

    if (dx >= PRX)
    {
      R->centre.x += dx;
      initrectanglearrondi(R);
      cp = SomCarDistRA(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->centre.x -= dx+dx;
        initrectanglearrondi(R);
        cm = SomCarDistRA(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->centre.x += dx; dx = dx / 2; }
      }
    } /* if (dx >= PRX) */

    if (dy >= PRY)
    {
      R->centre.y += dy;
      initrectanglearrondi(R);
      cp = SomCarDistRA(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->centre.y -= dy+dy;
        initrectanglearrondi(R);
        cm = SomCarDistRA(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->centre.y += dy; dy = dy / 2; }
      }
    } /* if (dy >= PRY) */

    if (dt >= PRT)
    {
      R->angle += dt;
      initrectanglearrondi(R);
      cp = SomCarDistRA(pb, npb, R, rs);
      if (cp < c) c = cp;
      else
      {
        R->angle -= dt+dt;
        initrectanglearrondi(R);
        cm = SomCarDistRA(pb, npb, R, rs);
        if (cm < c) c = cm;
        else { R->angle += dt; dt = dt / 2; }
      }
    } /* if (dt >= PRT) */

    if ((dh < PRH) && (dw < PRW) && (dx < PRX) && (dy < PRY) && (dt < PRT))
      break;

  } /* for (i = 0; i < MAXITER; i++) */

  return R;
} /* AjusteRectangleArrondi() */

/* ==================================== */
void DrawRectangle(rectangle R, uint8_t *F, int32_t rs)
/* ==================================== */
{
  lbresen(F, rs, R.s1.x1, R.s1.y1, R.s1.x2, R.s1.y2);
  lbresen(F, rs, R.s2.x1, R.s2.y1, R.s2.x2, R.s2.y2);
  lbresen(F, rs, R.s3.x1, R.s3.y1, R.s3.x2, R.s3.y2);
  lbresen(F, rs, R.s4.x1, R.s4.y1, R.s4.x2, R.s4.y2);
} /* DrawRectangle() */

/* ==================================== */
void DrawAxe(rectangle R, uint8_t *F, int32_t rs)
/* ==================================== */
{
  double x1, y1, x2, y2;

  if (dist2(R.s1.x1, R.s1.y1, R.s1.x2, R.s1.y2) <= dist2(R.s2.x1, R.s2.y1, R.s2.x2, R.s2.y2))
  {
    x1 = (R.s1.x1 + R.s1.x2) / 2; y1 = (R.s1.y1 + R.s1.y2) / 2;
    x2 = (R.s3.x1 + R.s3.x2) / 2; y2 = (R.s3.y1 + R.s3.y2) / 2;
  }
  else
  {
    x1 = (R.s2.x1 + R.s2.x2) / 2; y1 = (R.s2.y1 + R.s2.y2) / 2;
    x2 = (R.s4.x1 + R.s4.x2) / 2; y2 = (R.s4.y1 + R.s4.y2) / 2;
  }
  lbresen(F, rs, x1, y1, x2, y2);
} /* DrawAxe() */

/* ==================================== */
void DrawRectangleArrondi(rectanglearrondi R, uint8_t *F, int32_t rs, int32_t cs)
/* ==================================== */
{
  lbresen(F, rs, R.s1.x1, R.s1.y1, R.s1.x2, R.s1.y2);
  lbresen(F, rs, R.s2.x1, R.s2.y1, R.s2.x2, R.s2.y2);
  lellipse(F, rs, cs, R.w/2, 0, 0, R.w/2, R.c1.x0, R.c1.y0);
  lellipse(F, rs, cs, R.w/2, 0, 0, R.w/2, R.c2.x0, R.c2.y0);
} /* DrawRectangleArrondi() */

/****************************************************************
*
* Routine Name: lfenetreincluante
*
* Purpose: determine le plus petit rectangle de cotes paralleles
*          aux axes dans lequel chaque composante connexe est incluse
* Input:
* Output:
* Written By: Michel Couprie - fevrier 2001
*
****************************************************************/

/* ==================================== */
int32_t lfenetreincluante(
        struct xvimage *img,    /* image de depart */
        int32_t connex)             /* 4, 8  */
/* ==================================== */
#undef F_NAME
#define F_NAME "lfenetreincluante"
{
  int32_t i, k, l, n;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  struct xvimage * result;
  uint8_t *RES;
  Lifo * LIFO;
  int32_t incr_vois;
  int32_t xmin, ymin, xmax, ymax;
  rectangle R;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, LABMAX, labels, &n))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  result = copyimage(img);
  if (result == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }
  RES = UCHARDATA(result);
  memset(RES, 0, N);

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  /* Remarque: Cet algo est loin d'etre optimal ... */
  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      xmin = xmax = x%rs;
      ymin = ymax = x/rs;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        if (w%rs < xmin) xmin = w%rs; else if (w%rs > xmax) xmax = w%rs;
        if (w/rs < ymin) ymin = w/rs; else if (w/rs > ymax) ymax = w/rs;
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

      LABEL[x] = 0;              /* label nul */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer */
      {
        w = LifoPop(LIFO);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n+1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      R.s1.x2 = R.s2.x1 = xmin;  R.s1.y2 = R.s2.y1 = ymin;
      R.s2.x2 = R.s3.x1 = xmin;  R.s2.y2 = R.s3.y1 = ymax;
      R.s3.x2 = R.s4.x1 = xmax;  R.s3.y2 = R.s4.y1 = ymax;
      R.s4.x2 = R.s1.x1 = xmax;  R.s4.y2 = R.s1.y1 = ymin;
      DrawRectangle(R, RES, rs);

    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  memcpy(SOURCE, RES, N);
  LifoTermine(LIFO);
  freeimage(labels);
  freeimage(result);
  return(1);
} /* fenetreincluante() */

/****************************************************************
*
* Routine Name: lboxmin
*
* Purpose: determine la plus petite boite de cotes paralleles
*          aux axes dans lequel tous les points de l'image se trouvent
* Input:
* Output:
* Written By: Michel Couprie - novembre 2002
*
****************************************************************/

/* ==================================== */
int32_t lboxmin(struct xvimage *img)
/* ==================================== */
#undef F_NAME
#define F_NAME "lboxmin"
{
  int32_t x, y, z;
  uint8_t *F = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t ds = depth(img);
  int32_t ps = rs * cs;
  int32_t N = ps * ds;          /* taille image */
  int32_t xmin, xmax, ymin, ymax, zmin, zmax;

  xmin = rs; xmax = -1; 
  ymin = cs; ymax = -1; 
  zmin = ds; zmax = -1; 
  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if (F[z*ps + y*rs + x])
        {
          if (x < xmin) xmin = x;
          if (x > xmax) xmax = x;
          if (y < ymin) ymin = y;
          if (y > ymax) ymax = y;
          if (z < zmin) zmin = z;
          if (z > zmax) zmax = z;
        } // for z for y for x if (F[x])
  //printf("%d %d %d %d %d %d\n", xmin, xmax, ymin, ymax, zmin, zmax);
  for (z = 0; z < ds; z++)
    for (y = 0; y < cs; y++)
      for (x = 0; x < rs; x++)
        if ((x >= xmin) && (x <= xmax) && (y >= ymin) && (y <= ymax) && (z >= zmin) && (z <= zmax))
          F[z*ps + y*rs + x] = NDG_MAX;
  return(1);
} // boxmin()

/****************************************************************
*
* Routine Name: lmatchrect - library call for matchrect
*
* Purpose: segmentation d'objets de forme rectangulaire
* Input:
* Output:
* Written By: Michel Couprie - fevrier 2001
*
****************************************************************/

/* ==================================== */
int32_t lmatchrect(
        struct xvimage *img,    /* image de depart */
        int32_t connex,             /* 4, 8  */
        int32_t mode)               /* 0: normal, 1: arrondi, 2: axe */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmatchrect"
{
  int32_t i, k, l, n;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  struct xvimage * result;
  uint8_t *RES;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mx2, my2, mxy1;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  double x1, y1, x2, y2;
  double theta;
  rectanglearrondi *Ra;
  rectangle *R;
  int32_t *pointsbord;
  int32_t nbpointsbord;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, LABMAX, labels, &n))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  result = copyimage(img);
  if (result == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }
  RES = UCHARDATA(result);
  memset(RES, 0, N);

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  pointsbord = (int32_t *)calloc(1,N * sizeof(int32_t));
  if (pointsbord == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return 0;
  }

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mx2 = my2 = mxy1 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += (w%rs) * (w/rs);
        mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
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

      xc = mx1/area;
      yc = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy1 - mx1 * my1 / area;

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
        fprintf(stderr, "%s : valeur propre negative : %g\n", F_NAME, lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < MCGEO_EPSILON) 
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

      LABEL[x] = 0;              /* label nul */
      nbpointsbord = 0;
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer et stocker les points de bord */
      {
        w = LifoPop(LIFO);
        switch (connex)
        {
          case 4: if (nbvoisc8(SOURCE,w,rs,N) != 0) pointsbord[nbpointsbord++] = w; break;
          case 8: if (nbvoisc4(SOURCE,w,rs,N) != 0) pointsbord[nbpointsbord++] = w; break;
        }        
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n+1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      if (mode == 1)
      {
        Ra = AjusteRectangleArrondi(pointsbord, nbpointsbord, xc, yc, theta, 2.0 * (lambda1-lambda2), 2.0 * lambda2, rs);
        DrawRectangleArrondi(*Ra, RES, rs, cs);
      }
      else
      {
        R = AjusteRectangle(pointsbord, nbpointsbord, xc, yc, theta, 2.0 * lambda1, 2.0 * lambda2, rs);
        if (mode == 0) 
          DrawRectangle(*R, RES, rs);
        else
          DrawAxe(*R, RES, rs);
      }
    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  memcpy(SOURCE, RES, N);
  LifoTermine(LIFO);
  freeimage(labels);
  freeimage(result);
  free(pointsbord);
  return(1);
} /* matchrect() */

/****************************************************************
*
* Routine Name: lrectangleincluant - library call for rectangleincluant
*
* Purpose: segmentation d'objets de forme rectangulaire
* Input:
* Output:
* Written By: Michel Couprie - fevrier 2001
*
****************************************************************/

/* ==================================== */
int32_t lrectangleincluant(
        struct xvimage *img,    /* image de depart */
        int32_t connex)             /* 4, 8  */
/* ==================================== */
#undef F_NAME
#define F_NAME "lrectangleincluant"
{
  int32_t i, k, l, n;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  struct xvimage * result;
  uint8_t *RES;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mx2, my2, mxy1;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  double x1, y1, x2, y2;
  double theta;
#ifdef RECTARRONDI
  rectanglearrondi *R;
#else
  rectangle *R;
#endif
  int32_t *pointsbord;
  int32_t nbpointsbord;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, LABMAX, labels, &n))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  result = copyimage(img);
  if (result == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }
  RES = UCHARDATA(result);
  memset(RES, 0, N);

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  pointsbord = (int32_t *)calloc(1,N * sizeof(int32_t));
  if (pointsbord == NULL)
  {
    fprintf(stderr, "%s: malloc failed\n", F_NAME);
    return 0;
  }

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mx2 = my2 = mxy1 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += (w%rs) * (w/rs);
        mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
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

      xc = mx1/area;
      yc = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy1 - mx1 * my1 / area;

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
        fprintf(stderr, "%s : valeur propre negative : %g\n", F_NAME, lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < MCGEO_EPSILON) 
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

      LABEL[x] = 0;              /* label nul */
      nbpointsbord = 0;
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer et stocker les points de bord */
      {
        w = LifoPop(LIFO);
        switch (connex)
        {
          case 4: if (nbvoisc8(SOURCE,w,rs,N) != 0) pointsbord[nbpointsbord++] = w; break;
          case 8: if (nbvoisc4(SOURCE,w,rs,N) != 0) pointsbord[nbpointsbord++] = w; break;
        }        
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n+1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

      R = RectangleIncluant(pointsbord, nbpointsbord, xc, yc, theta, rs);

      DrawRectangle(*R, RES, rs);

    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  memcpy(SOURCE, RES, N);
  LifoTermine(LIFO);
  freeimage(labels);
  freeimage(result);
  free(pointsbord);
  return(1);
} /* rectangleincluant() */

/****************************************************************
*
* Routine Name: lmatchellipse - library call for matchellipse
*
* Purpose: segmentation d'objets de forme elliptique
* Input:
* Output:
* Written By: Michel Couprie - fevrier 2001
*
****************************************************************/

/* ==================================== */
int32_t lmatchellipse(
        struct xvimage *img,    /* image de depart */
        int32_t connex)             /* 4, 8  */
/* ==================================== */
#undef F_NAME
#define F_NAME "lmatchellipse"
{
  int32_t k, l, n;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * result;
  uint8_t *RES;
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mx2, my2, mxy1;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  double x1, y1, x2, y2;
  double theta;
  ellipse *ell;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  result = copyimage(img);
  if (result == NULL)
  {   fprintf(stderr, "%s : CreeLifoVide failed\n", F_NAME);
      return(0);
  }
  RES = UCHARDATA(result);
  memset(RES, 0, N);

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, LABMAX, labels, &n))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "%s() : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  ell = AllocEllipseList(rs, cs);

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mx2 = my2 = mxy1 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 2x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += (w%rs) * (w/rs);
        mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
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

      xc = mx1/area;
      yc = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy1 - mx1 * my1 / area;

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
        fprintf(stderr, "%s : valeur propre negative : %g\n", F_NAME, lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < MCGEO_EPSILON) 
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

      x1 = lambda1 * cos(theta);
      y1 = lambda1 * sin(theta);
      x2 = lambda2 * cos(theta + M_PI/2);
      y2 = lambda2 * sin(theta + M_PI/2);

      lellipse(RES, rs, cs, x1, y1, x2, y2, xc, yc);

      LABEL[x] = 0;              /* label nul */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer */
      {
        w = LifoPop(LIFO);
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n+1)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */

  memcpy(SOURCE, RES, N);
  FreeEllipseList(ell);
  LifoTermine(LIFO);
  freeimage(labels);
  freeimage(result);

  return(1);
} /* matchellipse() */


/****************************************************************
*
* Routine Name: lellipticite - library call for ellipticite
*
* Purpose: segmentation d'objets de forme elliptique
* Input:
* Output:
* Written By: Michel Couprie - novembre 1998
*
* facteur d'ellipticite:
*   a = aire de la composante
*   ni = nombre de pixels de la composante dans l'ellipse de la composante
*   f = ni / a
*
*   ce facteur vaut 1 pour un objet entierement inclus dans son ellipse
*   (mais pas forcement de forme elliptique)
*
****************************************************************/

/* ==================================== */
int32_t lellipticite(
        struct xvimage *img,    /* image de depart */
        int32_t connex)             /* 4, 8  */
/* ==================================== */
#undef F_NAME
#define F_NAME "lellipticite"
{
  int32_t k, l, n;
  int32_t w, x, y, z;
  uint8_t *SOURCE = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t d = depth(img);
  int32_t N = rs * cs;          /* taille image */
  struct xvimage * labels;
  uint32_t *LABEL;
  uint32_t label;
  Lifo * LIFO;
  int32_t area;
  double mx1, my1, mx2, my2, mxy1;
  double Mxy1, Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;
  int32_t incr_vois;
  double xx, yy, xc, yc, a;
  double x1, y1, x2, y2;
  double theta;
  ellipse *ell;
  uint8_t ellipticite;
  int32_t ni;

  if (depth(img) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "%s: mauvaise connexite: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */

  labels = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (labels == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  LABEL = ULONGDATA(labels);

  if (!llabelextrema(img, connex, LABMAX, labels, &n))
  {
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "%s() : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  ell = AllocEllipseList(rs, cs);

  for (x = 0; x < N; x++)
  {
    if (LABEL[x])
    {
#ifdef DEBUG
printf("point %d %d : nouvelle composante\n", x%rs, x/rs);
#endif
      label = LABEL[x];
      LABEL[x] = n+1; /* marquage provisoire */
      area = 0; mx1 = my1 = mx2 = my2 = mxy1 = 0.0;
      LifoPush(LIFO, x);         /* on va parcourir 3x la comp. a laquelle appartient x */
      while (! LifoVide(LIFO))   /* premier parcours */
      {
        w = LifoPop(LIFO);
        area++; mx1 += w%rs; my1 += w/rs; mxy1 += (w%rs) * (w/rs);
        mx2 += (w%rs) * (w%rs); my2 += (w/rs) * (w/rs);
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

      xc = mx1/area;
      yc = my1/area;

      /* moments centres d'ordre 1 */
      Mxy1 = (mxy1 - (mx1 * yc)) - (my1 * xc) + (area * xc * yc);
      
      /* moments centres d'ordre 2 */
      Mx2 = mx2 - mx1 * mx1 / area;  
      My2 = my2 - my1 * my1 / area;  
      Mxy2 = mxy1 - mx1 * my1 / area;

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
        fprintf(stderr, "%s : valeur propre negative : %g\n", F_NAME, lambda2);
        return 0;
      }

      if (abs(Mx2 - My2) < MCGEO_EPSILON) 
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

      x1 = lambda1 * cos(theta);
      y1 = lambda1 * sin(theta);
      x2 = lambda2 * cos(theta + M_PI/2);
      y2 = lambda2 * sin(theta + M_PI/2);

      lellipselist(ell, rs, cs, x1, y1, x2, y2, xc, yc);
      ni = 0;
      LABEL[x] = n+2;            /* nouveau label */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* et on calcule le facteur d'ellipticite */
      {
        w = LifoPop(LIFO);
        if (InEllipse(ell,w%rs,w/rs)) ni++;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n + 1)) 
          {
            LABEL[y] = n + 2;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */          
      } /* while (! LifoVide(LIFO)) */

      ellipticite = (uint8_t)((255 * ni) / area);

      LABEL[x] = 0;              /* label nul */
      LifoPush(LIFO, x);         /* on re-parcourt la composante */
      while (! LifoVide(LIFO))   /* pour la demarquer et stocker l'ellipticite dans img */
      {
        w = LifoPop(LIFO);
        SOURCE[w] = ellipticite;        
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if ((y != -1) && (LABEL[y] == n + 2)) 
          {
            LABEL[y] = 0;
            LifoPush(LIFO, y);
	  }
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */

#ifdef DEBUG
printf("l1 = %g ; l2 = %g ; a = %d ; ni = %d\n", lambda1, lambda2, area, ni);
for (y = 0; y < cs; y++) 
  if (ell->xmin[y] <= ell->xmax[y])
  {
    SOURCE[y*rs + ell->xmin[y]] = NDG_MAX;
    SOURCE[y*rs + ell->xmax[y]] = NDG_MAX;
  }
#endif

    } /* if (LABEL[x]) */
  } /* for (x = 0; x < N; x++) */


  FreeEllipseList(ell);
  LifoTermine(LIFO);
  freeimage(labels);

  return(1);
} /* lellipticite() */

/* ==================================== */
double MeanGrayCircle(struct xvimage *img, cercle *C)
/* ==================================== */
{
  uint8_t *I = UCHARDATA(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t N = rs * cs;          /* taille image */
  double sumgray = 0.0; 
  Liste *lp = lellipseliste(rs, cs, 0, C->r, C->r, 0, C->x0, C->y0);
  int32_t p, i, numpoints = lp->Sp;
  for (i = 0; i < numpoints; i++)
  {
    p = lp->Pts[i];
    sumgray += (double)(I[p]);
  }
  ListeTermine(lp);
  return sumgray / numpoints;
} // MeanGrayCircle()

/* ==================================== */
cercle *AjusteCercle(struct xvimage *img, double xc, double yc, double r)
/* ==================================== */
#undef F_NAME
#define F_NAME  "AjusteCercle"
/*
  Ajuste les parametres (xc, yc, r) d'un cercle minimisant localement
  le niveau de gris moyen des points "sous" le cercle dans 
  l'image img.
  Des valeurs initiales des parametres "proches" des valeurs optimales 
  doivent etre fournies en entree.
  Retourne le cercle obtenu et imprime ses parametres dans stdout.
*/
{
  cercle *C;
  double cost, cp, cm; 
  double dx = INCRX;
  double dy = INCRY;
  double dr = INCRR;
  int32_t i, stab;

  C = (cercle *)calloc(1,sizeof(cercle));
  C->r = r;
  C->x0 = xc;
  C->y0 = yc;
  cost = MeanGrayCircle(img, C);

  for (i = 0; i < MAXITER; i++)
  {
    stab = 1;
#ifdef DEBUG
    printf("i = %d\n", i);
#endif

    // STEP FOR THE X DIMENSION
    C->x0 += dx;
    cp = MeanGrayCircle(img, C);
    if (cp < cost) { cost = cp; stab = 0; }
    else
    {
      C->x0 -= dx+dx;
      cm = MeanGrayCircle(img, C);
      if (cm < cost) { cost = cm; stab = 0; }
      else C->x0 += dx;
    }

    // STEP FOR THE Y DIMENSION
    C->y0 += dy;
    cp = MeanGrayCircle(img, C);
    if (cp < cost) { cost = cp; stab = 0; }
    else
    {
      C->y0 -= dy+dy;
      cm = MeanGrayCircle(img, C);
      if (cm < cost) { cost = cm; stab = 0; }
      else C->y0 += dy;
    }

    // STEP FOR THE R DIMENSION
    C->r += dr;
    cp = MeanGrayCircle(img, C);
    if (cp < cost) { cost = cp; stab = 0; }
    else
    {
      C->r -= dr+dr;
      cm = MeanGrayCircle(img, C);
      if (cm < cost) { cost = cm; stab = 0; }
      else C->r += dr;
    }

    // LOOP TESTS
    if (stab) { dx /= 2; dy /= 2; dr /= 2; }
    if ((dx < PRX) && (dy < PRY) && (dr < PRR))
      break;

  } /* for (i = 0; i < MAXITER; i++) */
  return C;
} /* AjusteCercle() */


/* ==================================== */
cercle *AjusteCercle2(struct xvimage *img, double xc, double yc, double r)
/* ==================================== */
#undef F_NAME
#define F_NAME  "AjusteCercle2"
/*
  Ajuste les parametres (xc, yc, r) d'un cercle minimisant localement
  le niveau de gris moyen des points "sous" le cercle dans 
  l'image img.
  Des valeurs initiales des parametres "proches" des valeurs optimales 
  doivent etre fournies en entree.
  La minimisation se fait seulement sur x et y.
  Retourne le cercle obtenu et imprime ses parametres dans stdout.
*/
{
  cercle *C;
  double cost, cp, cm; 
  double dx = INCRX;
  double dy = INCRY;
  double dr = INCRR;
  int32_t i, stab;

  C = (cercle *)calloc(1,sizeof(cercle));
  C->r = r;
  C->x0 = xc;
  C->y0 = yc;
  cost = MeanGrayCircle(img, C);

  for (i = 0; i < MAXITER; i++)
  {
    stab = 1;
#ifdef DEBUG
    printf("i = %d\n", i);
#endif

    // STEP FOR THE X DIMENSION
    C->x0 += dx;
    cp = MeanGrayCircle(img, C);
    if (cp < cost) { cost = cp; stab = 0; }
    else
    {
      C->x0 -= dx+dx;
      cm = MeanGrayCircle(img, C);
      if (cm < cost) { cost = cm; stab = 0; }
      else C->x0 += dx;
    }

    // STEP FOR THE Y DIMENSION
    C->y0 += dy;
    cp = MeanGrayCircle(img, C);
    if (cp < cost) { cost = cp; stab = 0; }
    else
    {
      C->y0 -= dy+dy;
      cm = MeanGrayCircle(img, C);
      if (cm < cost) { cost = cm; stab = 0; }
      else C->y0 += dy;
    }

    // LOOP TESTS
    if (stab) { dx /= 2; dy /= 2; }
    if ((dx < PRX) && (dy < PRY))
      break;

  } /* for (i = 0; i < MAXITER; i++) */
  return C;
} /* AjusteCercle2() */

/* ==================================== */
struct xvimage *lcar2pol(struct xvimage *img, double xc, double yc)
/* ==================================== */
#undef F_NAME
#define F_NAME  "lcar2pol"
/*
  Converts an image from cartesian to polar coordinates
*/
{
  struct xvimage *result;
  uint8_t *F = UCHARDATA(img);
  uint8_t *R;
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t N = rs * cs;          /* taille image */
  int32_t angle_res, xi, yi, xs, ys, r, a;
  double angle_step, max_r, t, x, y, f1, f2, f;

  // calculates the radius max_r of the smallest disc centered on xc, yc
  // which contains the image
  max_r = dist2(xc, yc, 0.0, 0.0); 
  t = dist2(xc, yc, 0.0, (double)(cs-1)); 
  if (t > max_r) max_r = t;
  t = dist2(xc, yc, (double)(rs-1), 0.0); 
  if (t > max_r) max_r = t;
  t = dist2(xc, yc, (double)(rs-1), (double)(cs-1)); 
  if (t > max_r) max_r = t;
  max_r += 1.0;

  angle_step = asin(1.0 / max_r);
  angle_res = (int32_t)(2 * M_PI / angle_step) + 1;

  result = allocimage(NULL, (int32_t)max_r, angle_res, 1, VFF_TYP_1_BYTE);
  if (result == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  R = UCHARDATA(result);
  memset(R, 0, (int32_t)max_r * angle_res);

  for (r = 0; r < (int32_t)max_r; r ++)
  {
    for (a = 0, t = 0.0; a < angle_res; a++, t += angle_step)
    {
      x = xc + r * cos(t);
      y = yc + r * sin(t);
      xi = (int32_t)floor(x); xs = xi + 1;
      yi = (int32_t)floor(y); ys = yi + 1;
      if ((xi >= 0) && (yi >= 0) && (xs < rs) && (ys < cs))
      {
        f1 = (x - xi) * F[yi*rs + xs] + (xs - x) * F[yi*rs + xi];
        f2 = (x - xi) * F[ys*rs + xs] + (xs - x) * F[ys*rs + xi];
        f = (y - yi) * f2 + (ys - y) * f1;
        R[a*(int32_t)max_r + r] = arrondi(f);
      }
    } // for a
  } // for r
  return result;
} // lcar2pol()

/* ==================================== */
int32_t lpol2car(struct xvimage *img, struct xvimage *res, double xc, double yc)
/* ==================================== */
#undef F_NAME
#define F_NAME  "lpol2car"
/*
  Converts an image from polar to cartesian coordinates
*/
{
  uint8_t *F = UCHARDATA(img);
  uint8_t *R = UCHARDATA(res);
  int32_t rsp = rowsize(img);
  int32_t csp = colsize(img);
  int32_t Np = rsp * csp;          /* taille image polaire */
  int32_t rsc = rowsize(res);
  int32_t csc = colsize(res);
  int32_t N = rsc * csc;           /* taille image cartesienne */
  double max_r, t, angle_step, r, a, f1, f2, f;
  int32_t ri, rs, ai, as, x, y;

  memset(R, 0, N);

  for (y = 0; y < csc; y++)
    for (x = 0; x < rsc; x++)
    {
      r = dist2(xc, yc, (double)x, (double)y);
      a = atan2((double)yc - y, (double)xc - x);
      ri = (int32_t)floor(r); rs = ri + 1;
      a += M_PI;
      a = (a * (csp-2)) / (2*M_PI);
      ai = (int32_t)floor(a); as = ai + 1;
      if ((ri >= 0) && (ai >= 0) && (rs < rsp) && (as < csp))
      {
        f1 = (r - ri) * F[ai*rsp + rs] + (rs - r) * F[ai*rsp + ri];
        f2 = (r - ri) * F[as*rsp + rs] + (rs - r) * F[as*rsp + ri];
        f = (a - ai) * f2 + (as - a) * f1;
        R[y*rsc + x] = arrondi(f);
      }
    } // for y for x

  return 1;
} // lpol2car()

/* ==================================== */
struct xvimage *lsection_old(struct xvimage *img, 
                         double x0, double y0, double z0,
                         double x1, double y1, double z1,
                         double x2, double y2, double z2
                        )
/* ==================================== */
#undef F_NAME
#define F_NAME  "lsection"
/*
  2d cross-section from a 3d image
*/
{
  struct xvimage *result;
  uint8_t *F = UCHARDATA(img);
  uint8_t *R;
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t ds = depth(img);
  int32_t ps = rs * cs;          /* taille plan */
  int32_t N = ps * ds;           /* taille image */
  double xdim = img->xdim;
  double ydim = img->ydim;
  double zdim = img->zdim;
  int32_t rs2, cs2, N2; 
  double dx1, dy1, dz1, dx2, dy2, dz2, x, y, z;
  double fziyi, fziys, fzsyi, fzsys, fzi, fzs, f;
  int32_t i, j, imin, imax, jmin, jmax, ii, jj, xi, yi, zi, xs, ys, zs;

  // computes the size of the resulting image
  if ((x0 < 0) || (x0 > rs-1) || (y0 < 0) || (y0 > cs-1) || (z0 < 0) || (z0 > ds-1))
  {
    fprintf(stderr, "%s : base point out of bounds\n", F_NAME);
    return NULL;
  }  
  x = x0; y = y0; z = z0; 
  dx1 = (x1 - x0) / xdim; 
  dy1 = (y1 - y0) / ydim; 
  dz1 = (z1 - z0) / zdim;
  i = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x += dx1; y += dy1; z += dz1; i++; 
  }
  imax = i - 1;

  printf("x0=%f, y0=%f, z0=%f, x1=%f, y1=%f, z1=%f, x2=%f, y2=%f, z2=%f, dx1=%f, dy1=%f, dz1=%f, imax=%d\n", x0, y0, z0, x1, y1, z1, x2, y2, z2, dx1, dy1, dz1, imax);
  fflush(stdout);

  x = x0; y = y0; z = z0; 
  i = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x -= dx1; y -= dy1; z -= dz1; i--; 
  }
  imin = i + 1;

  x = x0; y = y0; z = z0; 
  dx2 = (x2 - x0) / xdim; 
  dy2 = (y2 - y0) / ydim; 
  dz2 = (z2 - z0) / zdim;
  j = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x += dx2; y += dy2; z += dz2; j++; 
  }
  jmax = j - 1;

  x = x0; y = y0; z = z0; 
  j = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x -= dx2; y -= dy2; z -= dz2; j--; 
  }
  jmin = j + 1;

  rs2 = imax - imin;
  cs2 = jmax - jmin;
  N2 = rs2 * cs2; 

  result = allocimage(NULL, rs2, cs2, 1, VFF_TYP_1_BYTE);
  if (result == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }
  R = UCHARDATA(result);
  memset(R, 0, N2);

  // interpolation loop

  for (j = 0; j < cs2; j++)
  {
    jj = jmin + j;
    for (i = 0; i < rs2; i++)
    {
      ii = imin + i;
      x = x0 + ii*dx1 + jj*dx2;
      y = y0 + ii*dy1 + jj*dy2;
      z = z0 + ii*dz1 + jj*dz2;
      xi = (int32_t)floor(x); xs = xi + 1;
      yi = (int32_t)floor(y); ys = yi + 1;
      zi = (int32_t)floor(z); zs = zi + 1;
      if ((xi >= 0) && (yi >= 0) && (zi >= 0) && (xs < rs) && (ys < cs) && (zs < ds))
      {
        fziyi = (x - xi) * F[zi*ps + yi*rs + xs] + (xs - x) * F[zi*ps + yi*rs + xi];
        fziys = (x - xi) * F[zi*ps + ys*rs + xs] + (xs - x) * F[zi*ps + ys*rs + xi];
        fzsyi = (x - xi) * F[zs*ps + yi*rs + xs] + (xs - x) * F[zs*ps + yi*rs + xi];
        fzsys = (x - xi) * F[zs*ps + ys*rs + xs] + (xs - x) * F[zs*ps + ys*rs + xi];
        fzi = (y - yi) * fziys + (ys - y) * fziyi;
        fzs = (y - yi) * fzsys + (ys - y) * fzsyi;
        f = (z - zi) * fzs + (zs - z) * fzi;
        R[j*rs2 + i] = arrondi(f);
      }
    }
  }

  return result;
} // lsection()

/* ==================================== */
struct xvimage *lsection(struct xvimage *img, 
                         double x0, double y0, double z0,
                         double x1, double y1, double z1,
                         double x2, double y2, double z2
                        )
/* ==================================== */
#undef F_NAME
#define F_NAME  "lsection"
/*
  2d cross-section from a 3d image
*/
{
  struct xvimage *result;
  int32_t imType = datatype(img);
  int32_t rs = rowsize(img);
  int32_t cs = colsize(img);
  int32_t ds = depth(img);
  int32_t ps = rs * cs;          /* taille plan */
  int32_t N = ps * ds;           /* taille image */
  double xdim = img->xdim;
  double ydim = img->ydim;
  double zdim = img->zdim;
  int32_t rs2, cs2, N2; 
  double dx1, dy1, dz1, dx2, dy2, dz2, x, y, z;
  double fziyi, fziys, fzsyi, fzsys, fzi, fzs, f;
  int32_t i, j, imin, imax, jmin, jmax, ii, jj, xi, yi, zi, xs, ys, zs;

  // computes the size of the resulting image
  if ((x0 < 0) || (x0 > rs-1) || (y0 < 0) || (y0 > cs-1) || (z0 < 0) || (z0 > ds-1))
  {
    fprintf(stderr, "%s : base point out of bounds\n", F_NAME);
    return NULL;
  }  
  x = x0; y = y0; z = z0; 
  dx1 = (x1 - x0) / xdim; 
  dy1 = (y1 - y0) / ydim; 
  dz1 = (z1 - z0) / zdim;
  i = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x += dx1; y += dy1; z += dz1; i++; 
  }
  imax = i - 1;

  x = x0; y = y0; z = z0; 
  i = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x -= dx1; y -= dy1; z -= dz1; i--; 
  }
  imin = i + 1;

  x = x0; y = y0; z = z0; 
  dx2 = (x2 - x0) / xdim; 
  dy2 = (y2 - y0) / ydim; 
  dz2 = (z2 - z0) / zdim;
  j = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x += dx2; y += dy2; z += dz2; j++; 
  }
  jmax = j - 1;

  x = x0; y = y0; z = z0; 
  j = 0; 
  while (! ((x < 0) || (x > rs-1) || (y < 0) || (y > cs-1) || (z < 0) || (z > ds-1)))
  {
    x -= dx2; y -= dy2; z -= dz2; j--; 
  }
  jmin = j + 1;

  rs2 = imax - imin;
  cs2 = jmax - jmin;
  N2 = rs2 * cs2; 

  result = allocimage(NULL, rs2, cs2, 1, imType);
  if (result == NULL)
  {
    fprintf(stderr, "%s : allocimage failed\n", F_NAME);
    return NULL;
  }

  if (imType == VFF_TYP_1_BYTE) {
    uint8_t *F = UCHARDATA(img);
    uint8_t *R = UCHARDATA(result);

    memset(R, 0, N2);

    // interpolation loop

    for (j = 0; j < cs2; j++)
      {
	jj = jmin + j;
	for (i = 0; i < rs2; i++)
	  {
	    ii = imin + i;
	    x = x0 + ii*dx1 + jj*dx2;
	    y = y0 + ii*dy1 + jj*dy2;
	    z = z0 + ii*dz1 + jj*dz2;
	    xi = (int32_t)floor(x); xs = xi + 1;
	    yi = (int32_t)floor(y); ys = yi + 1;
	    zi = (int32_t)floor(z); zs = zi + 1;
	    if ((xi >= 0) && (yi >= 0) && (zi >= 0) && (xs < rs) && (ys < cs) && (zs < ds))
	      {
		fziyi = (x - xi) * F[zi*ps + yi*rs + xs] + (xs - x) * F[zi*ps + yi*rs + xi];
		fziys = (x - xi) * F[zi*ps + ys*rs + xs] + (xs - x) * F[zi*ps + ys*rs + xi];
		fzsyi = (x - xi) * F[zs*ps + yi*rs + xs] + (xs - x) * F[zs*ps + yi*rs + xi];
		fzsys = (x - xi) * F[zs*ps + ys*rs + xs] + (xs - x) * F[zs*ps + ys*rs + xi];
		fzi = (y - yi) * fziys + (ys - y) * fziyi;
		fzs = (y - yi) * fzsys + (ys - y) * fzsyi;
		f = (z - zi) * fzs + (zs - z) * fzi;
		R[j*rs2 + i] = arrondi(f);
	      }
	  }
      }
  } else if (imType == VFF_TYP_4_BYTE) {
    uint32_t *F = ULONGDATA(img);
    uint32_t *R = ULONGDATA(result);

    memset(R, 0, 4*N2);

    // interpolation loop

    for (j = 0; j < cs2; j++)
      {
	jj = jmin + j;
	for (i = 0; i < rs2; i++)
	  {
	    ii = imin + i;
	    x = x0 + ii*dx1 + jj*dx2;
	    y = y0 + ii*dy1 + jj*dy2;
	    z = z0 + ii*dz1 + jj*dz2;
	    xi = (int32_t)floor(x); xs = xi + 1;
	    yi = (int32_t)floor(y); ys = yi + 1;
	    zi = (int32_t)floor(z); zs = zi + 1;
	    if ((xi >= 0) && (yi >= 0) && (zi >= 0) && (xs < rs) && (ys < cs) && (zs < ds))
	      {
		fziyi = (x - xi) * F[zi*ps + yi*rs + xs] + (xs - x) * F[zi*ps + yi*rs + xi];
		fziys = (x - xi) * F[zi*ps + ys*rs + xs] + (xs - x) * F[zi*ps + ys*rs + xi];
		fzsyi = (x - xi) * F[zs*ps + yi*rs + xs] + (xs - x) * F[zs*ps + yi*rs + xi];
		fzsys = (x - xi) * F[zs*ps + ys*rs + xs] + (xs - x) * F[zs*ps + ys*rs + xi];
		fzi = (y - yi) * fziys + (ys - y) * fziyi;
		fzs = (y - yi) * fzsys + (ys - y) * fzsyi;
		f = (z - zi) * fzs + (zs - z) * fzi;
		R[j*rs2 + i] = arrondi(f);
	      }
	  }
      }
  } else {
    fprintf(stderr, "%s : image datatype not supported\n", F_NAME);
    return NULL;
  }

  return result;
} // lsection()

/* ==================================== */
int32_t lidentifyplane(double *pbx, double *pby, double *pbz, int32_t npb, double *a, double *b, double *c, double *d)
/* ==================================== */
#undef F_NAME
#define F_NAME "lidentifyplane"
/*
 Identifie les parametres (a, b, c, d) de l'equation d'un plan 3D
 pour minimiser l'ecart (au sens des moindres carres)
 entre ce plan et les points contenus dans la liste de points (pbx,pby,pbz).
 Régression linéaire.
 */
{
  int32_t i, ret;
  double *X, *Y, *XtX, *XtXi, *XtY, *R;

  if (npb < 3)
  {
    fprintf(stderr, "%s: not enough points\n", F_NAME);
    return 0;
  }  

  X = lin_zeros(3, npb);
  Y = lin_zeros(1, npb);
  XtX = lin_zeros(3, 3);
  XtXi = lin_zeros(3, 3);
  XtY = lin_zeros(3, 1);
  R = lin_zeros(3, 1);

  for (i = 0; i < npb; i++)
  {
    X[3*i] = 1.0;
    X[3*i + 1] = pbx[i];
    X[3*i + 2] = pby[i];
    Y[i] = pbz[i];
  }  

  lin_multAtB(X, X, XtX, npb, 3, npb, 3);
  ret = lin_inverseLUP(XtX, XtXi, 3);
  if (ret != 0)
  { 
    lin_multAtB(X, Y, XtY, npb, 3, npb, 1);
    lin_mult(XtXi, XtY, R, 3, 3, 1);
    *a = R[1];
    *b = R[2];
    *c = -1;
    *d = R[0];
  }
  else
  {
    for (i = 0; i < npb; i++)
    {
      X[3*i] = 1.0;
      X[3*i + 1] = pbz[i];
      X[3*i + 2] = pbx[i];
      Y[i] = pby[i];
    }  

    lin_multAtB(X, X, XtX, npb, 3, npb, 3);
    ret = lin_inverseLUP(XtX, XtXi, 3);
    if (ret != 0)
    { 
      lin_multAtB(X, Y, XtY, npb, 3, npb, 1);
      lin_mult(XtXi, XtY, R, 3, 3, 1);
      *a = R[2];
      *b = -1;
      *c = R[1];
      *d = R[0];
    }
    else
    {
      for (i = 0; i < npb; i++)
      {
	X[3*i] = 1.0;
	X[3*i + 1] = pby[i];
	X[3*i + 2] = pbz[i];
	Y[i] = pbx[i];
      }  

      lin_multAtB(X, X, XtX, npb, 3, npb, 3);
      ret = lin_inverseLUP(XtX, XtXi, 3);
      if (ret != 0)
      { 
	lin_multAtB(X, Y, XtY, npb, 3, npb, 1);
	lin_mult(XtXi, XtY, R, 3, 3, 1);
	*a = -1;
	*b = R[1];
	*c = R[2];
	*d = R[0];
      }
      else
      {
	fprintf(stderr, "%s: cannot compute plane\n", F_NAME);
	return 0;
      }
    }
  }

  free(X);
  free(Y);
  free(XtX);
  free(XtXi);
  free(XtY);
  free(R);
  return 1;
} /* lidentifyplane() */
