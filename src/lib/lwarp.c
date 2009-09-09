/* $Id: lwarp.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* deformation affine lineaire */
/* Michel Couprie - mars 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcgeo.h>
#include <lwarp.h>

/*
#define DEBUGWARP
*/

int32_t lwarp(
  struct xvimage *in, 
  triangle *ts, 
  triangle *td, 
  int32_t noclip)
/* in : operande et resultat */
/* ts : triangle source */
/* td : triangle destination */
/* noclip : booleen */
{
  int32_t x, y;
  uint8_t *F;
  uint8_t *H;                     /* image de travail */
  int32_t rs, cs, N;
  mat33 m;                              /* matrice de travail */
  mat33 a;                              /* transformation affine dest -> source */
  vec3 b;
  vec3 sol;
  double X, Y;

  rs = in->row_size;
  cs = in->col_size;
  N = rs * cs;
  F = UCHARDATA(in);

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"lwarp() : malloc failed for H\n");
      return(0);
  }
  for (x = 0; x < N; x++) H[x] = F[x];
  for (x = 0; x < N; x++) F[x] = 0;

  if (inittriangle(ts) == 0)
  {
    printf("mauvais triangle source\n");
    return(0);
  }
#ifdef DEBUGWARP
  printtriangle(ts);
#endif

  if (inittriangle(td) == 0)
  {
    printf("mauvais triangle destination\n");
    return(0);
  }
#ifdef DEBUGWARP
  printtriangle(td);
#endif

  /* ------------------------------------------------------------------ */
  /* identification des parametres de la transformation affine lineaire */
  /* ------------------------------------------------------------------ */

  m[0][0] = td->xa;     m[0][1] = td->ya;     m[0][2] = 1.0;
  m[1][0] = td->xb;     m[1][1] = td->yb;     m[1][2] = 1.0;
  m[2][0] = td->xc;     m[2][1] = td->yc;     m[2][2] = 1.0;

  b[0] = ts->xa;
  b[1] = ts->xb;
  b[2] = ts->xc;
  
  if (solsyst3(m, b, sol) == 0)
    printf("determinant nul\n");

  a[0][0] = sol[0];     a[0][1] = sol[1];     a[0][2] = sol[2];

  b[0] = ts->ya;
  b[1] = ts->yb;
  b[2] = ts->yc;
  
  if (solsyst3(m, b, sol) == 0)
    printf("determinant nul\n");

  a[1][0] = sol[0];     a[1][1] = sol[1];     a[1][2] = sol[2];
  a[2][0] = 0.0;        a[2][1] = 0.0;        a[2][2] = 1.0;

#ifdef DEBUGWARP
  printf("transformation affine lineaire:\n");
  printmat(a, 3, 3);
#endif

  /* ------------------------------------------------------------- */
  /* calcul des intensites des points dans le triangle destination */
  /* ------------------------------------------------------------- */

  if (noclip)
  {
    for (x = 0; x < rs; x++)
      for (y = 0; y < cs; y++)
      {
        b[0] = (double)x;
        b[1] = (double)y;
        b[2] = 1.0;
        multmat3vec3(a, b, sol);
        X = sol[0];
        Y = sol[1];
        if ((X >= 0) && ((int32_t)X < rs) && (Y >= 0) && ((int32_t)Y < cs))
	{
          F[y * rs + x] = H[((int32_t)Y) * rs + (int32_t)X];
        }
      }
  } /* if (noclip) */
  else
  {
    for (x = (int32_t)floor(td->xmin); x <= (int32_t)ceil(td->xmax); x++)
      for (y = (int32_t)floor(td->ymin); y <= (int32_t)ceil(td->ymax); y++)
        if (danstriangle(td, (double)x, (double)y))
        {
          b[0] = (double)x;
          b[1] = (double)y;
          b[2] = 1.0;
          multmat3vec3(a, b, sol);
          X = sol[0];
          Y = sol[1];
          if ((X >= 0) && ((int32_t)X < rs) && (Y >= 0) && ((int32_t)Y < cs))
            F[y * rs + x] = H[((int32_t)Y) * rs + (int32_t)X];
        }
  } /* else if (noclip) */
  return 1;
}

