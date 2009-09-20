/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
/* 
   Operateur de calcul du carre du coefficient de variation du niveau de gris
   dans le voisinage d'un point

   Operateur de calcul de l'ecart-type du niveau de gris
   dans le voisinage d'un point

   Calcul a partir des moments d'ordre 0, 1 et 2:

     moyenne = m1 / m0
     variance = (m2 / m0) - (m1*m1 / (m0*m0))
     ecart-type = sqrt(variance)
     ccv = variance / (moyenne * moyenne)

   Michel Couprie - avril 1998 
 
   Modif: decembre 1999 (ajout ecart-type)
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <lccv.h>

/* ==================================== */
int32_t lccv(struct xvimage *f, struct xvimage *m)
/* m : masque representant le voisinage */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;           /* index muet */
  register int32_t k, l;           /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t Nm = rsm * csm;          /* taille masque */
  int32_t xc = rsm/2, yc = csm/2;  /* coordonnees du "centre" du masque */
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t c;
  double m0, m1, m2, M1;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lccv: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"lccv() : malloc failed for H\n");
     return(0);
  }

  for (x = 0; x < N; x++) H[x] = F[x];

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL))
  {  
     fprintf(stderr,"lccv() : malloc failed for tab_es\n");
     return(0);
  }

  k = 0;
  for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
      if (M[j * rsm + i])
      {
         tab_es_x[k] = i;
         tab_es_y[k] = j;
         k += 1;
      }

  /* calcul du ccv en chaque point */

  m0 = (double)nptb;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    m1 = m2 = 0.0;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_es_y[c] - yc;
      k = x + tab_es_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs))
      {
        M1 = (double)H[l * rs + k]; 
        m1 += M1;
        m2 += M1 * M1;
      }
    }
    F[y * rs + x] = (uint8_t)(100 * ((m2 * m0 / (m1 * m1)) - 1.0));
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  return 1;
} /* lccv() */

/* ==================================== */
int32_t lecarttype(struct xvimage *f, struct xvimage *m)
/* m : masque representant le voisinage */
/* ==================================== */
{
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t i, j;           /* index muet */
  register int32_t k, l;           /* index muet */
  int32_t rs = rowsize(f);         /* taille ligne */
  int32_t cs = colsize(f);         /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t rsm = rowsize(m);        /* taille ligne masque */
  int32_t csm = colsize(m);        /* taille colonne masque */
  int32_t Nm = rsm * csm;          /* taille masque */
  int32_t xc = rsm/2, yc = csm/2;  /* coordonnees du "centre" du masque */
  uint8_t *M = UCHARDATA(m);
  uint8_t *F = UCHARDATA(f);
  uint8_t *H;                    /* image de travail */
  int32_t nptb;                    /* nombre de points de l'e.s. */
  int32_t *tab_es_x;               /* liste des coord. x des points de l'e.s. */
  int32_t *tab_es_y;               /* liste des coord. y des points de l'e.s. */
  int32_t c;
  double m0, m1, m2, M1;

  if (depth(f) != 1) 
  {
    fprintf(stderr, "lecarttype: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  H = (uint8_t *)calloc(1,N*sizeof(char));
  if (H == NULL)
  {  
     fprintf(stderr,"lecarttype() : malloc failed for H\n");
     return(0);
  }

  for (x = 0; x < N; x++) H[x] = F[x];

  nptb = 0;
  for (i = 0; i < Nm; i += 1)
    if (M[i])
      nptb += 1;

  tab_es_x = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  tab_es_y = (int32_t *)calloc(1,nptb * sizeof(int32_t));
  if ((tab_es_x == NULL) || (tab_es_y == NULL))
  {  
     fprintf(stderr,"lecarttype() : malloc failed for tab_es\n");
     return(0);
  }

  k = 0;
  for (j = 0; j < csm; j += 1)
    for (i = 0; i < rsm; i += 1)
      if (M[j * rsm + i])
      {
         tab_es_x[k] = i;
         tab_es_y[k] = j;
         k += 1;
      }

  /* calcul de l'ecart-type en chaque point */

  m0 = (double)nptb;

  for (y = 0; y < cs; y++)
  for (x = 0; x < rs; x++)
  {
    m1 = m2 = 0.0;
    for (c = 0; c < nptb ; c += 1)
    {
      l = y + tab_es_y[c] - yc;
      k = x + tab_es_x[c] - xc; 
      if ((l >= 0) && (l < cs) && (k >= 0) && (k < rs))
      {
        M1 = (double)H[l * rs + k]; 
        m1 += M1;
        m2 += M1 * M1;
      }
    }
    F[y * rs + x] = (uint8_t)(sqrt((m2 / m0) - (m1 * m1) / (m0 * m0)));
  }

  free(H);
  free(tab_es_x);
  free(tab_es_y);
  return 1;
} /* lecarttype() */



