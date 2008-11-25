/* $Id: lconvexhull.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* enveloppe convexe d'un objet binaire */
/* Michel Couprie - avril 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <lconvexhull.h>

/*
#define DEBUG
*/

/* ==================================== */
int32_t angleinf(int32_t p, int32_t p1, int32_t p2, int32_t rs)
/* ==================================== */
/* retourne 1 si l'angle polaire p-p1 est inferieur a l'angle polaire p-p2 
   ou si, a angle egal, p1 est plus loin de p que p2 */
{
  int32_t x1 = (p1 % rs) - (p % rs);
  int32_t y1 = (p1 / rs) - (p / rs);
  int32_t x2 = (p2 % rs) - (p % rs);
  int32_t y2 = (p2 / rs) - (p / rs);
  int32_t d = x1 * y2 - x2 * y1;
  if (d > 0) return 1;
  if (d < 0) return 0;
  return ((x1 * x1 + y1 * y1) > (x2 * x2 + y2 * y2));
} /* angleinf() */

/* ==================================== */
int32_t angleinfxyneg(int32_t p, int32_t p1, int32_t p2, int32_t rs)
/* ==================================== */
/* retourne 1 si l'angle polaire p-p1 est inferieur a l'angle polaire p-p2 
   (par rapport aux x et aux y negatifs)
   ou si, a angle egal, p1 est plus loin de p que p2 */
{
  int32_t x1 = (p % rs) - (p1 % rs);
  int32_t y1 = (p / rs) - (p1 / rs);
  int32_t x2 = (p % rs) - (p2 % rs);
  int32_t y2 = (p / rs) - (p2 / rs);
  int32_t d = x1 * y2 - x2 * y1;
  if (d > 0) return 1;
  if (d < 0) return 0;
  return ((x1 * x1 + y1 * y1) > (x2 * x2 + y2 * y2));
} /* angleinfxyneg() */

/* ==================================== */
int32_t lconvexhull(struct xvimage * image1, int32_t ** listepoints, int32_t * npoints)
/* ==================================== */
{
  int32_t i, j, x, y;
  uint8_t *F;
  int32_t rs, cs, N;
  int32_t *P;             /* la liste des points de l'objet */
  int32_t n;              /* le nombre de points de l'objet */
  int32_t p0, yp0, ip0;   /* le point initial, son ordonnee et son index dans la liste P*/
  int32_t pk, ypk, ipk;   /* le point intermediaire, son ordonnee et son index */
  int32_t p, ip;          /* point courant */
  int32_t ps, ips;        /* point suivant */
  int32_t ps2, ips2;      /* point suivant aux */
  int32_t *Ph;            /* la liste des points de l'enveloppe */
  int32_t nh;             /* le nombre de points de l'enveloppe */

  if (depth(image1) != 1) 
  {
    fprintf(stderr, "lconvexhull: cette version ne traite pas les images volumiques\n");
    exit(0);
  }
  rs = image1->row_size;
  cs = image1->col_size;
  N = rs * cs;
  F = UCHARDATA(image1);
  
  P = (int32_t *)calloc(1,N * sizeof(int32_t));
  if (P == NULL)
  {   fprintf(stderr, "lconvexhull() : malloc failed for P\n");
      return(0); 
  }

  Ph = (int32_t *)calloc(1,N * sizeof(int32_t));
  if (Ph == NULL)
  {   fprintf(stderr, "lconvexhull() : malloc failed for Ph\n");
      return(0);
  }

  /* construit la liste p des points de l'objet */
  /* et trouve le point p0 d'ordonnee minimale */
  /* (si plusieurs solutions, on retient celui d'abcisse maximale) */
  /* et le point pk d'ordonnee maximale */
  /* (si plusieurs solutions, on retient celui d'abcisse minimale) */

  n = 0;
  yp0 = N;
  ypk = 0;
  for (i = 0; i < N; i++)
  {
    if (F[i]) 
    {
      P[n] = i;
      y = i / rs;
      if (y <= yp0) { ip0 = n; yp0 = y; }
      if (y > ypk) { ipk = n; ypk = y; }
      n++;
    } 
  }

#ifdef DEBUG
printf("p0 = %d,%d ; pk = %d,%d\n", P[ip0]%rs, P[ip0]/rs, P[ipk]%rs, P[ipk]/rs);
for (i = 0; i < n; i++) printf("P[i] = %d,%d\n", P[i]%rs, P[i]/rs);
#endif

  /* ---------------------------------------------------------- */
  /* construction de la chaine "droite" */
  /* ---------------------------------------------------------- */

  Ph[0] = p = P[ip0];
  nh = 1;
  pk = P[ipk];
  while (p != pk)
  {
    /* trouve le point suivant, qui minimise l'angle polaire par rapport a p */
    /* en cas d'egalite, on retient le plus eloigne de p */
    ips = 0;
    while ((P[ips] == -1) && (ips < n)) ips ++;  /* 1er candidat */
    for (ips2 = ips + 1; ips2 < n; ips2++)
    {                        /* recherche de meilleurs candidats */
      if ((P[ips2] != -1) && angleinf(p, P[ips2], P[ips], rs)) ips = ips2; 
    }
    Ph[nh] = p = P[ips];
    nh++;
    P[ips] = -1;
  } /* while (p != pk) */

  /* ---------------------------------------------------------- */
  /* construction de la chaine "gauche" */
  /* ---------------------------------------------------------- */

  p0 = P[ip0];
  while (p != p0)
  {
    /* trouve le point suivant, qui minimise l'angle polaire par rapport a p */
    /* par rapport aux x et y negatifs */
    /* en cas d'egalite, on retient le plus eloigne de p */
    ips = 0;
    while ((P[ips] == -1) && (ips < n)) ips ++;  /* 1er candidat */
    for (ips2 = ips + 1; ips2 < n; ips2++)
    {                        /* recherche de meilleurs candidats */
      if ((P[ips2] != -1) && angleinfxyneg(p, P[ips2], P[ips], rs)) ips = ips2; 
    }
    Ph[nh] = p = P[ips];
    nh++;
    P[ips] = -1;
  } /* while (p != p0) */
  nh--;                    /* on a compte p0 deux fois */
#ifdef DEBUG
for (i = 0; i < nh; i++)
  printf("Ph[i] = %d,%d\n", Ph[i]%rs, Ph[i]/rs);
#endif

  free(P);

  /* ---------------------------------------------------------- */
  /* liste des points de l'env. conv. */
  /* ---------------------------------------------------------- */
  
  *listepoints = (int32_t *)calloc(1,nh * sizeof(int32_t));
  if (*listepoints == NULL)
  {   fprintf(stderr, "lconvexhull() : malloc failed for *listepoints\n");
      return(0); 
  }
  for (i = 0; i < nh; i++)
    (*listepoints)[i] = Ph[i];
  *npoints = nh;

  /* ---------------------------------------------------------- */
  /* image de l'env. conv. */
  /* ---------------------------------------------------------- */

  for (i = 0; i < N; i++) F[i] = 0;
  for (i = 0; i < nh; i++) F[Ph[i]] = 255;

  free(Ph);
  return 1;
}







