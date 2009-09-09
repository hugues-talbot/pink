/* $Id: lnbtopo.c,v 1.1.1.1 2008-11-25 08:01:40 mcouprie Exp $ */
/* nombres de connexite en niveaux de gris */
/* Michel Couprie - juin 1998 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mctopo.h>
#include <mctopo3d.h>
#include <lnbtopo.h>

/* ==================================== */
int32_t lnbtopo(
  struct xvimage * image,
  int32_t connex,      /* 4, 8 (2D), 6, 26 (3D) */
  int32_t whichnumber) /* PP, P, M, MM */
/* ==================================== */
#undef F_NAME
#define F_NAME "lnbtopo"
{
  int32_t p;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t ds = depth(image);       /* nb plans */
  int32_t ps = rs * cs;            /* taille plans */
  int32_t N = ps * ds;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *I;                         /* image temporaire */

  if ((whichnumber != PP) && (whichnumber != P) && (whichnumber != M) && (whichnumber != MM))
  {
     fprintf(stderr, "%s: erreur parametre whichnumber: %d\n", F_NAME, whichnumber);
     return 0;
  }

  I = (uint8_t *)calloc(1,N * sizeof(char));
  if (I == NULL)
  {   fprintf(stderr, "%s: malloc failed\n", F_NAME);
      return(0);
  }
  memcpy(I, F, N); /* copie F dans I */
  memset(F, 0, N); /* met F a 0 (pour les bords) */

  /* ---------------------------------------------------------- */
  /* calcul du resultat */
  /* ---------------------------------------------------------- */

  switch (connex)
  {
    case 4:
      switch (whichnumber)
      {
        case PP: for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t4pp(I, p, rs, N); break;
        case P:  for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t4p(I, p, rs, N); break;
        case M:  for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t4m(I, p, rs, N); break;
        case MM: for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t4mm(I, p, rs, N); break;
      }
      break;
    case 8:
      switch (whichnumber)
      {
        case PP: for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t8pp(I, p, rs, N); break;
        case P:  for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t8p(I, p, rs, N);  break;
        case M:  for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t8m(I, p, rs, N);  break;
        case MM: for (p = 0; p < N; p++) if (nonbord(p, rs, N)) F[p] = (uint8_t)t8mm(I, p, rs, N); break;
      }
      break;
    case 6:
      init_topo3d();
      switch (whichnumber)
      {
        case PP: for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t6pp(I, p, rs, ps, N); break;
        case P:  for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t6p(I, p, rs, ps, N);  break;
        case M:  for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t6m(I, p, rs, ps, N);  break;
        case MM: for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t6mm(I, p, rs, ps, N); break;
      }
      termine_topo3d();
      break;
    case 26:
      init_topo3d();
      switch (whichnumber)
      {
        case PP: for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t26pp(I, p, rs, ps, N); break;
        case P:  for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t26p(I, p, rs, ps, N);  break;
        case M:  for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t26m(I, p, rs, ps, N);  break;
        case MM: for (p = 0; p < N; p++) if (nonbord3d(p, rs, ps, N)) F[p] = (uint8_t)t26mm(I, p, rs, ps, N); break;
      }
      break;
      termine_topo3d();
    default: 
      fprintf(stderr, "%s: bad value for connex: %d\n", F_NAME, connex);
      return 0;
  } /* switch (connex) */
  
  free(I);
  return 1;
}
