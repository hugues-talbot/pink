/* $Id: lcorrel.c,v 1.1.1.1 2008-11-25 08:01:42 mcouprie Exp $ */
/* operateur de convolution et de correlation */
/* algo. naif en o(n2) */
/* definitions (en 1D) : 
   soient f[0..N-1], g[0..M-1]
   f*g[i] = sum f[i] g[j - i]
             j

   corr(f,g)[i] = sum f[i] g[i - j]
                   j

*/
/* Michel Couprie - avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcutil.h>

/* ==================================== */
int32_t lconvol(struct xvimage *f, struct xvimage *g, struct xvimage **h)
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;           /* index muet */
  register int32_t k, l;           /* index muet */
  int32_t rsf = rowsize(f);        /* taille ligne f */
  int32_t csf = colsize(f);        /* taille colonne f */
  int32_t Nf = rsf * csf;          /* taille image f */
  int32_t rsg = rowsize(g);        /* taille ligne g */
  int32_t csg = colsize(g);        /* taille colonne g */
  int32_t Ng = rsg * csg;          /* taille image g */
  uint8_t *F = UCHARDATA(f);
  uint8_t *G = UCHARDATA(g);
  uint8_t *H;            /* image de travail */

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lconvol: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  *h = allocimage(NULL, rsf + rsg - 1, csf + csg - 1, 1, VFF_TYP_1_BYTE);
  if (*h == NULL)
  {  
     fprintf(stderr,"lconvol() : allocimage failed\n");
     return(0);
  }
  H = UCHARDATA(*h);












  for (x = 0; x < N; x++) H[x] = F[x];

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_m_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_m_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_m_val = (uint8_t *)calloc(1,nptb * sizeof(char));
  if ((tab_m_x == NULL) || (tab_m_y == NULL) || (tab_m_val == NULL))
  {  
     fprintf(stderr,"lconvol() : malloc failed for tab_m\n");
     return(0);
  }

  k = 0;
  for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
      if (M[j * rsm + i])
      {
         tab_m_x[k] = i;
         tab_m_y[k] = j;
         tab_m_val[k] = M[j * rsm + i];
         k += 1;
      }

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    nb = 0;
    sum = 0;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_m_y[c] - yc;
      k = x + tab_m_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs))
      {
        sum += H[l * rs + k];
        nb += 1;
      }
    }
    if (nb > 0) F[y * rs + x] = (uint8_t)(sum / nb);
    else        F[y * rs + x] = 0;
  }

  free(H);
  free(tab_m_x);
  free(tab_m_y);
  return 1;
}
