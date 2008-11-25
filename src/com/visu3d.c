/* $Id: visu3d.c,v 1.1.1.1 2008-11-25 08:01:37 mcouprie Exp $ */
/* \file visu3d.c

\brief 

<B>Usage:</B> 

<B>Description:</B>

<B>Types supported:</B> byte 2D

<B>Category:</B> 
\ingroup  

\author Michel Couprie
*/
/* Visualisation simultanee de deux images 3d */
/* Michel Couprie - mai 2000 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcxbib.h>

/* ==================================== */
int32_t lvisu3d(struct xvimage *image1, struct xvimage *image2, int32_t zoom)
/* ==================================== */
{
  char buf[32];
  register int32_t i;              /* index muet */
  int32_t k;                       /* index muet */
  register int32_t x;              /* index muet de pixel */
  int32_t rs = rowsize(image1);    /* taille ligne */
  int32_t cs = colsize(image1);    /* taille colonne */
  int32_t d = depth(image1);       /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *SOURCE1 = UCHARDATA(image1);
  uint8_t *SOURCE2 = UCHARDATA(image2);
  int32_t increment = 1;
  int32_t iplan = 0;

#define NBBUTTONS 5
#define NBLIGBUTT 1
#define BOXWIDTH 70
#define BOXHEIGHT 20
  char * ButtonStrings[NBLIGBUTT][NBBUTTONS];
  int32_t xx, yy;
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT;
  int32_t Width = 2 * rs;
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

  if ((rowsize(image2) != rs) || (colsize(image2) != cs) || (depth(image2) != d))
  {
    fprintf(stderr, "lvisu3d: incompatible image sizes\n");
    return 0;
  }
  
  InitColorGraphics(0, 0, /* x, y */
                    Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], " PLAN");
  sprintf(ButtonStrings[0][1], "%d", iplan);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], " QUIT");

/*
    affichage : 

    <------- rs ------> <------- rs ------>     
    ------------------- -------------------.
    |                 | |                 |.
    |                 | |                 |.
    |                 | |                 |.
    |      image      | |     image       |cs
    |       ndg       | |      bin        |.
    |                 | |                 |.
    |                 | |                 |.
    -------------------------------------------------
    ------------------------------------
    | plan | 111  |  +   |  -   | QUIT | BoxHeight
    ------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  /* send output to the screen */
  FRectangle(0, 0, ImageWidth, ImageHeight);
  for (x = iplan * n; x < (iplan+1)*n; x++)
  {
    Color(SOURCE1[x]/4);
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
  for (x = iplan * n; x < (iplan+1)*n; x++)
  {
    Color(SOURCE2[x]/4);
    if (zoom)
    {
      Point(2*rs+2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*rs+2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*rs+2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*rs+2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
    }
    else
      Point(rs+(x%n)%rs, (x%n)/rs);
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

  xx /= BoxWidth;
  yy -= ImageHeight;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: iplan++;
                      if (iplan > d-1) iplan = d-1;
                      sprintf(ButtonStrings[0][1], "%d", iplan);
                      goto ReDisplayImage;
              case 3: iplan--;
                      if (iplan < 0) iplan = 0;
                      sprintf(ButtonStrings[0][1], "%d", iplan);
                      goto ReDisplayImage;
              case 4: goto Terminate;
            } /* switch(xx) */
            break;
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      free(ButtonStrings[i][k]);

  return(1);
}

/* =============================================================== */
int main(argc, argv) 
/* =============================================================== */
  int argc; char **argv; 
{
  struct xvimage * image1;
  struct xvimage * image2;
  int32_t zoom;

  if (argc != 4)
  {
    fprintf(stderr, "usage: %s i1.pgm i2.pgm <1|0>\n", argv[0]);
    exit(1);
  }

  image1 = readimage(argv[1]);
  image2 = readimage(argv[2]);
  if ((image1 == NULL) || (image2 == NULL))
  {
    fprintf(stderr, "visu3d: readimage failed\n");
    exit(1);
  }

  zoom = atoi(argv[3]);

  if (! lvisu3d(image1, image2, zoom))
  {
    fprintf(stderr, "visu3d: function lvisu3d failed\n");
    exit(1);
  }

  freeimage(image1);
  freeimage(image2);

  return 0;
} /* main */
