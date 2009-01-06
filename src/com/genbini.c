/* $Id: genbini.c,v 1.3 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file genbini.c

\brief generates a binary image following interactive specifications

<B>Usage:</B> genbini rs cs ds out.pgm

<B>Description:</B>

The size of the result image <B>out.pgm</B> is given by the parameters 
<B>rs</B>, <B>cs</B>, <B>ds</B>.
The user interactively specifies the objects that are to be inserted in the result
image: points, spheres (circles), balls (discs) of given sizes and positions.

<B>Types supported:</B> byte 2d, byte 3d

<B>Category:</B> divers
\ingroup  divers

\author Michel Couprie
*/

/* Michel Couprie - mars 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <lbresen.h>
#include <larith.h>
#include <lpropgeo.h>

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image;
  struct xvimage * imagec;
  int32_t i, j, k, p;
  uint8_t *Im;
  uint8_t *Imc;
  int32_t rs, cs, ds, ps, N;
  int32_t r;
  double rayon;
  double x, y, z, zmin, zmax;
  double theta, incz, inct, ntours;
  int32_t n = 10000;
  int32_t command;

  if (argc != 5)
  {
    fprintf(stderr, "usage: %s rs cs ds out.pgm \n", argv[0]);
    exit(1);
  }

  rs = atoi(argv[1]);
  cs = atoi(argv[2]);
  ds = atoi(argv[3]);
  ps = rs * cs;
  N = ds * ps;
  image = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  imagec = allocimage(NULL, rs, cs, ds, VFF_TYP_1_BYTE);
  if ((image == NULL) || (imagec == NULL))
  {
    fprintf(stderr, "%s: allocimage failed\n", argv[0]);
    exit(1);
  }
  Im = UCHARDATA(image);
  Imc = UCHARDATA(imagec);
  for (i = 0; i < N; i++) Im[i] = 0;

  do
  {

    printf("commande (0 = quit, 1 = point, 2 = sphere/cercle, 3 = boule/disque, 4 = spirale) : \n");
    scanf("%d", &command);

    switch(command)
    {
      case 0: break;

      case 1: 
        printf("coordonnees x, y, z du point : \n");
        scanf("%d %d %d", &i, &j, &k);
        if ((i >= 0) && (j >= 0) && (k >= 0) && (i < rs) && (j < cs) && (k < ds)) 
          Im[k*ps + j*rs + i] = 255;
        else 
          printf("point hors image\n");
        break;

      case 2:
        printf("NYI \n");
	//printf("rayon : \n");
        //scanf("%d", &rayon);
        //printf("coordonnees x, y, z du centre : \n");
        //scanf("%lf %lf %lf", &x, &y, &z);
	//lellipse(Im, rs, cs, rayon, 0, 0, rayon, x, y);
        break;

      case 3:
        printf("NYI \n");
        //printf("rayon : \n");
        //scanf("%d", &rayon);
        //printf("coordonnees x, y du centre : \n");
        //scanf("%lf %lf %lf", &x, &y, &z);
        break;

      case 4:
	printf("rayon : \n");
        scanf("%lf", &rayon);
        printf("coordonnees x, y du centre : \n");
        scanf("%lf %lf", &x, &y);
        printf("zmin, zmax : \n");
        scanf("%lf %lf", &zmin, &zmax);
        printf("ntours : \n");
        scanf("%lf", &ntours);
        incz = (zmax - zmin) / n;
        inct = ((double)ntours * M_PI * 2.0) / n;
        for (z = zmin, theta = 0, p = 0; p < n; p++, z += incz, theta += inct)
        {
          i = (int32_t)(x + rayon*sin(theta));
          j = (int32_t)(y + rayon*cos(theta));
          k = (int32_t)z;
          if ((i >= 0) && (j >= 0) && (k >= 0) && (i < rs) && (j < cs) && (k < ds)) 
            Im[k*ps + j*rs + i] = 255;
        }
        break;
        

      default: printf("mauvaise commande\n");
    } /* switch */
  } while (command != 0);

  writeimage(image, argv[argc-1]);
  freeimage(image);
  freeimage(imagec);
  return 0;
} /* main */

