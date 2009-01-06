/* $Id: gradcol.c,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
/* \file gradcol.c

\brief color morphological gradient

<B>Usage:</B> gradcol in.pgm es out.pgm

<B>Description:</B> IMPLEMENTATION NON TERMINEE 

<B>Types supported:</B> byte color 2D

<B>Category:</B> morpho
\ingroup  morpho

\author Michel Couprie
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mccolor.h>

/* ==================================== */
int32_t lgradcol(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc)
/* gradient morphologique couleur par un element structurant plan de taille quelconque */
/* Michel Couprie - juin 2003 */
/* m : masque representant l'element structurant */
/* xc, yc : coordonnees du "centre" de l'element structurant */
/* ==================================== */
#undef F_NAME
#define F_NAME "lgradcol"
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;                    /* index muet */
  register int32_t k, l;                    /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t Nm = rsm * csm;
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  uint8_t sup;
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t c;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "%s: not for 3d images\n", F_NAME);
    return 0;
  }

  H = (uint8_t *)calloc(1, N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"%s() : malloc failed for H\n", F_NAME);
     return(0);
  }

  memcpy(H, F, N);

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL))
  {  
     fprintf(stderr,"%s() : malloc failed for tab_es\n", F_NAME);
     return(0);
  }

  k = 0;
  for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
      if (M[j * rsm + i])
      {
         tab_es_x[k] = rsm - 1 - i;  /* symetrique de l'e.s. */
         tab_es_y[k] = csm - 1 - j;
         k += 1;
      }
  xc = rsm - 1 - xc;
  yc = csm - 1 - yc;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    sup = NDG_MIN;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_es_y[c] - yc;
      k = x + tab_es_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs) && (H[l * rs + k] > sup))
	sup = H[l * rs + k];
    }
    F[y * rs + x] = sup;
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  return 1;
} /* lgradcol() */

/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  struct xvimage * image1;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s in1.ppm es out.pgm \n", 
            argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);  
  if (image1 == NULL)
  {
    fprintf(stderr, "gradcol: readimage failed\n");
    exit(1);
  }
  alpha = atof(argv[2]);

  if (! lgradcol(image1, , dummy))
  {
    fprintf(stderr, "gradcol: function lgradcol failed\n");
    exit(1);
  }

  writeimage(image1, argv[argc-1]);
  freeimage(image1);

  return 0;
} /* main */


