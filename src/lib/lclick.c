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
   Operateur interactif de visualisation 2D et 3D
   et de specification de marqueurs

   Michel Couprie - decembre 1999

   Versions definies dans ce fichier:

     lclick2d : genere un tableau image labels au format pgm (int32_t) - permet des labels differents
     lclicklist : genere une liste de points values
     lclick3d : idem lclicklist, avec 3 vues 
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
#include <lclick.h>

/* ==================================== */
int32_t lclick2d(struct xvimage *image, struct xvimage *lab, int32_t zoom)
/* ==================================== */
{
  register int32_t x;              /* index muet de pixel */
  int32_t i, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t n = rs * cs;             /* taille plan */
  int32_t d = depth(image);        /* nb. plans */
  int32_t N = d * n;               /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint32_t *LAB = SLONGDATA(lab);
  int32_t curlab = 1;
  int32_t incr = 0;
  int32_t iplan = 0;
  int32_t incrplan = 1;
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

  if (zoom)
  {
    Height += cs;
    Width += rs;
    ImageHeight += cs;
    ImageWidth += rs;
  }

  if (BoxWidth * NBBUTTONS2D > Width) Width = BoxWidth * NBBUTTONS2D;
  
  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT2D; i++) 
    for (k = 0; k < NBBUTTONS2D; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], "LABEL");
  sprintf(ButtonStrings[0][1], "%d", curlab);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], "INCR ");
  strcpy(ButtonStrings[0][5], "UNDO ");
  strcpy(ButtonStrings[0][6], "QUIT ");

  strcpy(ButtonStrings[1][0], " PLAN");
  sprintf(ButtonStrings[1][1], "%d", iplan);
  sprintf(ButtonStrings[1][2], " +%d", incrplan);
  sprintf(ButtonStrings[1][3], " -%d", incrplan);
  strcpy(ButtonStrings[1][4], "  ++ ");
  strcpy(ButtonStrings[1][5], "  -- ");
  strcpy(ButtonStrings[1][6], "     ");

/* ===========================================================
    affichage : 
   ===========================================================

    <------- rs ------>     
    ------------------- .
    |                 | .
    |                 | .
    |                 | .
    |      image      | cs
    |                 | .
    |                 | .
    |                 | .
    --------------------------------------------------
    |LABEL | 111  |  +   |  -   | AUTO | UNDO | QUIT |        BoxHeight
    --------------------------------------------------
    | PLAN | 111  | +1   | -1   |  ++  |  --  |      |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  /* send output to the screen */
  for (x = iplan * n; x < (iplan+1)*n; x++)
  {
    if (LAB[x])    
      Color(MC_RED);
    else
      Color(SOURCE[x]/4);
    if (zoom)
    {
      Point(2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
    }
    else
      Point((x%n)%rs, (x%n)/rs);
  }

ReDisplay:

  ColToWhite();
  FRectangle(0, ImageHeight, BoxWidth * NBBUTTONS2D, BoxHeight * NBLIGBUTT2D);
  ColToBlack();

  for (i = 0; i < NBLIGBUTT2D; i++) 
  {
    Line(0, ImageHeight + i * BoxHeight, Width, ImageHeight + i * BoxHeight);
    for (k = 0; k < NBBUTTONS2D; k++) 
    {
      Line(k * BoxWidth, ImageHeight + i*BoxHeight, k*BoxWidth, ImageHeight + (i+1)*BoxHeight);
      String(k * BoxWidth + 3, ImageHeight + (i+1) * BoxHeight - 2, ButtonStrings[i][k]);
    }
  }

  FlushGraphics();  

NextEvent:
  eventtype = WaitEvent(&xx, &yy);
  switch(eventtype)
  {
    case KeyPress: 
      switch(xx)
      {
        case key_q: goto Terminate;
      }
      break;
    
    case ButtonPress:
      break;

    case ButtonRelease: goto NextEvent;

    case EnterNotify: goto ReDisplay;

    default: printf("type : %d ; x = %d ; y = %d\n", eventtype, xx, yy); break;

  } /* switch(eventtype) */

  if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight))
  {
    if (zoom)
    {
      xx /= 2;
      yy /= 2;
    }
    x = iplan * n + yy * rs + xx;

    if (LAB[x]) 
    {
      LAB[x] = 0;
    }
    else
    {
      LAB[x] = curlab;
      if (incr) 
      {
        curlab++;
        sprintf(ButtonStrings[0][1], "%d", curlab);
      }
    }
    if (LAB[x])    
      Color(MC_RED);
    else
      Color(SOURCE[x]/4);
    if (zoom)
    {
      Point(2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
    }
    else
      Point((x%n)%rs, (x%n)/rs);
    goto ReDisplay;
  }

  xx /= BoxWidth;
  yy -= ImageHeight;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: curlab++;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 3: curlab--;
                      if (curlab < 1) curlab = 1;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 4: if (incr) 
                      {
                        incr = 0; 
                        sprintf(ButtonStrings[0][4], "%s", "INCR ");
		      }
                      else
		      { 
                        incr = 1;
                        sprintf(ButtonStrings[0][4], "%s", "IDEM ");
		      }
                      break;
              case 5: for (x = 0; x < N; x++) LAB[x] = 0;
                      goto ReDisplayImage;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
    case 1: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: iplan++;
                      if (iplan > d-1) iplan = d-1;
                      sprintf(ButtonStrings[1][1], "%d", iplan);
                      goto ReDisplayImage;
              case 3: iplan--;
                      if (iplan < 0) iplan = 0;
                      sprintf(ButtonStrings[1][1], "%d", iplan);
                      goto ReDisplayImage;
              case 4: if (incrplan == 1) incrplan = 5; else incrplan += 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplayImage;
              case 5: if (incrplan == 5) incrplan = 1; else if (incrplan > 5) incrplan -= 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplayImage;
              case 6: break;
            } /* switch(xx) */
            break;
    default: goto ReDisplayImage;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();
  return(1);
} /* lclick2d() */

/* ==================================== */
int32_t lclicklist(struct xvimage *image, uint32_t *LAB, int32_t *VLAB, int32_t *nlabs, int32_t zoom)
/* ==================================== */
{
  register int32_t x;              /* index muet de pixel */
  int32_t i, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t n = rs * cs;             /* taille plan */
  int32_t d = depth(image);        /* nb. plans */
  int32_t N = d * n;               /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  int32_t curlab = 1;
  int32_t incr = 0;
  int32_t iplan = 0;
  int32_t incrplan = 1;
#define NBBUTTONS 7
#define NBLIGBUTT 2
#define BOXWIDTH 70
#define BOXHEIGHT 20
  char * ButtonStrings[NBLIGBUTT][NBBUTTONS];
  int32_t xx, yy;
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT;
  int32_t Width = rs;
  int32_t ImageHeight = cs;
  int32_t ImageWidth = rs;
  int32_t eventtype;

  *nlabs = 0;

  if (zoom)
  {
    Height += cs;
    Width += rs;
    ImageHeight += cs;
    ImageWidth += rs;
  }

  if (BoxWidth * NBBUTTONS > Width) Width = BoxWidth * NBBUTTONS;
  
  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], "LABEL");
  sprintf(ButtonStrings[0][1], "%d", curlab);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], "INCR ");
  strcpy(ButtonStrings[0][5], "UNDO ");
  strcpy(ButtonStrings[0][6], "QUIT ");

  strcpy(ButtonStrings[1][0], " PLAN");
  sprintf(ButtonStrings[1][1], "%d", iplan);
  sprintf(ButtonStrings[1][2], " +%d", incrplan);
  sprintf(ButtonStrings[1][3], " -%d", incrplan);
  strcpy(ButtonStrings[1][4], "  ++ ");
  strcpy(ButtonStrings[1][5], "  -- ");
  strcpy(ButtonStrings[1][6], "     ");

/* ===========================================================
    affichage : 
   ===========================================================

    <------- rs ------>     
    ------------------- .
    |                 | .
    |                 | .
    |                 | .
    |      image      | cs
    |                 | .
    |                 | .
    |                 | .
    --------------------------------------------------
    |LABEL | 111  |  +   |  -   | AUTO | UNDO | QUIT |        BoxHeight
    --------------------------------------------------
    | PLAN | 111  | +1   | -1   |  ++  |  --  |      |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  /* send output to the screen */
  for (x = iplan * n; x < (iplan+1)*n; x++)
  {
    for (i = 0; i < *nlabs; i++) /* devient lourd s'il y a beaucoup de points ... */
      if (LAB[i] == x) break;
    if (i < *nlabs)
      Color(MC_RED);
    else
      Color(SOURCE[x]/4);
    if (zoom)
    {
      Point(2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
    }
    else
      Point((x%n)%rs, (x%n)/rs);
  }

ReDisplay:

  ColToWhite();
  FRectangle(0, ImageHeight, BoxWidth * NBBUTTONS, BoxHeight * NBLIGBUTT);
  ColToBlack();

  for (i = 0; i < NBLIGBUTT; i++) 
  {
    Line(0, ImageHeight + i * BoxHeight, Width, ImageHeight + i * BoxHeight);
    for (k = 0; k < NBBUTTONS; k++) 
    {
      Line(k * BoxWidth, ImageHeight + i * BoxHeight, k * BoxWidth, ImageHeight + (i+1) * BoxHeight);
      String(k * BoxWidth + 3, ImageHeight + (i+1) * BoxHeight - 2, ButtonStrings[i][k]);
    }
  }

  FlushGraphics();  

NextEvent:
  eventtype = WaitEvent(&xx, &yy);
  switch(eventtype)
  {
    case KeyPress: 
      switch(xx)
      {
        case key_q: goto Terminate;
      }

    case ButtonPress:
    break;

    case ButtonRelease: goto NextEvent;

    case EnterNotify: goto ReDisplay;

    default: printf("type : %d ; x = %d ; y = %d\n", eventtype, xx, yy); break;

  } /* switch(eventtype) */

  if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight))
  {
    if (zoom)
    {
      xx /= 2;
      yy /= 2;
    }
    x = iplan * n + yy * rs + xx;

    LAB[*nlabs] = x;
    VLAB[*nlabs] = curlab;
    if (incr) 
    {
      curlab++;
      sprintf(ButtonStrings[0][1], "%d", curlab);
    }
    *nlabs += 1;

    Color(MC_RED);
    if (zoom)
    {
      Point(2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
    }
    else
      Point((x%n)%rs, (x%n)/rs);
    goto ReDisplay;
  }

  xx /= BoxWidth;
  yy -= ImageHeight;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: curlab++;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 3: curlab--;
                      if (curlab < 1) curlab = 1;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 4: if (incr) 
                      {
                        incr = 0; 
                        sprintf(ButtonStrings[0][4], "%s", "INCR ");
		      }
                      else
		      { 
                        incr = 1;
                        sprintf(ButtonStrings[0][4], "%s", "IDEM ");
		      }
                      break;
              case 5: *nlabs = 0;
                      goto ReDisplayImage;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
    case 1: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: iplan += incrplan;
                      if (iplan > d-1) iplan = d-1;
                      sprintf(ButtonStrings[1][1], "%d", iplan);
                      goto ReDisplayImage;
              case 3: iplan -= incrplan;
                      if (iplan < 0) iplan = 0;
                      sprintf(ButtonStrings[1][1], "%d", iplan);
                      goto ReDisplayImage;
              case 4: if (incrplan == 1) incrplan = 5; else incrplan += 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplayImage;
              case 5: if (incrplan == 5) incrplan = 1; else if (incrplan > 5) incrplan -= 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplayImage;
              case 6: break;
            } /* switch(xx) */
            break;
    default: goto ReDisplayImage;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();

  return(1);
} /* lclicklist() */


/* ==================================== */
void SetRange(uint8_t *SOURCE, int32_t rs, int32_t n, int32_t xr, int32_t yr, int32_t zr, 
              int32_t xc, int32_t yc, int32_t zc, int32_t *inf, int32_t *sup)
/* ==================================== */
{
  int32_t i, j, k, t;
  if (zr > zc) {t = zr; zr = zc; zc = t;} 
  if (yr > yc) {t = yr; yr = yc; yc = t;} 
  if (xr > xc) {t = xr; xr = xc; xc = t;} 
  *inf = *sup = SOURCE[zr * n + yr * rs + xr];
  for (k = zr; k <= zc; k++)
  for (j = yr; j <= yc; j++)
  for (i = xr; i <= xc; i++)
  {
    t = SOURCE[k * n + j * rs + i];
    if (t > *sup) *sup = t;
    else if (t < *inf) *inf = t;
  }
} /* SetRange() */

#define VAL_PROPAG 255
#define VAL_NULLE   0

/* ==================================== */
void InitPropag(uint8_t *PROPAG, int32_t rs, int32_t n, int32_t xr, int32_t yr, int32_t zr, 
              int32_t xc, int32_t yc, int32_t zc)
/* ==================================== */
{
  int32_t i, j, k, t;
  if (zr > zc) {t = zr; zr = zc; zc = t;} 
  if (yr > yc) {t = yr; yr = yc; yc = t;} 
  if (xr > xc) {t = xr; xr = xc; xc = t;} 
  for (k = zr; k <= zc; k++)
  for (j = yr; j <= yc; j++)
  for (i = xr; i <= xc; i++)
  {
    PROPAG[k * n + j * rs + i] = VAL_PROPAG;
  }
} /* InitPropag() */

/* ==================================== */
void Propagate(uint8_t *SOURCE, uint8_t *PROPAG, int32_t rs, int32_t n, int32_t N, 
               int32_t connex, int32_t infrange, int32_t suprange, int32_t tolinf, int32_t tolsup, int32_t radius)
/* ==================================== */
{
  Lifo * LIFO1, * LIFO2, * LIFOtmp;
  int32_t p, q, k, s;
  double range;
  int32_t niter;

  if (suprange < infrange) return;

  suprange += tolsup;
  infrange -= tolinf;

  LIFO1 = CreeLifoVide(N/2);
  LIFO2 = CreeLifoVide(N/2); /* ATTENTION: peut etre insuffisant dans des cas tres rares */
  if ((LIFO1 == NULL) || (LIFO2 == NULL))
  { 
    fprintf(stderr, "Propagate() : CreeLifoVide failed\n");
    exit(0);
  }

  /* init la lifo1 avec les points de la zone marquee */
  for (p = 0; p < N; p++)
    if (PROPAG[p])      
      LifoPush(LIFO1, p);

  /* boucle : propagation largeur d'abord */
  switch (connex)
  {
    case 6:
      niter = 0;
      while ((! LifoVide(LIFO1)) && (niter < radius))
      {
        while (! LifoVide(LIFO1))
        {
          p = LifoPop(LIFO1);
          for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
          {
            q = voisin6(p, k, rs, n, N);
            if ((q != -1) && (PROPAG[q] == 0))
            {
              s = SOURCE[q];
              if ((s >= infrange) && (s <= suprange))
 	      {
                PROPAG[q] = VAL_PROPAG;
                LifoPush(LIFO2, q);
 	      }
 	    }
          }
        } /* while (! LifoVide(LIFO1)) */
        niter++;
        LIFOtmp = LIFO1; LIFO1 = LIFO2; LIFO2 = LIFOtmp;
      } /* while (! LifoVide(LIFO1)) */
      break;
  } /* switch (connex) */

  LifoTermine(LIFO1);
  LifoTermine(LIFO2);
  
} /* Propagate() */

/* ==================================== */
int32_t lclick3d(struct xvimage *image, uint32_t *LAB, int32_t *VLAB, int32_t *nlabs, int32_t zoom)
/* ==================================== */
{
  register int32_t x, y, z;        /* index muet */
  int32_t i, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t n = rs * cs;             /* taille plan */
  int32_t d = depth(image);        /* nb. plans */
  int32_t N = d * n;               /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  int32_t curlab = 1;
  int32_t incr = 0;
  int32_t incrplan = 1;
#define NBBUTTONS3D  7
#define NBLIGBUTT3D  3
#define BOXWIDTH  70
#define BOXHEIGHT 20
#define MARGE     10
  char * ButtonStrings[NBLIGBUTT3D][NBBUTTONS3D];
  int32_t xx, yy;
  int32_t button;
  int32_t xref = rs / 2; /* coordonnees du point de reference */
  int32_t yref = cs / 2;
  int32_t zref = d  / 2;
  int32_t xclick = xref; /* coordonnees du dernier point clique' */
  int32_t yclick = yref;
  int32_t zclick = zref;
  char var = 'z'; /* axe normal au plan de la vue principale */
  int32_t eventtype;
  int32_t spot = 1;
  char coordrefstring[128];
  char coordclickstring[128];
  char valclickstring[128];
  char valrangestring[128];
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  /* dimensions totales */
  int32_t Height = cs + MARGE + d + BoxHeight * NBLIGBUTT3D;
  int32_t Width = rs + MARGE + d;
  /* dimensions du panneau image */
  int32_t ImagePannelHeight = cs + MARGE + d;
  int32_t ImagePannelWidth = rs + MARGE + d;
  /* dimensions image "centrale" */
  int32_t ImageHeight = cs;
  int32_t ImageWidth = rs;  
  /* dimensions image "droite" */
  int32_t ImageRightWidth = d;  
  /* dimensions image "bas" */
  int32_t ImageBottomHeight = d;
  /* variables pour la "baguette magique" */
  int32_t suprange = -1, infrange = 0;
  int32_t tolinf = 0;
  int32_t tolsup = 0;
  int32_t radius = 0;
  struct xvimage *propag;
  uint8_t *PROPAG;        /* l'image pour la baguette */
  int32_t connex = 6;
#define FIELD_COOR 0
#define FIELD_TOLP 1
#define FIELD_TOLM 2
#define FIELD_RAD  3
  int32_t selfield = 0;

  propag = allocimage(NULL, rs, cs, d, VFF_TYP_1_BYTE);
  if (propag == NULL)
  {   fprintf(stderr,"click3d() : allocimage failed for PROPAG\n");
      return 0;
  }
  PROPAG = UCHARDATA(propag);
  memset(PROPAG, 0, N);

  coordclickstring[0] = '\0';
  valclickstring[0] = '\0';
  valrangestring[0] = '\0';

  *nlabs = 0;

  if (zoom)
  {
    Height += cs + d;
    Width += rs + d;
    ImagePannelHeight += cs + d;
    ImagePannelWidth += rs + d;
    ImageHeight += cs;
    ImageWidth += rs;
    ImageBottomHeight += d;
    ImageRightWidth += d;
  }

  if (BoxWidth * NBBUTTONS3D > Width) Width = BoxWidth * NBBUTTONS3D;
  
  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT3D; i++) 
    for (k = 0; k < NBBUTTONS3D; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], "LABEL");
  sprintf(ButtonStrings[0][1], "%d", curlab);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], "INCR ");
  strcpy(ButtonStrings[0][5], "UNDO ");
  strcpy(ButtonStrings[0][6], "QUIT ");

  sprintf(ButtonStrings[1][0], "  %c ", 'z');
  sprintf(ButtonStrings[1][1], "%d", zref);
  sprintf(ButtonStrings[1][2], " +%d", incrplan);
  sprintf(ButtonStrings[1][3], " -%d", incrplan);
  strcpy(ButtonStrings[1][4], "  ++ ");
  strcpy(ButtonStrings[1][5], "  -- ");
  strcpy(ButtonStrings[1][6], "SPOT ");

  strcpy(ButtonStrings[2][0], "PROPAG");
  strcpy(ButtonStrings[2][1], " SET ");
  strcpy(ButtonStrings[2][2], " TOL-");
  strcpy(ButtonStrings[2][3], " TOL+");
  strcpy(ButtonStrings[2][4], " RAD ");
  strcpy(ButtonStrings[2][5], "CLEAR");
  strcpy(ButtonStrings[2][6], "SAVE ");

/* ===========================================================
    affichage : 
   ===========================================================

    <------- rs ------>      <---- d ---->
    ------------------- ^    -------------
    |                 | .    |           |
    |                 | .    |           |
    |                 | .    |           |
    |      image      | cs   |           |
    |                 | .    |           |
    |                 | .    |           |
    |                 | .    |           |
    ------------------- v    -------------
    ------------------- ^
    |                 | .     1,2,3
    |                 | d
    |                 | .
    |                 | .
    ------------------- v  
    --------------------------------------------------
    |LABEL | 111  |  +   |  -   | AUTO | UNDO | QUIT |        BoxHeight
    --------------------------------------------------
    |  z   | 111  | +1   | -1   |  ++  |  --  | SPOT |        BoxHeight
    --------------------------------------------------
    |PROPAG | SET  | TOL- | TOL+ | RAD  |CLEAR | SAVE |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  /* send output to the screen */

  /* ================================================================= */
  /* image "centrale" (projection x,y) */
  /* ================================================================= */
  if (zoom)
  {
    for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
    {
      Color(SOURCE[zref * n + y * rs + x]/4);
      Rectangle(2*x, 2*y, 1, 1);
    }
  } /* if (zoom) */
  else
    for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
    {
      Color(SOURCE[zref * n + y * rs + x]/4);
      Point(x, y);
    }

  Color(MC_RED);
  x = xref;
  if (var == 'x')
  {
    if (zoom)
    {
      for (y = 0; y < cs; y++)
        Rectangle(2*x, 2*y, 1, 1);
    } /* if (zoom) */
    else
      for (y = 0; y < cs; y++)
        Point(x, y);
  } /* if (var == 'x') */
  else
  {
    if (zoom)
    {
      for (y = 0; y < 5; y++) Rectangle(2*x, 2*y, 1, 1);
      for (y = cs - 5; y < cs; y++) Rectangle(2*x, 2*y, 1, 1);
    } /* if (zoom) */
    else
    {
      for (y = 0; y < 5; y++) Point(x, y);
      for (y = cs - 5; y < cs; y++) Point(x, y);
    }
  } /* else if (var == 'x') */

  y = yref;
  if (var == 'y')
  {
    if (zoom)
    {
      for (x = 0; x < rs; x++)
        Rectangle(2*x, 2*y, 1, 1);
    } /* if (zoom) */
    else
      for (x = 0; x < rs; x++)
        Point(x, y);
  } /* if (var == 'x') */
  else
  {
    if (zoom)
    {
      for (x = 0; x < 5; x++) Rectangle(2*x, 2*y, 1, 1);
      for (x = rs - 5; x < rs; x++) Rectangle(2*x, 2*y, 1, 1);
    } /* if (zoom) */
    else
    {
      for (x = 0; x < 5; x++) Point(x, y);
      for (x = rs - 5; x < rs; x++) Point(x, y);
    }
  } /* else if (var == 'y') */

  Color(MC_GREEN);
  for (i = 0; i < *nlabs; i++) 
    if (LAB[i] / n == zref)
    {
      if (zoom)
        Rectangle(2*(LAB[i]%rs), 2*((LAB[i]%n)/rs), 1, 1);
      else
        Point((LAB[i]%rs), ((LAB[i]%n)/rs));
    }

  if (spot)
  {
    if (zoom)
    {
      Rectangle(2*xref - 10, 2 * yref, 20, 1);
      Rectangle(2*xref, 2 * yref - 10, 1, 20);
    }
    else
    {
      Line(xref - 5, yref, xref + 5, yref);
      Line(xref, yref - 5, xref, yref + 5);
    }
  }

  /* Baguette Magique */
  if (suprange >= infrange)
  {
    Color(MC_BLUE);
    if (zoom)
    {
      for (y = 0; y < cs; y++) for (x = 0; x < rs; x++) 
        if (PROPAG[zref * n + y * rs + x]) Rectangle(2*x, 2*y, 1, 1);
    }
    else
    {
      for (y = 0; y < cs; y++) for (x = 0; x < rs; x++)
        if (PROPAG[zref * n + y * rs + x]) Point(x, y);
    }
  } /* Baguette Magique */

  /* ================================================================= */
  /* image droite (projection y,z) */
  /* ================================================================= */
  if (zoom)
  {
    for (y = 0; y < cs; y++) for (z = 0; z < d; z++)
    {
      Color(SOURCE[z * n + y * rs + xref]/4);
      Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    }
  } /* if (zoom) */
  else
    for (y = 0; y < cs; y++) for (z = 0; z < d; z++)
    {
      Point(ImageWidth + MARGE + z, y);
      Color(SOURCE[z * n + y * rs + xref]/4);
    }

  Color(MC_RED);
  z = zref;
  if (var == 'z')
  {
    if (zoom)
    {
      for (y = 0; y < cs; y++)
        Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    } /* if (zoom) */
    else
      for (y = 0; y < cs; y++)
        Point(ImageWidth + MARGE + z, y);
  } /* if (var == 'z') */
  else
  {
    if (zoom)
    {
      for (y = 0; y < 5; y++) Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
      for (y = cs - 5; y < cs; y++) Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    } /* if (zoom) */
    else
    {
      for (y = 0; y < 5; y++) Point(ImageWidth + MARGE + z, y);
      for (y = cs - 5; y < cs; y++) Point(ImageWidth + MARGE + z, y);
    }
  } /* else if (var == 'z') */

  y = yref;
  if (var == 'y')
  {
    if (zoom)
    {
      for (z = 0; z < d; z++)
        Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    } /* if (zoom) */
    else
    {
      for (z = 0; z < d; z++)
        Point(ImageWidth + MARGE + z, y);
    }
  } /* if (var == 'y') */
  else
  {
    if (zoom)
    {
      for (z = 0; z < 5; z++) Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
      for (z = d-5; z < d; z++) Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    } /* if (zoom) */
    else
    {
      for (z = 0; z < 5; z++) Point(ImageWidth + MARGE + z, y);
      for (z = d-5; z < d; z++) Point(ImageWidth + MARGE + z, y);
    }
  } /* else if (var == 'y') */

  Color(MC_GREEN);
  for (i = 0; i < *nlabs; i++) 
    if (LAB[i] % rs == xref)
    {
      if (zoom)
        Rectangle(ImageWidth + MARGE + 2*(LAB[i]/n), 2*((LAB[i]%n)/rs), 1, 1);
      else
        Point(ImageWidth + MARGE + (LAB[i]/n), ((LAB[i]%n)/rs));
    }

  if (spot)
  {
    if (zoom)
    {
      Rectangle(ImageWidth + MARGE + 2*zref - 10, 2 * yref, 20, 1);
      Rectangle(ImageWidth + MARGE + 2*zref, 2 * yref - 10, 1, 20);
    }
    else
    {
      Line(ImageWidth + MARGE + zref - 5, yref, ImageWidth + MARGE + zref + 5, yref);
      Line(ImageWidth + MARGE + zref, yref - 5, ImageWidth + MARGE + zref, yref + 5);
    }
  }

  /* Baguette Magique */
  if (suprange >= infrange)
  {
    Color(MC_BLUE);
    if (zoom)
    {
      for (y = 0; y < cs; y++) for (z = 0; z < d; z++)
        if (PROPAG[z * n + y * rs + xref]) Rectangle(ImageWidth + MARGE + 2*z, 2*y, 1, 1);
    }
    else
    {
      for (y = 0; y < cs; y++) for (z = 0; z < d; z++)
        if (PROPAG[z * n + y * rs + xref]) Point(ImageWidth + MARGE + z, y);
    }
  } /* Baguette Magique */

  /* ================================================================= */
  /* image bas (projection x,z) */
  /* ================================================================= */
  if (zoom)
  {
    for (x = 0; x < rs; x++) for (z = 0; z < d; z++)
    {
      Color(SOURCE[z * n + yref * rs + x]/4);
      Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    }
  } /* if (zoom) */
  else
    for (x = 0; x < rs; x++) for (z = 0; z < d; z++)
    {
      Color(SOURCE[z * n + yref * rs + x]/4);
      Point(x, ImageHeight + MARGE + z);
    }

  Color(MC_RED);
  x = xref;
  if (var == 'x')
  {
    if (zoom)
    {
      for (z = 0; z < d; z++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    } /* if (zoom) */
    else
    {
      for (z = 0; z < d; z++) Point(x, ImageHeight + MARGE + z);
    }
  } /* if (var == 'x') */
  else
  {
    if (zoom)
    {
      for (z = 0; z < 5; z++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
      for (z = d-5; z < d; z++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    } /* if (zoom) */
    else
    {
      for (z = 0; z < 5; z++) Point(x, ImageHeight + MARGE + z);
      for (z = d-5; z < d; z++) Point(x, ImageHeight + MARGE + z);
    }
  } /* else if (var == 'x') */

  z = zref;
  if (var == 'z')
  {
    if (zoom)
    {
      for (x = 0; x < rs; x++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    } /* if (zoom) */
    else
    {
      for (x = 0; x < rs; x++) Point(x, ImageHeight + MARGE + z);
    }
  } /* if (var == 'z') */
  else
  {
    if (zoom)
    {
      for (x = 0; x < 5; x++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
      for (x = rs-5; x < rs; x++) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    } /* if (zoom) */
    else
    {
      for (x = 0; x < 5; x++) Point(x, ImageHeight + MARGE + z);
      for (x = rs-5; x < rs; x++) Point(x, ImageHeight + MARGE + z);
    }
  } /* else if (var == 'z') */

  Color(MC_GREEN);
  for (i = 0; i < *nlabs; i++) 
    if ((LAB[i] % n) / rs == yref)
    {
      if (zoom)
        Rectangle(2*(LAB[i]%rs), ImageHeight + MARGE + 2*(LAB[i]/n), 1, 1);
      else
        Point((LAB[i]%rs), ImageHeight + MARGE + (LAB[i]/n));
    }

  if (spot)
  {
    if (zoom)
    {
      Rectangle(2*xref - 10, ImageHeight + MARGE + 2 * zref, 20, 1);
      Rectangle(2*xref, ImageHeight + MARGE + 2 * zref - 10, 1, 20);
    }
    else
    {
      Line(xref - 5, ImageHeight + MARGE + zref, xref + 5, ImageHeight + MARGE + zref);
      Line(xref, ImageHeight + MARGE + zref - 5, xref, ImageHeight + MARGE + zref + 5);
    }
  }

  /* Baguette Magique */
  if (suprange >= infrange)
  {
    Color(MC_BLUE);
    if (zoom)
    {
      for (x = 0; x < rs; x++) for (z = 0; z < d; z++)  
        if (PROPAG[z * n + yref * rs + x]) Rectangle(2*x, ImageHeight + MARGE + 2*z, 1, 1);
    }
    else
    {
      for (x = 0; x < rs; x++) for (z = 0; z < d; z++)
        if (PROPAG[z * n + yref * rs + x]) Point(x, ImageHeight + MARGE + z);
    }
  } /* Baguette Magique */

  ColToBlack();
  FRectangle(ImageWidth, ImageHeight, 
    mcmax(ImagePannelWidth,Width) - ImageWidth, ImagePannelHeight - ImageHeight);
  ColToWhite();
  sprintf(coordrefstring, "%d,%d,%d", xref, yref, zref);
  String(ImageWidth + 2 * MARGE, ImageHeight + BoxHeight + MARGE, coordrefstring);
  String(ImageWidth + 2 * MARGE, ImageHeight+(2*BoxHeight) + MARGE, coordclickstring);
  String(ImageWidth + 2 * MARGE, ImageHeight+(3*BoxHeight) + MARGE, valclickstring);
  String(ImageWidth + 2 * MARGE, ImageHeight+(4*BoxHeight) + MARGE, valrangestring);

  /* ================================================================= */
  /* ================================================================= */
ReDisplay:
  /* ================================================================= */
  /* ================================================================= */

  ColToWhite();
  FRectangle(0, ImagePannelHeight, BoxWidth * NBBUTTONS3D, BoxHeight * NBLIGBUTT3D);
  ColToBlack();

  for (i = 0; i < NBLIGBUTT3D; i++) 
  {
    Line(0, ImagePannelHeight + i * BoxHeight, Width, ImagePannelHeight + i * BoxHeight);
    for (k = 0; k < NBBUTTONS3D; k++) 
    {
      Line(k * BoxWidth, ImagePannelHeight + i * BoxHeight, k * BoxWidth, ImagePannelHeight + (i+1) * BoxHeight);
      String(k * BoxWidth + 3, ImagePannelHeight + (i+1) * BoxHeight - 2, ButtonStrings[i][k]);
    }
  }

  FlushGraphics();  

NextEvent:
  eventtype = WaitAnyEvent(&xx, &yy, &button);
  switch(eventtype)
  {
    case KeyPress: 
      switch(xx)
      {
        case key_q: goto Terminate;
        case key_p: /* action CUTPLANE */
	            if (var == 'z')
	            {
                      for (x = 0; x < rs; x++)
                        for (y = 0; y < cs; y++)
                          SOURCE[zref * n + y * rs + x] = 0;
                    } else
	            if (var == 'y')
	            {
                      for (x = 0; x < rs; x++)
                        for (z = 0; z < d; z++)
                          SOURCE[z * n + yref * rs + x] = 0;
                    }
	            if (var == 'x')
	            {
                      for (z = 0; z < d; z++)
                        for (y = 0; y < cs; y++)
                          SOURCE[z * n + y * rs + xref] = 0;
                    }
                    goto ReDisplayImage;
        case key_l: /* action LABEL */
                    LAB[*nlabs] = zclick * n + yclick * rs + xclick;
                    VLAB[*nlabs] = curlab;
                    *nlabs += 1;
                    if (incr)
                    {
                      curlab++;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                    }
                    goto ReDisplayImage;
        case key_c: /* action CLEAR */
                    memset(PROPAG, VAL_NULLE, N);
                    suprange = -1;
                    valrangestring[0] = '\0';
                    tolinf = tolsup = 0;
                    radius = 0;
                    sprintf(ButtonStrings[2][2], "-%d", tolinf);
                    sprintf(ButtonStrings[2][3], "+%d", tolsup);
                    sprintf(ButtonStrings[2][4], "R%d", radius);
                    goto ReDisplayImage;
        case key_m: /* action PROPAG */
                    Propagate(SOURCE, PROPAG, rs, n, N, connex, 
                              infrange, suprange, tolinf, tolsup, radius);
                    goto ReDisplayImage;
        case key_s: /* action SETRANGE */
                    SetRange(SOURCE, rs, n, xref, yref, zref, 
                               xclick, yclick, zclick, &infrange, &suprange);
                    InitPropag(PROPAG, rs, n, xref, yref, zref, xclick, yclick, zclick);
                    sprintf(valrangestring, "%d-%d", infrange-tolinf, suprange+tolsup);
                    goto ReDisplayImage;
        case key_r: xref = xclick; yref = yclick; zref = zclick; goto ReDisplayImage;
        case key_x: 
           var = 'x'; 
           sprintf(ButtonStrings[1][0], "  %c ", var);
           sprintf(ButtonStrings[1][1], "%d", xref);
           goto ReDisplayImage;
        case key_y: 
           var = 'y'; 
           sprintf(ButtonStrings[1][0], "  %c ", var);
           sprintf(ButtonStrings[1][1], "%d", yref);
           goto ReDisplayImage;
        case key_z: 
           var = 'z'; 
           sprintf(ButtonStrings[1][0], "  %c ", var);
           sprintf(ButtonStrings[1][1], "%d", zref);
           goto ReDisplayImage;
        case left:
        case Left:
          switch (selfield)
	  {
	  case FIELD_COOR:  
           switch (var)
	   {
             case 'x': zref--; break;
             case 'y': xref--; break;
             case 'z': xref--; break;
	   }
           goto ReDisplayImage;
	  case FIELD_TOLM: 
           tolinf -= incrplan;
           sprintf(ButtonStrings[2][2], "-%d", tolinf);
           break;
	  case FIELD_TOLP: 
           tolsup -= incrplan; if (tolsup < 0) tolsup = 0;
           sprintf(ButtonStrings[2][2], "+%d", tolsup);
           break;
	  } /* switch (selfield) */
        case right:
        case Right:
           switch (var)
	   {
             case 'x': zref++; break;
             case 'y': xref++; break;
             case 'z': xref++; break;
	   }
           goto ReDisplayImage;
        case up:
        case Up:
           switch (var)
	   {
             case 'x': yref--; break;
             case 'y': zref--; break;
             case 'z': yref--; break;
	   }
           goto ReDisplayImage;
        case Down:
        case down:
           switch (var)
	   {
             case 'x': yref++; break;
             case 'y': zref++; break;
             case 'z': yref++; break;
	   }
           goto ReDisplayImage;
      }

    case ButtonPress:
    break;

    case ButtonRelease: goto NextEvent;

    case EnterNotify: goto ReDisplay;

    default: printf("type : %d ; x = %d ; y = %d\n", eventtype, xx, yy); break;

  } /* switch(eventtype) */

  if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight))
  {  /* click dans l'image "principale" */
    if (zoom)
    {
      xx /= 2;
      yy /= 2;
    }
    xclick = xx; yclick = yy; zclick = zref;  
    x = zclick * n + yclick * rs + xclick;
    sprintf(coordclickstring, "%d,%d,%d", xclick, yclick, zclick);
    sprintf(valclickstring, "%d", SOURCE[x]);
    
    goto ReDisplayImage;
  } /* if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight)) */
  else
  if ((xx >= ImageWidth+MARGE) && (yy >= 0) && 
      (xx < ImageWidth+MARGE+ImageRightWidth) && (yy < ImageHeight))
  {  /* click dans l'image "droite" */
    xx -= (ImageWidth+MARGE);
    if (zoom)
    {
      xx /= 2;
      yy /= 2;
    }
    xclick = xref; yclick = yy; zclick = xx;  
    x = zclick * n + yclick * rs + xclick;
    sprintf(coordclickstring, "%d,%d,%d", xclick, yclick, zclick);
    sprintf(valclickstring, "%d", SOURCE[x]);
    
    goto ReDisplayImage;
  }
  else
  if ((xx >= 0) && (yy >= ImageHeight+MARGE) && 
      (xx < ImageWidth) && (yy < ImageHeight+MARGE+ImageBottomHeight))
  {  /* click dans l'image "bas" */
    yy -= (ImageHeight+MARGE);
    if (zoom)
    {
      xx /= 2;
      yy /= 2;
    }
    xclick = xx; yclick = yref; zclick = yy;  
    x = zclick * n + yclick * rs + xclick;
    sprintf(coordclickstring, "%d,%d,%d", xclick, yclick, zclick);
    sprintf(valclickstring, "%d", SOURCE[x]);
    
    goto ReDisplayImage;
  }

  if ((xx >= 0) && (yy >= 0) && (xx < ImagePannelWidth) && (yy < ImagePannelHeight))
    goto ReDisplayImage;

  xx /= BoxWidth;
  yy -= ImagePannelHeight;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: /* bouton LABEL */
                      LAB[*nlabs] = zclick * n + yclick * rs + xclick;
                      VLAB[*nlabs] = curlab;
                      *nlabs += 1;
                      if (incr)
                      {
                        curlab++;
                        sprintf(ButtonStrings[0][1], "%d", curlab);
                      }
                      goto ReDisplayImage;
              case 1: break;
              case 2: curlab++;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 3: curlab--;
                      if (curlab < 1) curlab = 1;
                      sprintf(ButtonStrings[0][1], "%d", curlab);
                      break;
              case 4: if (incr) 
                      {
                        incr = 0; 
                        sprintf(ButtonStrings[0][4], "%s", "INCR ");
		      }
                      else
		      { 
                        incr = 1;
                        sprintf(ButtonStrings[0][4], "%s", "IDEM ");
		      }
                      break;
              case 5: *nlabs = 0;
                      goto ReDisplayImage;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
    case 1: switch(xx)
            {
              case 0: selfield = FIELD_COOR;
                      if (var == 'x') var = 'y'; 
                      else if (var == 'y') var = 'z'; 
                      else if (var == 'z') var = 'x'; 
                      sprintf(ButtonStrings[1][0], "  %c ", var);
                      if (var == 'x') 
                        sprintf(ButtonStrings[1][1], "%d", xref);
		      else if (var == 'y') 
                        sprintf(ButtonStrings[1][1], "%d", yref);
		      else if (var == 'z') 
                        sprintf(ButtonStrings[1][1], "%d", zref);
                      goto ReDisplayImage;
              case 1: break;
              case 2: if (var == 'x') 
                      {
                        xref += incrplan;
                        if (xref > rs-1) xref = rs-1;
                        sprintf(ButtonStrings[1][1], "%d", xref);
		      } else
                      if (var == 'y') 
                      {
                        yref += incrplan;
                        if (yref > cs-1) yref = cs-1;
                        sprintf(ButtonStrings[1][1], "%d", yref);
		      } else
                      if (var == 'z') 
                      {
                        zref += incrplan;
                        if (zref > d-1) zref = d-1;
                        sprintf(ButtonStrings[1][1], "%d", zref);
		      }
                      goto ReDisplayImage;
              case 3: if (var == 'x') 
                      {
                        xref -= incrplan;
                        if (xref < 0) xref = 0;
                        sprintf(ButtonStrings[1][1], "%d", xref);
		      } else
                      if (var == 'y') 
                      {
                        yref -= incrplan;
                        if (yref < 0) yref = 0;
                        sprintf(ButtonStrings[1][1], "%d", yref);
		      } else
                      if (var == 'z') 
                      {
                        zref -= incrplan;
                        if (zref < 0) zref = 0;
                        sprintf(ButtonStrings[1][1], "%d", zref);
		      }
                      goto ReDisplayImage;
              case 4: if (incrplan == 1) incrplan = 5; else incrplan += 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplay;
              case 5: if (incrplan == 5) incrplan = 1; else if (incrplan > 5) incrplan -= 5;
                      sprintf(ButtonStrings[1][2], " +%d", incrplan);
                      sprintf(ButtonStrings[1][3], " -%d", incrplan);
                      goto ReDisplay;
              case 6: if (spot) spot = 0; else spot = 1;
                      goto ReDisplayImage;
            } /* switch(xx) */
            break;
    case 2: switch(xx)
            {
              case 0: Propagate(SOURCE, PROPAG, rs, n, N, connex, 
                                infrange, suprange, tolinf, tolsup, radius);
                      goto ReDisplayImage;
              case 1: SetRange(SOURCE, rs, n, xref, yref, zref, 
                               xclick, yclick, zclick, &infrange, &suprange);
                      InitPropag(PROPAG, rs, n, xref, yref, zref, xclick, yclick, zclick);
                      sprintf(valrangestring, "%d-%d", infrange-tolinf, suprange+tolsup);
                      goto ReDisplayImage;
              case 2: selfield = FIELD_TOLM;
                      if (button == MC_LEFTBUTTON) tolinf++; else tolinf+=10;
                      sprintf(ButtonStrings[2][2], "-%d", tolinf);
                      break;
              case 3: selfield = FIELD_TOLP;
                      if (button == MC_LEFTBUTTON) tolsup++; else tolsup+=10;
                      sprintf(ButtonStrings[2][3], "+%d", tolsup);
                      break;
              case 4: selfield = FIELD_RAD;
                      if (button == MC_LEFTBUTTON) radius++; else radius+=10;
                      sprintf(ButtonStrings[2][4], "R%d", radius);
                      break;


              case 5: memset(PROPAG, VAL_NULLE, N);
                      suprange = -1;
                      valrangestring[0] = '\0';
                      radius = tolinf = tolsup = 0;
                      sprintf(ButtonStrings[2][2], "-%d", tolinf);
                      sprintf(ButtonStrings[2][3], "+%d", tolsup);
                      sprintf(ButtonStrings[2][4], "R%d", radius);
                      goto ReDisplayImage;
              case 6: writeimage(propag, "propag.pgm");
                      break;
            } /* switch(xx) */
            break;
    default: goto ReDisplayImage;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();
  freeimage(propag);

  return(1);
} /* lclick3d() */

