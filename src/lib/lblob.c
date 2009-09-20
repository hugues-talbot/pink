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
   Simulation d'objets topologiques vivants: les blobs

   Michel Couprie - mai 2000
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mcutil.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcxbib.h>
#include <mclifo.h>
#include <mctopo.h>
#include <mcprobas.h>
#include <lblob.h>

#define DSURF 30
#define DPERIM 10

/* ==================================== */
static int32_t contrib_perim(int32_t p, uint8_t *ORI, int32_t rs, int32_t N, int32_t incr_vois)
/* ==================================== */
{ /* calcule la contribution du point p au perimetre de la composante */
  int32_t q, k, nv = 0;

  for (k = 0; k < 8; k += incr_vois) /* compte le nombre nv de voisins */
  {
    q = voisin(p, k, rs, N);
    if ((q != -1) && ORI[q]) nv++;
  } /* for (k = 0; k < 8; k += incr_vois) */

  return 4 - 2 * nv;
}

/* ==================================== */
int32_t lblob(struct xvimage *image, int32_t nitermax)
/* ==================================== */
{
  register int32_t x, y;              /* index muet de pixel */
  register int32_t i, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille plan */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint8_t *EMPILE;
#define NBBUTTONS2D  7
#define NBLIGBUTT2D  2
#define BOXWIDTH    70
#define BOXHEIGHT   20
  char * ButtonStrings[NBLIGBUTT2D][NBBUTTONS2D];
  int32_t xx, yy;
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT2D;
  int32_t Width = rs;
  int32_t ImageHeight = cs;
  int32_t ImageWidth = rs;
  int32_t eventtype;
  Lifo * LIFO;
  int32_t r;
  int32_t niter;
  int32_t surface;
  int32_t perimetre;
  int32_t osurf; 
  int32_t operim;
  int32_t dsurf = DSURF;
  int32_t dperim = DPERIM;
  int32_t cp;

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lblob() : CreeLifoVide failed\n");
      return 0;
  }

  EMPILE = (uint8_t *)calloc(N, sizeof(char));
  if (EMPILE == NULL)
  {   fprintf(stderr, "lblob() : calloc failed\n");
      return 0;
  }

  /* calcul de la surface initiale */
  surface = 0;
  for (x = 0; x < N; x++) if (SOURCE[x]) surface++;
  osurf = surface;

  /* calcul du perimetre initial */
  perimetre = 0;
  for (x = 0; x < N; x++)
    if (SOURCE[x])
    {
      EMPILE[x] = 255;
      perimetre += contrib_perim(x, EMPILE, rs, N, 2);
    }  
  operim = perimetre;
  memset(EMPILE, 0, N);

printf("surface: %d ; perimetre: %d\n", surface, perimetre);

  if (BoxWidth * NBBUTTONS2D > Width) Width = BoxWidth * NBBUTTONS2D;
  
  InitGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  /* send output to the screen */
  for (x = 0; x < N; x++)
  {
    ColToBlack();
    if (SOURCE[x]) Point(x%rs, x/rs);
  }

  FlushGraphics();  

  /* empile initialement les points simples */

  for (x = 0; x < N; x++)
    if (pdestr4(SOURCE, x, rs, N) || pconstr4(SOURCE, x, rs, N))
    {
      LifoPush(LIFO, x);
      EMPILE[x] = 1;
    }

  /* ===================================================================== */
  /*                            BOUCLE PRINCIPALE                          */
  /* ===================================================================== */
  niter = 0;
  while(niter < nitermax)
  {
    if ((niter % 1000) == 0)
    {
      /* send output to the screen */
      for (x = 0; x < N; x++)
      {
        if (SOURCE[x]) ColToWhite(); else ColToBlack();
        Point(x%rs, x/rs);
      }
      FlushGraphics();  
    }
    niter++;

    /* echange l'element en haut de pile avec un element choisi aleatoirement */
    r = Random(0, LIFO->Sp-1);
    i = LIFO->Pts[LIFO->Sp-1];
    LIFO->Pts[LIFO->Sp-1] = LIFO->Pts[r];
    LIFO->Pts[r] = i;

    /* depile, change la couleur du point, et empile les points simples non empiles du voisinage */
    x = LifoPop(LIFO);
    EMPILE[x] = 0;
    /*  printf("POP x = %d,%d\n", x%rs, x/rs);
        { char dummy[64]; scanf("%s", dummy); } */
    yy = x / rs;
    if (pdestr4(SOURCE, x, rs, N) && (nbvoisc4(SOURCE, x, rs, N) > 1))
    {
      cp = contrib_perim(x, SOURCE, rs, N, 2);
      if (!((surface > osurf - dsurf) && (Prob() >= ((double)yy) / cs)))
      { LifoPush(LIFO, x); EMPILE[x] = 1; goto Skip; }
      if ( ((cp > 0) && (perimetre <= operim - dperim)) ||
           ((cp < 0) && (perimetre >= operim + dperim)) )
      { LifoPush(LIFO, x); EMPILE[x] = 1; goto Skip; }
      SOURCE[x] = 0;
      surface--;
      perimetre -= cp;
      ColToBlack();
      Point(x%rs, x/rs);
      for (k = 0; k < 8; k += 1)  /* parcourt les voisins en 8-connexite */
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (!EMPILE[y]))
        { 
          LifoPush(LIFO, y);
          EMPILE[y] = 1;
        }
      } /* for k */
      LifoPush(LIFO, x);
      EMPILE[x] = 1;
    }
    else if (pconstr4(SOURCE, x, rs, N) && (nbvois8(SOURCE, x, rs, N) > 1))
    {
      cp = contrib_perim(x, SOURCE, rs, N, 2);
      if (!((surface < osurf + dsurf) && (Prob() < ((double)yy) / cs)))
      { LifoPush(LIFO, x); EMPILE[x] = 1; goto Skip; }
      if ( ((cp < 0) && (perimetre <= operim - dperim)) ||
           ((cp > 0) && (perimetre >= operim + dperim)) )
      { LifoPush(LIFO, x); EMPILE[x] = 1; goto Skip; }

      SOURCE[x] = 255;
      surface++;
      perimetre += cp;
      ColToWhite();
      Point(x%rs, x/rs);
      for (k = 0; k < 8; k += 1)  /* parcourt les voisins en 8-connexite */
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (!EMPILE[y]))
        { 
          LifoPush(LIFO, y);
          EMPILE[y] = 1;
        }
      } /* for k */
      LifoPush(LIFO, x);
      EMPILE[x] = 1;
    }
Skip:;
  }

  free(EMPILE);
  return(1);
} /* lblob() */
