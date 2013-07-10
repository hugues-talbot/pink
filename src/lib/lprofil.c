/* $Id: lprofil.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* operateur interactif de visualisation de profils 1D */
/* Michel Couprie - mai 1998 */

#define ZOOM4
/*
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
/*  #include <malloc.h> !!! HT 20130704 not ISO */
#include <mccodimage.h>
#include <mcimage.h>
#include <mcutil.h>
#include <mcxbib.h>
#include <lbresen.h>
#include <lprofil.h>

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
#ifdef UNIXIO
#  include <unistd.h>
#else /* NOT UNIXIO */
#  include <stdlib.h>
#  include <io.h>
#  include <Time.h>
// The following codeblock is copied from here
// http://social.msdn.microsoft.com/Forums/en/vcgeneral/thread/430449b3-f6dd-4e18-84de-eebd26a8d668
// Note: it is only used in the Microsoft Windows port

#  include <windows.h> 
#  if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#    define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#  else
#    define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#  endif

struct timezone 
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
}; 

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;
  static int tzflag;
 
  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);
 
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
 
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS; 
    tmpres /= 10;  /*convert into microseconds*/
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
 
  if (NULL != tz)
  {
    if (!tzflag)
    {
      _tzset();
      tzflag++;
    }
    tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
 
  return 0;
}

#endif /* NOT UNIXIO */


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
  int32_t *lp;                     /* pour la liste de points du profil */
  uint8_t *ep;           /* etiquettes des points du profil */
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
  if (lp == NULL)
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

      lbresenlist(rs, x, y, xx, yy, lp, &np);

      /* dessine le profil et remplace les coord. des points par les ndg */
      for (i = 0; i < np; i++)
      {
        z = lp[i];
#ifdef ZOOM4
        Point(2*(z%rs), 2*(z/rs));
        Point(2*(z%rs)+1, 2*(z/rs));
        Point(2*(z%rs), 2*(z/rs)+1);
        Point(2*(z%rs)+1, 2*(z/rs)+1);
#else
        Point(z%rs, z/rs);
#endif          
        lp[i] = SOURCE[z];
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

