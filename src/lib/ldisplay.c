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
/* visualisation des pixels et de leurs valeurs */
/* Michel Couprie - juin 1999 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcxbib.h>
#include <lclick.h>

/* ==================================== */
int32_t lclick(struct xvimage *image, struct xvimage *lab, int32_t zoom)
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
  strcpy(ButtonStrings[1][2], "  +  ");
  strcpy(ButtonStrings[1][3], "  -  ");
  strcpy(ButtonStrings[1][4], "     ");
  strcpy(ButtonStrings[1][5], "     ");
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
    | PLAN | 111  |  +   |  -   |      |      |      |        BoxHeight
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

  WaitMouseEvent(&xx, &yy);

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
              case 4: break;
              case 5: break;
              case 6: break;
            } /* switch(xx) */
            break;
    default: goto ReDisplayImage;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();
  return(1);
}

