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
/* mesh warper */
/* Michel Couprie - decembre 1998 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#ifdef HP
#define _INCLUDE_XOPEN_SOURCE
#endif
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcxbib.h>
#include <mcgeo.h>
#include <lbresen.h>
#include <lvoronoi.h>
#include <lmeshwarp.h>

#define MAXSIZEMESH 5000

/* ==================================== */
int32_t warptriangle(
  triangle * ts,
  triangle * td,
  uint8_t *F,
  uint8_t *H,
  int32_t rspix)
/* ==================================== */
{
  int32_t x, y;
  mat33 m;                              /* matrice de travail */
  mat33 a;                              /* transformation affine dest -> source */
  vec3 b;
  vec3 sol;
  double X, Y;

  if (inittriangle(ts) == 0)
  {
    printf("mauvais triangle source\n");
    return(0);
  }

  if (inittriangle(td) == 0)
  {
    printf("mauvais triangle destination\n");
    return(0);
  }

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
  {
    printf("determinant nul\n");
    return 0;
  }

  a[0][0] = sol[0];     a[0][1] = sol[1];     a[0][2] = sol[2];

  b[0] = ts->ya;
  b[1] = ts->yb;
  b[2] = ts->yc;
  
  if (solsyst3(m, b, sol) == 0)
  {
    printf("determinant nul\n");
    return 0;
  }

  a[1][0] = sol[0];     a[1][1] = sol[1];     a[1][2] = sol[2];
  a[2][0] = 0.0;        a[2][1] = 0.0;        a[2][2] = 1.0;

  /* ------------------------------------------------------------- */
  /* calcul des intensites des points dans le triangle destination */
  /* ------------------------------------------------------------- */
  
  for (x = (int32_t)floor(td->xmin); x <= (int32_t)ceil(td->xmax); x++)
    for (y = (int32_t)floor(td->ymin); y <= (int32_t)ceil(td->ymax); y++)
      if ((danstriangle(td, (double)x, (double)y)) && (F[y * rspix + x] == 0))
      {
        b[0] = (double)x;
        b[1] = (double)y;
        b[2] = 1.0;
        multmat3vec3(a, b, sol);
        X = sol[0];
        Y = sol[1];

        F[y * rspix + x] = H[((int32_t)Y) * rspix + (int32_t)X];
      }
  return 1;
}


/* ==================================== */
int32_t lwarp(
  struct xvimage * in,
  point *S, 
  point *SD, 
  int32_t n, 
  int32_t **v, 
  int32_t *nv, 
  int32_t *ec
)
/* ==================================== */
{
  int32_t x, a, b, c;
  uint8_t *F;
  uint8_t *H;                     /* image de travail */
  int32_t rspix, cspix, N;
  triangle ts;
  triangle td;

  rspix = in->row_size;
  cspix = in->col_size;
  N = rspix * cspix;
  F = UCHARDATA(in);

  H = (uint8_t *)calloc(1,N * sizeof(char));
  if (H == NULL)
  {   fprintf(stderr,"lwarp() : malloc failed for H\n");
      return(0);
  }

  memcpy((void *)H, (void *)F, N*sizeof(char));
  memset(F, 0, N*sizeof(char));
  for (a = 0; a < n; a++)       /* attention : on fait 2 fois le travail ! */
  {
    ts.xa = (int32_t)S[a].x; ts.ya = (int32_t)S[a].y;
    td.xa = (int32_t)SD[a].x; td.ya = (int32_t)SD[a].y;
    b = 0;
    for (c = 1; c < nv[a]; c++)
    {
      ts.xb = (int32_t)S[v[a][b]].x; ts.yb = (int32_t)S[v[a][b]].y;
      ts.xc = (int32_t)S[v[a][c]].x; ts.yc = (int32_t)S[v[a][c]].y;
      td.xb = (int32_t)SD[v[a][b]].x; td.yb = (int32_t)SD[v[a][b]].y;
      td.xc = (int32_t)SD[v[a][c]].x; td.yc = (int32_t)SD[v[a][c]].y;
      (void)warptriangle(&ts, &td, F, H, rspix);
      b = c;
    }
    if (!ec[a])
    {
      c = 0;
      ts.xb = (int32_t)S[v[a][b]].x; ts.yb = (int32_t)S[v[a][b]].y;
      ts.xc = (int32_t)S[v[a][c]].x; ts.yc = (int32_t)S[v[a][c]].y;   
      td.xb = (int32_t)SD[v[a][b]].x; td.yb = (int32_t)SD[v[a][b]].y;
      td.xc = (int32_t)SD[v[a][c]].x; td.yc = (int32_t)SD[v[a][c]].y;  
      (void)warptriangle(&ts, &td, F, H, rspix);
    }
  }
  free(H);
  return 1;
} /* lwarp() */

/* ==================================== */
void desmesh(
  point *S, 
  int32_t n, 
  int32_t **v, 
  int32_t *nv, 
  int32_t *ec)
/* ==================================== */
{
  int32_t i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < nv[i]; j++)
      Line((int32_t)S[i].x, (int32_t)S[i].y, (int32_t)S[v[i][j]].x, (int32_t)S[v[i][j]].y);
} /* desmesh() */

/* ==================================== */
void deshot(
  int32_t hotxpix,
  int32_t hotypix)
/* ==================================== */
{
    /* redessine le point chaud */
    Color(MC_RED);
    Line(hotxpix-1, hotypix, hotxpix+1, hotypix);
    Line(hotxpix, hotypix-1, hotxpix, hotypix+1);
}

/* ==================================== */
void effhot(
  int32_t hotxpix,
  int32_t hotypix)
/* ==================================== */
{
    /* efface le point chaud */
    ColToWhite();
    Line(hotxpix-1, hotypix, hotxpix+1, hotypix);
    Line(hotxpix, hotypix-1, hotxpix, hotypix+1);
}

/* ==================================== */
int32_t LePlusProche(point *S, int32_t n, double x, double y)
/* ==================================== */
{
  int32_t j, k;
  double d, dk;
  point p;

  p.x = x; p.y = y;
  j = 0; 
  d = carredistance(p, S[j]);
  for (k = 0; k < n; k++)
  {
    dk = carredistance(p, S[k]);
    if (dk < d) { d = dk; j = k; }
  } /* for k */
  return j;
} /* LePlusProche() */

/* ==================================== */
int32_t lmeshwarp(
        struct xvimage *image,
        int32_t mrs)
/* ==================================== */
{
  register int32_t i, j, k, z;     /* index muet */
  int32_t xx, yy;                  /* index muet */
  int32_t eventtype;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille plan */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint8_t *SAVE;         /* l'image de sauvegarde */
#define NBBUTTONS   3
#define BOXWIDTH  100
#define BOXHEIGHT  20
#define NBLIGBUTT   4
  char * ButtonStrings[NBLIGBUTT][NBBUTTONS];
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT;
  int32_t Width = rs;
  int32_t ImageHeight = cs;
  int32_t ImageWidth = rs;
  double a = 0.03;
  double d, x, y;
  int32_t showgrid = 1;
#define MODE_EXPAND  0
#define MODE_RETRACT 1
#define MODE_PICK    2
#define MODE_DEPOSE  3
#define MODE_NEWP    4
#define MODE_DELP    5
  int32_t mode = MODE_NEWP;
  int32_t hotx, hoty;
  int32_t hotxpix, hotypix;
  int32_t **voisins = NULL, *nbvois = NULL, *envconv = NULL;
  point *S;
  point *SD;
  int32_t nbmeshpoints = 0;
  int32_t oldnbmeshpoints;
  int32_t picked;
  int32_t affimage = 1;
  int32_t delaunay = 1;
  double s;
  int32_t mcs;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lmeshwarp: cette version ne traite pas les images volumiques\n");
    return 0;
  }
  
  S = (point *)calloc(1,MAXSIZEMESH * sizeof(point));
  SD = (point *)calloc(1,MAXSIZEMESH * sizeof(point));
  if ((S == NULL) || (SD == NULL))
  {   fprintf(stderr, "lmeshwarp() : malloc failed for S\n");
      return(0);
  }

  s = (double)rs / (double)(mrs - 1);
  mcs = (int32_t)((double)cs / (s * sin(M_PI / 3)));

  nbmeshpoints = 0;
  /* calcule la grille initiale */
  for (j = 0; j < mcs; j++)
  {
    if ((j % 2) == 0)
      for (i = 0; i < mrs; i++)
      {
        S[nbmeshpoints].x = (double)((rs-1) * i) / (mrs-1);
        S[nbmeshpoints].y = (double)((cs-1) * j) / (mcs-1);
        nbmeshpoints++;
      }
    else
    {
      S[nbmeshpoints].x = 0.0;
      S[nbmeshpoints].y = (double)((cs-1) * j) / (mcs-1);
      nbmeshpoints++;
      for (i = 0; i < mrs - 1; i++)
      {
        S[nbmeshpoints].x = (double)((rs-1) * i) / (mrs-1) + s / 2;
        S[nbmeshpoints].y = (double)((cs-1) * j) / (mcs-1);
        nbmeshpoints++;
      }
      S[nbmeshpoints].x = (double)(rs - 1);
      S[nbmeshpoints].y = (double)((cs-1) * j) / (mcs-1);      
      nbmeshpoints++;
    }
  }

  for (i = 0; i < nbmeshpoints; i++) SD[i] = S[i];
  oldnbmeshpoints = nbmeshpoints;

  SAVE = (uint8_t *)calloc(1,N * sizeof(char));
  if (SAVE == NULL)
  {   fprintf(stderr, "lmeshwarp() : malloc failed for SAVE\n");
      return(0);
  }

  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], " WARP   ");
  strcpy(ButtonStrings[0][1], " HIDE   ");
  strcpy(ButtonStrings[0][2], " UNDO   ");

  strcpy(ButtonStrings[1][0], " EXPAND ");
  strcpy(ButtonStrings[1][1], " RETRACT");
  strcpy(ButtonStrings[1][2], " PICK   ");

  strcpy(ButtonStrings[2][0], " NEW P  ");
  strcpy(ButtonStrings[2][1], " DEL P  ");
  strcpy(ButtonStrings[2][2], " MK MESH");

  strcpy(ButtonStrings[3][0], " RS MESH");
  strcpy(ButtonStrings[3][1], " REDRAW ");
  strcpy(ButtonStrings[3][2], " QUIT   ");

ReDraw:

  /* send output to the screen */
  if (affimage)
  {
#ifdef DEBUG
fprintf(stderr,"debut affimage\n");
#endif
    for (z = 0; z < N; z++)
    {
      Color(SOURCE[z]/4);
      Point(z%rs, z/rs);
    }
    affimage = 0;
#ifdef DEBUG
fprintf(stderr,"fin affimage\n");
#endif
  }

  if (delaunay)
  {
    if (voisins)
      for (i = 0; i < oldnbmeshpoints; i++) 
        free(voisins[i]);
    if (voisins) free(voisins);
    if (nbvois) free(nbvois);
    if (envconv) free(envconv);
    voisins = NULL; nbvois = envconv = NULL;
#ifdef DEBUG
fprintf(stderr,"debut delaunay\n");
#endif
    ldelaunay(S, nbmeshpoints, &voisins, &nbvois, &envconv);
    oldnbmeshpoints = nbmeshpoints; 
    delaunay = 0;
#ifdef DEBUG
fprintf(stderr,"fin delaunay\n");
#endif
  }

  if (showgrid)   
  {
#ifdef DEBUG
fprintf(stderr,"debut showgrid\n");
#endif
    Color(MC_RED);   desmesh(S, nbmeshpoints, voisins, nbvois, envconv);
    Color(MC_GREEN); desmesh(SD, nbmeshpoints, voisins, nbvois, envconv);
#ifdef DEBUG
fprintf(stderr,"fin showgrid\n");
#endif
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
      Line(k*BoxWidth, ImageHeight + i*BoxHeight, k*BoxWidth, ImageHeight + (i+1)*BoxHeight);
      if ((i == 1) && (k == 0) && (mode == MODE_EXPAND)) Color(MC_RED); else
      if ((i == 1) && (k == 1) && (mode == MODE_RETRACT)) Color(MC_RED); else
      if ((i == 1) && (k == 2) && (mode == MODE_PICK)) Color(MC_RED); else
      if ((i == 1) && (k == 2) && (mode == MODE_DEPOSE)) Color(MC_RED); else
      if ((i == 2) && (k == 0) && (mode == MODE_NEWP)) Color(MC_RED); else
      if ((i == 2) && (k == 1) && (mode == MODE_DELP)) Color(MC_RED);
      String(k*BoxWidth + 3, ImageHeight + (i+1)*BoxHeight - 4, ButtonStrings[i][k]);
      ColToBlack();
    }
  }

  FlushGraphics();  

  eventtype = WaitEvent(&xx, &yy);
  switch(eventtype)
  {
    case KeyPress: 
      switch(xx)
      {
        case key_q: goto Terminate;
        case space: goto ReDraw;
        
#ifdef OLD
        case num1: mode = 1; break;
        case num2: mode = 2; break;
        case num0: mode = 0; break;
        case key_x: rotallx(cube, cubec); copiecube(cube, cubec); affcube(cube);
                    inversecube(cubec, cube); break;
        case key_y: rotally(cube, cubec); copiecube(cube, cubec); affcube(cube);
                    inversecube(cubec, cube); break;
        case key_z: rotallz(cube, cubec); copiecube(cube, cubec); affcube(cube);
                    inversecube(cubec, cube); break;
        case key_l: scanf("%x", &i); initcubes(cube, cubec, i); affcube(cube); break;
        case key_p: printf("%% %x\n", i); latexcube(cube, 0, 40, 20, 6, 6, 4); break;
        case key_u: symyz(cube, cubec); copiecube(cube, cubec); affcube(cube); break;
        case key_v: symx(cube, cubec); copiecube(cube, cubec); affcube(cube); break;
        case key_i: symy(cube, cubec); copiecube(cube, cubec); affcube(cube); break;
        case key_r: symz(cube, cubec); copiecube(cube, cubec); affcube(cube); break;
        case key_s: sym(cube, cubec); copiecube(cube, cubec); affcube(cube); break;
        case key_a:
        case key_w: break;
        default: printf("bad key : %d\n", xx); break;
#endif
      }
      goto ReDisplay;

    case ButtonPress:

  if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight))
  {
    if ((mode == MODE_EXPAND) || (mode == MODE_RETRACT))
    {
     /* modifie la grille */
     for (i = 0; i < nbmeshpoints; i++)
     {
         x = SD[i].x;
         y = SD[i].y;
         d = sqrt((x-(double)xx) * (x-(double)xx) + (y-(double)yy) * (y-(double)yy));
         if (mode == MODE_EXPAND)
         {
           x += (x - (double)xx) / (a*d*d);
           y += (y - (double)yy) / (a*d*d);
           if (x < 0.0) x = 0.0;
           if (y < 0.0) y = 0.0;
	 }
         else
	 {
           x -= (x - (double)xx) / (a*d*d);
           y -= (y - (double)yy) / (a*d*d);
	 }
         if (x > (double)(rs-1)) x = (double)(rs-1);
         if (y > (double)(cs-1)) y = (double)(cs-1);
         SD[i].x = x;
	 SD[i].y = y;
       }
       goto ReDraw;
    }
    if (mode == MODE_PICK)
    {
      picked = LePlusProche(SD, nbmeshpoints, (double)xx, (double)yy);
      hotxpix = (int32_t)SD[picked].x;
      hotypix = (int32_t)SD[picked].y;
      deshot(hotxpix, hotypix);
      strcpy(ButtonStrings[1][2], " DEPOSE ");
      mode = MODE_DEPOSE;
      goto ReDisplay;
    }
    if (mode == MODE_DEPOSE)
    {
      SD[picked].x = (double)xx;
      SD[picked].y = (double)yy;
      strcpy(ButtonStrings[1][2], " PICK   ");
      mode = MODE_PICK;
      if (showgrid)   
      {
        Color(MC_RED);   desmesh(S, nbmeshpoints, voisins, nbvois, envconv);
        Color(MC_GREEN); desmesh(SD, nbmeshpoints, voisins, nbvois, envconv);
      }
      goto ReDisplay;
    }
    if (mode == MODE_NEWP)
    {
      if (nbmeshpoints >= MAXSIZEMESH)
        fprintf(stderr, "lmeshwarp() : trop de points\n");
      else  
      {
        SD[nbmeshpoints].x = S[nbmeshpoints].x = (double)xx;
        SD[nbmeshpoints].y = S[nbmeshpoints].y = (double)yy;
        nbmeshpoints++;
        deshot(xx, yy);
        delaunay = 1;
        goto ReDisplay;
      }
    } /* if (mode == MODE_NEWP) */
    if (mode == MODE_DELP)
    {
      picked = LePlusProche(S, nbmeshpoints, (double)xx, (double)yy);
      hotxpix = (int32_t)S[picked].x;
      hotypix = (int32_t)S[picked].y;
      effhot(hotxpix, hotypix);
      for (i = picked; i < nbmeshpoints-1; i++) SD[i] = S[i] = S[i+1];
      nbmeshpoints-- ; 
      delaunay = 1;
      goto ReDisplay;
    }
  }
  else /* click hors de l'image */
  {
    xx /= BoxWidth;
    yy -= ImageHeight;
    yy /= BoxHeight;

    switch(yy)
    {
      case 0: switch(xx)
              {
                case 0: /* WARP */
                  lwarp(image, S, SD, nbmeshpoints, voisins, nbvois, envconv);
                  for (i = 0; i < nbmeshpoints; i++) S[i] = SD[i];
                  delaunay = 1;
                  affimage = 1;
                  strcpy(ButtonStrings[0][1], " SHOW   ");
                  showgrid = 0;
                  goto ReDraw;
                case 1: /* SHOW / HIDE */
                  if (showgrid) 
                  {
                    strcpy(ButtonStrings[0][1], " SHOW   ");
                    showgrid = 0;
                    affimage = 1;
                  }
                  else
                  { 
                    strcpy(ButtonStrings[0][1], " HIDE   ");
                    showgrid = 1;
		  }
                  goto ReDraw;
                case 2: /* UNDO */
                  goto ReDraw;
	      } /* switch(xx) */
      case 1: switch(xx)
              {
                case 0: mode = MODE_EXPAND; goto ReDisplay;
                case 1: mode = MODE_RETRACT; goto ReDisplay;
                case 2: mode = MODE_PICK; goto ReDisplay;
	      } /* switch(xx) */
      case 2: switch(xx)
              {
                case 0: mode = MODE_NEWP; goto ReDisplay;
                case 1: mode = MODE_DELP; goto ReDisplay;
                case 2: affimage = 1; goto ReDraw;
	      } /* switch(xx) */
      case 3: switch(xx)
              {
                case 0: 
                  /* raz la grille */
                  nbmeshpoints = 0;
                  goto ReDraw;
                case 1: 
                  affimage = 1; goto ReDraw;
                case 2: goto Terminate;
	      } /* switch(xx) */
    } /* switch(yy) */
  } /* else */
      break;

    case ButtonRelease: 
      break;

    case EnterNotify: 
      break;

  } /* switch (eventtype) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();
  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      free(ButtonStrings[i][k]);
  if (voisins)
  {
    for (i = 0; i < nbmeshpoints; i++) 
      free(voisins[i]);
    free(voisins);
  }
  if (nbvois) free(nbvois);
  if (envconv) free(envconv);
  free(S);
  free(SD);
  free(SAVE);
  return(1);
}

