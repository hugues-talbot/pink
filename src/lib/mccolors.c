/* $Id: mccolors.c,v 1.2 2009-01-06 13:18:15 mcouprie Exp $ */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccolors.h>

/* rgb2hls.c - construct 3 pgm from 1 ppm file:
** - basename.H : Hue (int32_t - 0 to 359)
** - basename.L : Luminance (byte)
** - basename.S : Saturation (byte)
**
** Michel Couprie  -  avril 1999
**
** Based on Foley, Van Damm & al: "Computer Graphics", 2nd ed., p. 595
**
** Quelques valeurs de HUE:
**     0  rouge
**    60  jaune
**   120  vert
**   180  cyan
**   240  bleu
**   300  magenta
*/

/* ======================================================================== */
char * colorname(double h)
/* ======================================================================== */
{
  if ((h < 30.0) || (h >= 330.0)) return (char *)CN_RED;
  if ((h >= 30.0) && (h < 90.0)) return (char *)CN_YELLOW;
  if ((h >= 90.0) && (h < 150.0)) return (char *)CN_GREEN;
  if ((h >= 150.0) && (h < 210.0)) return (char *)CN_CYAN;
  if ((h >= 210.0) && (h < 270.0)) return (char *)CN_BLUE;
  if ((h >= 270.0) && (h < 330.0)) return (char *)CN_MAGENTA;
}

/* ======================================================================== */
char * nomcouleur(double h)
/* ======================================================================== */
{
  if ((h < 30.0) || (h >= 330.0)) return (char *)CN_ROUGE;
  if ((h >= 30.0) && (h < 90.0)) return (char *)CN_JAUNE;
  if ((h >= 90.0) && (h < 150.0)) return (char *)CN_VERT;
  if ((h >= 150.0) && (h < 210.0)) return (char *)CN_CYAN;
  if ((h >= 210.0) && (h < 270.0)) return (char *)CN_BLEU;
  if ((h >= 270.0) && (h < 330.0)) return (char *)CN_MAGENTA;
}

/* ======================================================================== */
void rgb2hls(double r, double g, double b, double *h, double *l, double *s)
/* ======================================================================== */
/*
  Given: r, g, b each in [0,1].
  Desired: h in [0,360[, l and s in [0,1]. If s = 0, then h = UNDEFINED = 0.
*/
#define UNDEFINED 0.0
{
  double maxi = max(max(r,g),b); 
  double mini = min(min(r,g),b); 
  double delta;

  *l = (maxi + mini) / 2.0; /* lightness */
  if (maxi == mini)
  {                   /* achromatic case */
    *s = 0.0; *h = UNDEFINED;
  }
  else
  {                   /* chromatic case */
    delta = maxi - mini;
    if (*l <= 0.5)
      *s = delta / (maxi + mini);
    else
      *s = delta / (2 - maxi - mini);
    if (r == maxi) *h = (g - b) / delta;
    else if (g == maxi) *h = 2.0 + (b - r) / delta;
    else if (b == maxi) *h = 4.0 + (r - g) / delta;
    *h = *h * 60.0;
    if (*h < 0.0) *h = *h + 360.0;
  }
}

/* hls2rgb.c - construct 1 ppm from 3 pgm files:
** - basename.H : Hue (int32_t - 0 to 359)
** - basename.L : Luminance (byte)
** - basename.S : Saturation (byte)
**
** Michel Couprie  -  avril 1999
**
** Based on Foley, Van Damm & al: "Computer Graphics", 2nd ed., p. 595
**
** Quelques valeurs de HUE:
**     0  rouge
**    60  jaune
**   120  vert
**   180  cyan
**   240  bleu
**   300  magenta
*/

/* ======================================================================== */
static double value(double n1, double n2, double hue)
/* ======================================================================== */
{
  if (hue >= 360.0) hue -= 360.0;
  else if (hue < 0.0) hue += 360.0;
  if (hue < 60.0) return n1 + (n2 - n1) * hue / 60.0;
  if (hue < 180.0) return n2;
  if (hue < 240.0) return n1 + (n2 - n1) * (240 - hue) / 60.0;
  return n1;
}

/* ======================================================================== */
void hls2rgb(double *r, double *g, double *b, double h, double l, double s)
/* ======================================================================== */
/*
  Given: h in [0,360[, l and s in [0,1]. If s = 0, then h = UNDEFINED = 0.
  Desired: r, g, b each in [0,1].
*/
#define UNDEFINED 0.0
{
  double m1, m2;
  if (l <= 0.5)
    m2 = l * (1.0 + s);
  else
    m2 = l + s - l * s;
  m1 = 2 * l - m2;
  if (s == 0)
    *r = *g = *b = l;
  else
  {
    *r = value(m1, m2, h + 120.0);
    *g = value(m1, m2, h);
    *b = value(m1, m2, h - 120.0);
  }
}

/* ======================================================================== */
int32_t huedistance(int32_t h1, int32_t h2)
/* ======================================================================== */
{
  int32_t k, h = h1 - h2;
  h = abs(h);
  k = 360 - h;
  h = min(h,k);
  return h;
} //huedistance()
