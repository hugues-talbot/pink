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
/* affichage d'une image 3d par projection de voxels */
/* Michel Couprie - septembre 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcxbib.h>
#include <lseuili.h>

/* ==================================== */
int32_t lseuili(struct xvimage *image)
/* ==================================== */
{
  register int32_t i;              /* index muet */
  int32_t k;                       /* index muet */
  register int32_t x;              /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint8_t *TEMP;
  

  int32_t Height = cs + ;
  int32_t Width = rs;
  
  InitGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], " INF ");
  sprintf(ButtonStrings[0][1], "%d", seuilinf);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], "  1  ");
  strcpy(ButtonStrings[0][5], "  5  ");
  strcpy(ButtonStrings[0][6], "     ");

  strcpy(ButtonStrings[1][0], " SUP ");
  sprintf(ButtonStrings[1][1], "%d", seuilsup);
  strcpy(ButtonStrings[1][2], "  +  ");
  strcpy(ButtonStrings[1][3], "  -  ");
  strcpy(ButtonStrings[1][4], " 10  ");
  strcpy(ButtonStrings[1][5], " 20  ");
  strcpy(ButtonStrings[1][6], "     ");

  strcpy(ButtonStrings[2][0], " PLAN");
  sprintf(ButtonStrings[2][1], "%d", iplan);
  strcpy(ButtonStrings[2][2], "  +  ");
  strcpy(ButtonStrings[2][3], "  -  ");
  strcpy(ButtonStrings[2][4], "     ");
  strcpy(ButtonStrings[2][5], "  OK ");
  strcpy(ButtonStrings[2][6], " QUIT");

  TEMP = (uint8_t *)calloc(1,N * sizeof(char));
  if (TEMP == NULL)
  {   fprintf(stderr, "lseuili() : malloc failed for TEMP\n");
      return(0);
  }

ReSeuil:
  for (x = 0; x < N; x++)
    if ((SOURCE[x] >= seuilinf) && (SOURCE[x] <= seuilsup))
      TEMP[x] = 255;
    else
      TEMP[x] = 0;

/*
    affichage : 

    <------- rs ------>     
    ------------------- .
    |                 | .
    |                 | .
    |                 | .
    |      image      | cs
    |                 | .
    |                 | .
    |                 | .
    -------------------------------------------------
    | inf  | 111  |  +   |  -   |  1   |  5  |      | BoxHeight
    -------------------------------------------------
    | sup  | 111  |  +   |  -   |  10  |  20 |      | BoxHeight
    -------------------------------------------------
    | plan | 111  |  +   |  -   |      |  OK | QUIT | BoxHeight
    -------------------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  FRectangle(0, 0, rs, cs);
  ColToWhite();

  for (x = iplan * n; x < (iplan+1)*n; x++)
    if (TEMP[x])    
      Point((x%n)%rs, (x%n)/rs);
  /* send output to the screen */
  ColToBlack();

ReDisplay:

  ColToWhite();
  FRectangle(0, cs, BoxWidth * NBBUTTONS, BoxHeight * NBLIGBUTT);
  ColToBlack();

  for (i = 0; i < NBLIGBUTT; i++) 
  {
    Line(0, cs + i * BoxHeight, Width, cs + i * BoxHeight);
    for (k = 0; k < NBBUTTONS; k++) 
    {
      Line(k * BoxWidth, cs + i * BoxHeight, k * BoxWidth, cs + (i+1) * BoxHeight);
      String(k * BoxWidth + 3, cs + (i+1) * BoxHeight - 2, ButtonStrings[i][k]);
    }
  }

  switch (increment)
  {
    case 1: FRectangle(4 * BoxWidth, cs, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, cs + BoxHeight - 2, ButtonStrings[0][4]);
            ColToBlack();
            break;
    case 5: FRectangle(5 * BoxWidth, cs, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, cs + BoxHeight - 2, ButtonStrings[0][5]);
            ColToBlack();
            break;
   case 10: FRectangle(4 * BoxWidth, cs + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, cs + 2*BoxHeight - 2, ButtonStrings[1][4]);
            ColToBlack();
            break;
   case 20: FRectangle(5 * BoxWidth, cs + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, cs + 2*BoxHeight - 2, ButtonStrings[1][5]);
            ColToBlack();
            break;
  } /* switch (increment) */

  FlushGraphics();  

  WaitMouseEvent(&xx, &yy);

  xx /= BoxWidth;
  yy -= cs;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: seuilinf += increment;
                      if (seuilinf > NDG_MAX) seuilinf = NDG_MAX;
                      sprintf(ButtonStrings[0][1], "%d", seuilinf);
                      break;
              case 3: seuilinf -= increment;
                      if (seuilinf < NDG_MIN) seuilinf = NDG_MIN;
                      sprintf(ButtonStrings[0][1], "%d", seuilinf);
                      break;
              case 4: increment = 1;
                      break;
              case 5: increment = 5;
                      break;
              case 6: break;
            } /* switch(xx) */
            break;
    case 1: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: seuilsup += increment;
                      if (seuilsup > NDG_MAX) seuilsup = NDG_MAX;
                      sprintf(ButtonStrings[1][1], "%d", seuilsup);
                      break;
              case 3: seuilsup -= increment;
                      if (seuilsup < NDG_MIN) seuilsup = NDG_MIN;
                      sprintf(ButtonStrings[1][1], "%d", seuilsup);
                      break;
              case 4: increment = 10;
                      break;
              case 5: increment = 20;
                      break;
              case 6: break;
            } /* switch(xx) */
            break;
    case 2: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: iplan++;
                      if (iplan > d-1) iplan = d-1;
                      sprintf(ButtonStrings[2][1], "%d", iplan);
                      goto ReDisplayImage;
              case 3: iplan--;
                      if (iplan < 0) iplan = 0;
                      sprintf(ButtonStrings[2][1], "%d", iplan);
                      goto ReDisplayImage;
              case 4: increment = 1000000;
                      break;
              case 5: goto ReSeuil;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();

  for (x = 0; x < N; x++) SOURCE[x] = TEMP[x];

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      free(ButtonStrings[i][k]);
  free(TEMP);
  return(1);
}

