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
/* operateur interactif de visualisation de profils 1D */
/* Michel Couprie - mai 1998 */

#define ZOOM4
/*
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcxbib.h>
#include <lbresen.h>
#include <lprofil.h>

#include <unistd.h>

/* ==================================== */
void afficheprofil(int32_t *profil, int32_t n, int32_t t)
/* ==================================== */
{
  struct xvimage *im;
  uint8_t *F;
  int32_t i, j;
  char filename[32];
  
  im = allocimage(NULL, n, NDG_MAX, 1, VFF_TYP_1_BYTE);
  if (im == NULL)
  {   
    fprintf(stderr, "afficheprofil: allocimage failed\n");
    exit(0);
  }
  F = UCHARDATA(im);

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < profil[i]; j++)
      F[(NDG_MAX - j) * n + i] = NDG_MAX;
    for (j = profil[i]; j <= NDG_MAX; j++)
      F[(NDG_MAX - j) * n + i] = NDG_MIN;
  }
  sprintf(filename, "_%d.profil", t);
  writeimage(im, filename);
  freeimage(im);
  if (fork() == 0) /* fils - processus xv */
  {
    execlp("xv", "xv", filename, NULL);
    perror("xv");
  }

} /* afficheprofil() */

/* ==================================== */
int32_t lprofil(struct xvimage *image)
/* ==================================== */
{
  register int32_t x, y, z;        /* index muet de pixel */
  int32_t debut, fin, xx, yy, i, k, t;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille plan */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  int32_t *lx, *ly;                /* pour la liste de points du profil */
  int32_t *lp;                     /* pour la liste de valeurs du profil */
  uint8_t *ep;                     /* etiquettes des points du profil */
  int32_t np;                      /* le nb de points effectif */
#define NBBUTTONS   2
#define BOXWIDTH  100
#define BOXHEIGHT  20
#define NBLIGBUTT   1
  char * ButtonStrings[NBLIGBUTT][NBBUTTONS];
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT;
  int32_t Width = rs;
  int32_t ImageHeight = cs;
  int32_t ImageWidth = rs;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lprofil: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  lp = (int32_t *)calloc(1,2 * N * sizeof(int32_t));
  lx = (int32_t *)calloc(1,2 * N * sizeof(int32_t));
  ly = (int32_t *)calloc(1,2 * N * sizeof(int32_t));
  if (!lx || !ly || !lp)
  {
    fprintf(stderr, "lprofil: malloc failed\n");
    return 0;
  }

  ep = (uint8_t *)calloc(1,2 * N * sizeof(uint8_t));
  if (ep == NULL)
  {
    fprintf(stderr, "lprofil: malloc failed\n");
    return 0;
  }

#ifdef ZOOM4
  Height += cs;
  Width += rs;
  ImageHeight += cs;
  ImageWidth += rs;
#endif

  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], " REDRAW ");
  strcpy(ButtonStrings[0][1], " QUIT   ");

  debut = fin = -1;
  t = 0;

ReDraw:

  /* send output to the screen */
  for (z = 0; z < N; z++)
  {
    Color(SOURCE[z]/4);
#ifdef ZOOM4
    Point(2*(z%rs), 2*(z/rs));
    Point(2*(z%rs)+1, 2*(z/rs));
    Point(2*(z%rs), 2*(z/rs)+1);
    Point(2*(z%rs)+1, 2*(z/rs)+1);
#else
    Point(z%rs, z/rs);
#endif
  }

  ColToWhite();
  FRectangle(0, ImageHeight, BoxWidth * NBBUTTONS, BoxHeight * NBLIGBUTT);
  ColToBlack();

  for (i = 0; i < NBLIGBUTT; i++) 
  {
    Line(0, ImageHeight + i * BoxHeight, Width, ImageHeight + i * BoxHeight);
    for (k = 0; k < NBBUTTONS; k++) 
    {
      Line(k*BoxWidth, ImageHeight + i*BoxHeight, k*BoxWidth, ImageHeight + (i+1)*BoxHeight);
      String(k*BoxWidth + 3, ImageHeight + (i+1)*BoxHeight - 4, ButtonStrings[i][k]);
    }
  }

  Color(MC_RED);

ReDisplay:

  FlushGraphics();  

  WaitMouseEvent(&xx, &yy);

  if ((xx >= 0) && (yy >= 0) && (xx < ImageWidth) && (yy < ImageHeight))
  {
#ifdef ZOOM4
    xx /= 2;
    yy /= 2;
#endif
    z = yy * rs + xx;
    if (debut == -1) 
    {
      debut = z; 
    }
    else
    { 
      fin = z;
      x = debut % rs;
      y = debut / rs;
      xx = fin % rs;
      yy = fin / rs;

      np = 2*N;
      lbresenlist(x, y, xx, yy, lx, ly, &np);

      /* dessine le profil et stocke les ndg */
      for (i = 0; i < np; i++)
      {
#ifdef ZOOM4
        Point(2*(lx[i]), 2*(ly[i]));
        Point(2*(lx[i])+1, 2*(ly[i]));
        Point(2*(lx[i]), 2*(ly[i])+1);
        Point(2*(lx[i])+1, 2*(ly[i])+1);
#else
        Point(lx[i], ly[i]);
#endif          
        lp[i] = SOURCE[ly[i]*rs + lx[i]];
      } /* for (i = 0; i < np; i++) */

      t++;
      afficheprofil(lp, np, t);
      debut = -1;
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
                case 0: goto ReDraw;
                case 1: goto Terminate;
	      } /* switch(xx) */
    } /* switch(yy) */
  }
goto ReDisplay;

Terminate:
  TerminateGraphics();
  free(lp);
  free(ep);
  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      free(ButtonStrings[i][k]);

  return(1);
}

