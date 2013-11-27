/* $Id: lsegmenti.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur interactif de segmentation */
/* utilise une File d'Attente Hierarchique */
/* utilise un arbre des composantes (anciennement nomme captation basin tree, CBT) */
/* Michel Couprie - novembre 1996 */
/* version 3.3 du 25/8/97 */
/*
  20/1/97 : correction de la mesure de la profondeur 
  5/08/97 : affichage en niveaux de gris
  25/O8/97 : connexites 4 ou 8
  01/09/97 : zoom (x 4)
  1/2/98 : - retire   UnSetAll(x);   apres        ncc = 0; x = FahPop(FAH);
           - remplace
               n = FirstSon(CBT, y + 1) - FirstSon(CBT, y);
             par
               if (y < nbcell - 1)
                 n = FirstSon(CBT, y + 1) - FirstSon(CBT, y);
               else
                 n = nbcell - 2 - FirstSon(CBT, y);
           - nouvelle construction de l'arbre : travaille au niveau des plateaux et
             non plus des pixels
  14/3/98 : ajoute le perimetre et l'indice de circularite
            la circularite est definie dans le continu par le rapport :
                                          2
                     4 . PI . Surf / Perim        et vaut 1 pour un disque. 

            en discret, une etude empirique donne les facteurs K1 ou K2 a la place de 4.PI,
            avec K1 = 256/21 pour Perim <= 16, et K2 = 13924/757 sinon.
  18/3/98 : etude de la signature des attributs 
  20/3/98 : ajout de l'attribut "distance a la racine" (RDIST)
  28/3/98 : ajout de l'attribut "nombre de trous" (TROU)
  29/3/98 : corrige le bug de SURF
  29/3/98 : ajout de l'attribut "densite de trous" (DTROU = TROU*10000/SURF)
  08/4/98 : ajout de l'attribut excentricite
  24/5/98 : reconstruction
*/

#define PARANO                 /* even paranoid people have ennemies */
#define VERBOSE

#define VOL2
#define MAXCOND

/*
#define ZOOM4
#define DEBUGSURF
#define TRACECBT
*/

/*
   Methode : 

===============================================
 CONSTRUCTION DU CBT
===============================================
  entree : une image F
  sortie : le CBT, l'application M: Dom(F) -> CBT

  M <- etiquettes des minima de F
  nminima <- nombre de minima de F
  init(CBT, nminima)
  pour tout point x de Dom(F)
    si x appartient a un minimum
      SetData(CBT, M[x], F[x] + 1);
      pour tout y dans gamma4(x)
        si y n'appartient pas a un minimum et pas deja dans FAH
          FahPush(FAH, y, F[y]);
        finsi
      finpour
    finsi
  finpour

  tant que FAH non vide
    x = FahPop(FAH);
    etiqcc = liste des M[y], y dans le voisinage du plateau de x
    diffanc = liste des ancetres differents des etiquettes de etiqcc dans le CBT
    si |diffanc| == 1
      M[x] = first(diffanc)
    sinon
      new = CreateCell(CBT)
      M[x] = new
      SetData(CBT, new, F[x] + 1)
      pour tout a dans diffanc 
        SetFather(CBT, a, new)
      finpour
    fin si
    pour tout y dans gamma4(x) pas deja dans FAH
      si M[y] == 0
        FahPush(FAH, y, F[y]);
      finsi
    finpour
  fintantque

  regularise(CBT) 

===============================================
 SEGMENTATION
===============================================

  en parallele avec la construction du CBT, on a recueilli une "mesure" (MU)
  des bassins versants.

  ici cette mesure est la surface de la plus grande coupe d'un BV avant sa fusion
  avec un autre.

  l'algorithme de segmentation est alors:

  1ERE ETAPE : SELECTIONNE LE NB MAX DE CC PERTINENTES

  init:  ranger au niveau 0 de la FAH les feuilles du CBT

  tant que FAH non vide faire
    k = FAHNIVEAU()
    x = FAHPOP()
    si MU[c] >= seuil alors
      marque les ancetres de x "INVALIDE"
      marque x "PERTINENT"
    sinon
      si pere(x) existe et non INVALIDE alors FAHPUSH(pere(x), k+1) finsi
    finsi
  fintantque

  2EME ETAPE (FACULTATIVE) : MAXIMISE AU SENS DE L'INCLUSION 

  Re-init:  ranger a leur niveau data dans la FAH les cellules PERTINENTes

  I = arbre inverse

  tant que FAH non vide faire
    x = FAHPOP()
    y = pere(x)
    si y existe alors 
      k = FirstSon(y)
      n = FirstSon(y + 1) - FirstSon(y)
      nbfilspert = 0
      pour i de 0 a n-1 faire
        si Label(I[k+i]) == PERTINENT
          nbfilspert++ finsi finpour
      si nbfilspert == 1 alors
        Label(x) = NONMARQUE
        Label(y) = PERTINENT
        FahPush(y, Data(y))
      finsi
    finsi
  fintantque
*/

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcfah.h>
#include <mcindic.h>
#include <mccbt.h>
#include <mctopo.h>
#include <mcxbib.h>
#include <lsegmenti.h>

#define EN_FAH     0
#define MINIMUM    1
#define MARQUE1    2
#define MARQUE2    3
#define MARQUETROU 4

#define NONMARQUE  0
#define INVALIDE   1
#define PERTINENT  2

#define K1 (256.0/21.0)
#define K2 (13924.0/757.0)

// #include <unistd.h> in Microsoft Windows it does not exist, but we only need a subset of it
#ifdef UNIXIO
#  include <unistd.h>
#else /* NOT UNIXIO */
#  include <stdlib.h>
#  include <io.h>
#endif /* NOT UNIXIO */

/* ==================================== */
static void visualise(int32_t tabres[], char * c, int32_t x, int32_t y)
/* ==================================== */
{
  int32_t i, lastv;
  FILE *fd = NULL;
  char filename[32];
/*
  for (i = 0; i < 256; i++)
    if (tabres[i] != -1)
      printf("%d:%d ", i, tabres[i]);
  printf("\n");
*/
  sprintf(filename, "_%s_%d_%d.sign", c, x, y);
  fd = fopen(filename, "w");
  if (!fd)
  {
    fprintf(stderr, "lsegmenti: unable to open file: %s\n", filename);
    exit (0);
  }
  fprintf(fd, "\"%s\n", c);
  fprintf(fd, "%d %d\n", 0, 0);
  lastv = 0;
  for (i = 0; i < 256; i++)
    if (tabres[i] != -1)
    {
      fprintf(fd, "%d %d\n", i, lastv);
      fprintf(fd, "move %d %d\n", i, tabres[i]);
      lastv = tabres[i];
    }
  fprintf(fd, "%d %d\n", 255, lastv);

  if (fork() == 0) /* fils - processus xgraph */
  {
    execlp("xgraph", "xgraph", "=400x180+0+0", "-lw", "4", filename, NULL);
    perror("xgraph");
  }

  fclose(fd);
} /* visualise() */

#ifdef MOMENTS
/* ==================================== */
int32_t excentricity(double mx1, double my1, double mx2, double my2, 
                 double mxy2, int32_t n)
/* ==================================== */
{
  double Mx2, My2, Mxy2, delta;
  double lambda1, lambda2;

  /* moments d'ordre 2 */
  Mx2 = mx2 - mx1 * mx1 / n;  
  My2 = my2 - my1 * my1 / n;  
  Mxy2 = mxy2 - mx1 * my1 / n;

  /* calcul des valeurs propres de la matrice des covariances */
  delta = (Mx2 - My2) * (Mx2 - My2) + 4 * Mxy2 * Mxy2;
  lambda1 = (Mx2 + My2 + sqrt(delta)) / 2.0;
  lambda2 = (Mx2 + My2 - sqrt(delta)) / 2.0;
#ifdef DEBUGEXCEN
  printf("Mx2 = %g ; My2 = %g ; Mxy2 = %g ; delta = %g\n", Mx2, My2, Mxy2, delta);
  printf("lambda1 = %g ; lambda2 = %g\n", lambda1, lambda2);
#endif
  if (lambda2 < 0.0)
  {
    fprintf(stderr, "excentricity : valeur propre negative : %g\n", lambda2);
    return 0;
  }
  return 255 - (int32_t)(lambda2 * 255 / lambda1);
} /* excentricity() */
#endif

/* ==================================== */
static
int32_t circ(int32_t area, int32_t perim)
/* ==================================== */
{
  int32_t val_attrib;
  if (perim <= 30)
    val_attrib = (int32_t)(100 * K1 * (double)area / (double)(perim * perim));
  else
    val_attrib = (int32_t)(100 * K2 * (double)area / (double)(perim * perim));
  if (val_attrib > 100) return 100;
  return val_attrib;
} /* circ() */

/* ==================================== */
static
int32_t dtrou(int32_t area, int32_t trou)
/* ==================================== */
{
  return (10000 * trou) / area;
} /* dtrou() */

/* ==================================== */
static
void signature(int32_t mesure, int32_t X, int32_t Y, int32_t * M, cbtcell * CBT, int32_t rs, 
               int32_t *SURF, int32_t *PROF, int32_t *VOL, int32_t *PER, 
               uint8_t *RDIST, int32_t *TROU, uint8_t *EXCEN)
/* ==================================== */
{
  int32_t tabres[256];
  int32_t i;

  for (i = 0; i < 256; i++) tabres[i] = -1;

  if (mesure & PROFONDEUR)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)] = PROF[i]; i = Father(CBT, i); }
    visualise(tabres, "prof", X, Y);
  }
  if (mesure & SURFACE)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)] = SURF[i]; i = Father(CBT, i); }
    visualise(tabres, "surf", X, Y);
  }
  if (mesure & VOLUME)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)] = VOL[i]; i = Father(CBT, i); }
    visualise(tabres, "vol", X, Y);
  }
  if (mesure & PERIMETRE)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)] = PER[i]; i = Father(CBT, i); }
    visualise(tabres, "perim", X, Y);
  }
  if (mesure & CIRCULARITE)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)]=circ(SURF[i],PER[i]); i=Father(CBT,i); }
    visualise(tabres, "circ", X, Y);
  }
  if (mesure & ROOTDIST)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)]= RDIST[i]; i=Father(CBT,i); }
    visualise(tabres, "rdist", X, Y);
  }
  if (mesure & NBTROUS)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)]= TROU[i]; i=Father(CBT,i); }
    visualise(tabres, "trou", X, Y);
  }
  if (mesure & DENSTROUS)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)]= dtrou(SURF[i],TROU[i]); i=Father(CBT,i); }
    visualise(tabres, "dtrou", X, Y);
  }
#ifdef MOMENTS
  if (mesure & EXCENTRICITE)
  {
    i = M[Y * rs + X];
    while (i != NIL) { tabres[Data(CBT,i)]= EXCEN[i]; i=Father(CBT,i); }
    visualise(tabres, "excen", X, Y);
  }
#endif
} /* signature() */


/* ==================================== */
static
void printcomp(int32_t X, int32_t Y, int32_t * M, cbtcell * CBT, int32_t rs,
               int32_t *SURF, int32_t *PROF, int32_t *VOL, int32_t *PER, 
               uint8_t *RDIST, int32_t *TROU, uint8_t *EXCEN)
/* ==================================== */
{
  int32_t x = M[Y*rs + X];
  printf("niveau %d (%d, point %d,%d) \n", 
          Data(CBT, x), x, X, Y);

  while ((x != NIL) && (Label(CBT,x) == NONMARQUE)) 
    x = Father(CBT,x);
  if (Label(CBT,x) == PERTINENT)
    printf("composante PERTINENTE de niveau %d (%d, point %d,%d) \n", 
            Data(CBT, x), x, X, Y);
  else
    printf("composante NON PERTINENTE de niveau %d (%d, point %d,%d) \n", 
            Data(CBT, x), x, X, Y);
#ifdef MOMENTS
  printf("PROF\tSURF\tVOL(K)\tPER\tCIRC\tRDIST\tTROU\tDTROU\tEXCEN\n");
  printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
          PROF[x], SURF[x], VOL[x]/1000, PER[x], circ(SURF[x],PER[x]), 
          RDIST[x], TROU[x], dtrou(SURF[x],TROU[x]), EXCEN[x]);
#else
  printf("PROF\tSURF\tVOL(K)\tPER\tCIRC\tRDIST\tTROU\tDTROU\n");
  printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
          PROF[x], SURF[x], VOL[x]/1000, PER[x], circ(SURF[x],PER[x]), 
          RDIST[x], TROU[x], dtrou(SURF[x],TROU[x]));
#endif
} /* printcomp() */

/* ==================================== */
int32_t contribperim(int32_t w, uint8_t *F, uint8_t h, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t t4mm, t4m, t8p, t8pp;
  if ((w%rs==rs-1) || (w<rs) || (w%rs==0) || (w>=N-rs)) return 1; /* point de bord */
  nbtopoh(F, w, h, rs, N, &t4mm, &t4m, &t8p, &t8pp);
  return t8pp;  /* les points de T8pp=n sont comptes n fois dans le perimetre */
} /* contribperim() */

/* ==================================== */
int32_t lsegmenti(struct xvimage *image, int32_t connex)
/* ==================================== */
{
  register int32_t i, k, l;        /* index muet */
  register int32_t w, x, y, z;     /* index muet de pixel */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *CONTRIPER;    /* pour le calcul du perimetre */
  uint8_t *TEMP;         /* pour la recuperation de la segmentation */
  int32_t *M;                     /* l'image d'etiquettes de composantes connexes */
  int32_t label;
  int32_t *T;                      /* table de correspondance pour regularisation */
  int32_t *I;                      /* pour l'inversion du CBT */
  int32_t nminima;                 /* nombre de minima differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  Lifo * LIFO;                 /* pour parcourir un plateau */
  cbtcell * CBT;               /* arbre des bassins versants (Catchment Basin Tree) */
  int32_t nbcell;
  int32_t nbmaxcell;
  int32_t *etiqcc;
  int32_t nccmax;
  int32_t ncc;
  int32_t new;
  int32_t prof;
  int32_t *SURF;                   /* pour le stockage de la surface */
  int32_t *PROF;                   /* pour le stockage de la profondeur */
  int32_t *DERPROF;                /* pour le stockage de la derniere profondeur */
  int32_t *VOL;                    /* pour le stockage du volume */
  int32_t *PER;                    /* pour le stockage du perimetre */
  int32_t *TROU;                   /* pour le stockage du nombre de trous */
  uint8_t *RDIST;        /* pour le stockage de la dist. a la racine */
#ifdef MOMENTS
  uint8_t *EXCEN;        /* pour le stockage de l'excentricite */
  double *mx1;                 /* moments */
  double *my1;                 /*    -    */
  double *mx2;                 /*    -    */
  double *my2;                 /*    -    */
  double *mxy2;                /*    -    */
#endif
  int32_t seuilsurf = 1;
  int32_t seuilprof = 0;
  int32_t seuilvol = 0;
  int32_t seuilperim = 0;
  int32_t seuilcirc = 0;
  int32_t seuilrdist = 0;
  int32_t seuiltrou = 0;
  int32_t seuildtrou = 0;
  int32_t seuilexcen = 0;
  int32_t mesure = PROFONDEUR|SURFACE;  /* conjonction des mesures utilisees (defaut: prof+surf) */
  int32_t increment = 1;
  int32_t maximise = 0;
  int32_t conjonction = 1;
  int32_t incr_vois;
  int32_t h;
  int32_t tailleplateau;
#define NBBUTTONS 7
#define NBLIGBUTT 7
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

#ifdef VOL2
  mesure |= VOLUME;
#endif

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lsegmenti: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connex)
  {
    case 4: incr_vois = 2; break;
    case 8: incr_vois = 1; break;
    default: 
      fprintf(stderr, "lsegmenti: mauvaise connexite: %d\n", connex);
      return 0;
  } /* switch (connex) */

#ifdef ZOOM4
  Height += cs;
  Width += rs;
  ImageHeight += cs;
  ImageWidth += rs;
#endif
  if (BoxWidth * NBBUTTONS > Width) Width = BoxWidth * NBBUTTONS;
  
#ifdef COLOR
  InitColorGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */
#else
  InitGraphics(0, 0, /* x, y */
               Width, Height); /* w, h */
#endif

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      ButtonStrings[i][k] = (char *)calloc(1,16);

  strcpy(ButtonStrings[0][0], " SURF");
  sprintf(ButtonStrings[0][1], "%d", seuilsurf);
  strcpy(ButtonStrings[0][2], "  +  ");
  strcpy(ButtonStrings[0][3], "  -  ");
  strcpy(ButtonStrings[0][4], "  1  ");
  strcpy(ButtonStrings[0][5], " 10  ");
  strcpy(ButtonStrings[0][6], " MIN ");

  strcpy(ButtonStrings[1][0], " PROF");
  sprintf(ButtonStrings[1][1], "%d", seuilprof);
  strcpy(ButtonStrings[1][2], "  +  ");
  strcpy(ButtonStrings[1][3], "  -  ");
  strcpy(ButtonStrings[1][4], " 100 ");
  strcpy(ButtonStrings[1][5], " 1000");
  strcpy(ButtonStrings[1][6], "  ET ");

#ifdef VOL2
  strcpy(ButtonStrings[2][0], " VOL ");
  sprintf(ButtonStrings[2][1], "%d", seuilvol);
#endif
#ifdef PER2
  strcpy(ButtonStrings[2][0], " PER ");
  sprintf(ButtonStrings[2][1], "%d", seuilperim);
#endif
#ifdef TROU2
  strcpy(ButtonStrings[2][0], " TROU");
  sprintf(ButtonStrings[2][1], "%d", seuiltrou);
#endif
  strcpy(ButtonStrings[2][2], "  +  ");
  strcpy(ButtonStrings[2][3], "  -  ");
  strcpy(ButtonStrings[2][4], "10000");
  strcpy(ButtonStrings[2][5], " OK  ");
  strcpy(ButtonStrings[2][6], " QUIT");

  strcpy(ButtonStrings[3][0], "RDIST");
  sprintf(ButtonStrings[3][1], "%d", seuilrdist);
  strcpy(ButtonStrings[3][2], "  +  ");
  strcpy(ButtonStrings[3][3], "  -  ");
  strcpy(ButtonStrings[3][4], "     ");
  strcpy(ButtonStrings[3][5], "     ");
  strcpy(ButtonStrings[3][6], "     ");

  strcpy(ButtonStrings[4][0], " CIRC");
  sprintf(ButtonStrings[4][1], "%d", seuilcirc);
  strcpy(ButtonStrings[4][2], "  +  ");
  strcpy(ButtonStrings[4][3], "  -  ");
  strcpy(ButtonStrings[4][4], "     ");
  strcpy(ButtonStrings[4][5], "     ");
  strcpy(ButtonStrings[4][6], "     ");

  strcpy(ButtonStrings[5][0], "DTROU");
  sprintf(ButtonStrings[5][1], "%d", seuildtrou);
  strcpy(ButtonStrings[5][2], "  +  ");
  strcpy(ButtonStrings[5][3], "  -  ");
  strcpy(ButtonStrings[5][4], "     ");
  strcpy(ButtonStrings[5][5], "     ");
  strcpy(ButtonStrings[5][6], "     ");
#ifdef MOMENTS
  strcpy(ButtonStrings[6][0], "EXCEN");
  sprintf(ButtonStrings[6][1], "%d", seuilexcen);
  strcpy(ButtonStrings[6][2], "  +  ");
  strcpy(ButtonStrings[6][3], "  -  ");
  strcpy(ButtonStrings[6][4], "     ");
  strcpy(ButtonStrings[6][5], "     ");
  strcpy(ButtonStrings[6][6], "     ");
#endif
  IndicsInit(N);
  FAH = CreeFahVide(N+1);
  if (FAH == NULL)
  {   fprintf(stderr, "lsegmenti() : CreeFah failed\n");
      return(0);
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lsegmenti() : CreeLifoVide failed\n");
      return(0);
  }

  CONTRIPER = (uint8_t *)calloc(N, sizeof(char));
  if (CONTRIPER == NULL)
  {   fprintf(stderr, "lsegmenti() : calloc failed for CONTRIPER\n");
      return(0);
  }

  /* =================================================== */
  /* CALCUL DES ETIQUETTES DES MINIMA  */
  /* =================================================== */

  M = (int32_t *)calloc(1,N * sizeof(int32_t));
  if (M == NULL)
  {   fprintf(stderr, "lsegmenti() : malloc failed for M\n");
      return(0);
  }

  /* le LABEL M initialement est mis a -1 */
  for (x = 0; x < N; x++) M[x] = -1;

  nminima = 0;

  for (x = 0; x < N; x++)
  {
    if (M[x] == -1)          /* on trouve un point x non etiquete */
    {
      nminima += 1;              /* on cree un numero d'etiquette */
      M[x] = nminima;
      LifoPush(LIFO, x);         /* on va parcourir le plateau auquel appartient x */
      while (! LifoVide(LIFO))
      {
        w = LifoPop(LIFO);
        label = M[w];
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if (y != -1)
          {
            if ((label > 0) && (F[y] < F[w]))
            { /* w non dans un minimum */
              label = 0;
              nminima -= 1;
              M[w] = label;
              LifoPush(LIFO, w);
            } 
            else
            if (F[y] == F[w])
            {
              if (((label > 0) && (M[y] == -1)) ||
                  ((label == 0) && (M[y] != 0)))
              {
                M[y] = label;
                LifoPush(LIFO, y);
              } /* if .. */
            } /* if F ... */
          } /* if (y != -1) */
        } /* for k ... */
      } /* while (! LifoVide(LIFO)) */
    } /* if (M[x] == -1) */
  } /* for (x = 0; x < N; x++) */

  if (nminima < 2)
  {   fprintf(stderr, "lsegmenti() : only 1 minimum - image probably empty\n");
      return(0);
  }
  nminima += 1; /* pour le niveau 0 */

  /* ================================================ */
  /*                CONSTRUCTION DU CBT               */
  /* ================================================ */

  /* initialisation */
  /* ============== */

  nbmaxcell = nminima * 2;
  CBT = (cbtcell *)calloc(1,nbmaxcell * sizeof(cbtcell));
  if (CBT == NULL)
  {   fprintf(stderr, "lsegmenti() : malloc failed for CBT\n");
      return(0);
  }

  SURF    = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  PROF    = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  DERPROF = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  VOL     = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  PER     = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  TROU    = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  RDIST   = (uint8_t *)calloc(1,nbmaxcell * sizeof(char));
  if ((SURF==NULL) || (PROF==NULL) || (DERPROF==NULL) || (VOL==NULL) || 
      (PER==NULL) || (TROU==NULL) || (RDIST==NULL))
  {   fprintf(stderr, "lsegmenti() : calloc failed\n");
      return(0);
  }
#ifdef MOMENTS
  EXCEN   = (uint8_t *)calloc(1,nbmaxcell * sizeof(char));
  if (EXCEN==NULL)
  {   fprintf(stderr, "lsegmenti() : calloc failed\n");
      return(0);
  }
  mx1  = (double *)calloc(nbmaxcell, sizeof(double));   /* init a 0 */
  my1  = (double *)calloc(nbmaxcell, sizeof(double));   /* init a 0 */
  mx2  = (double *)calloc(nbmaxcell, sizeof(double));   /* init a 0 */
  my2  = (double *)calloc(nbmaxcell, sizeof(double));   /* init a 0 */
  mxy2 = (double *)calloc(nbmaxcell, sizeof(double));   /* init a 0 */
  if ((mx1==NULL) || (my1==NULL) || (mx2==NULL) || (my2==NULL) || (mxy2==NULL))
  {   fprintf(stderr, "lsegmenti() : calloc failed\n");
      return(0);
  }
#endif
  /* initialise les attributs des minima */
  for (x = 0; x < N; x++) 
    if (M[x]) 
    {
      SURF[M[x]] += 1;
      DERPROF[M[x]] = F[x] + 1;
#ifdef MOMENTS
      mx1[M[x]] += x%rs; 
      my1[M[x]] += x/rs; 
      mx2[M[x]] += (x%rs) * (x%rs); 
      my2[M[x]] += (x/rs) * (x/rs);
      mxy2[M[x]] += (x%rs) * (x/rs);
#endif
    }
  for (x = 0; x < N; x++) /* pour le perimetre et les trous */
  {
    if (M[x] > 0)
    {
      CONTRIPER[x] = contribperim(x, F, F[x], rs, N);
      if (!IsSet(x, MARQUETROU))
      {
        int32_t nbtrous = 0;
        Set(x, MARQUETROU);
        LifoPush(LIFO, x); /* on va parcourir le plateau auquel appartient x */
        while (! LifoVide(LIFO))
        {
          w = LifoPop(LIFO);
          for (k = 0; k < 8; k += incr_vois)
          {
            y = voisin(w, k, rs, N);
            if ((y != -1) && (M[y] == M[x]) && (!IsSet(y, MARQUETROU)))
            {
              int32_t masque = 0, imasque = 1;
              /* fabrique le masque des voisins de y MARQUES */
              for (l = 0; l < 8; l += 1)
              {
                z = voisin(y, l, rs, N);
                if ((z != -1) && (M[z] == M[y]) && IsSet(z, MARQUETROU))
                  masque |= imasque; 
                imasque = imasque << 1;
              } /* for l ... */
              if (connex == 4) 
                { nbtrous += (t4(masque) - 1); if (t8b(masque) == 0) nbtrous--; }
              else
                { nbtrous += (t8(masque) - 1); if (t4b(masque) == 0) nbtrous--; }
              Set(y, MARQUETROU);
              LifoPush(LIFO, y);
            } /* if (!IsSet(x, MARQUETROU)) */
          } /* for k ... */
        } /* while (! LifoVide(LIFO)) */
        TROU[M[x]] = nbtrous;
      } /* if (!IsSet(x, MARQUETROU) */
    } /* if (M[x] > 0) */
  } /* for (x = 0; x < N; x++) */
  for (x = 0; x < N; x++) if (M[x]) PER[M[x]] += CONTRIPER[x];

  nccmax = 2*N / 3;  /* majore la longueur de la frontiere exterieure d'un plateau */
  etiqcc = (int32_t *)calloc(1,nccmax * sizeof(int32_t));
  if (etiqcc == NULL)
  {   fprintf(stderr, "lsegmenti() : malloc failed for etiqcc\n");
      return(0);
  }

  nbcell = nminima;
  InitCbt(CBT, nbcell);

  /* init. inondation */
  /* ================ */
  
  FahPush(FAH, -1, 0);               /* force la creation du niveau 0 dans la Fah. */
                                     /* NECESSAIRE pour eviter la creation prematuree */
                                     /* de la file d'urgence */ 
  for (x = 0; x < N; x++)
  {
    if (M[x] != 0)                   /* le pixel appartient a un minimum */
    {
      Set(x, MINIMUM);
      SetData(CBT, M[x], F[x]);     /* conceptuellement : F[x] + 1 */
      for (k = 0; k < 8; k += incr_vois)
      {                              /* si un voisin n'est pas deja dans la FAH */
        y = voisin(x, k, rs, N);     /* et n'est pas dans un minimum, on le met en FAH */
        if ((y != -1) && (! IsSet(y, EN_FAH)) && (M[y] == 0))
	{
          FahPush(FAH, y, F[y]);
          Set(y, EN_FAH);
        } /* if y */
      } /* for k */
    } /* if (M[x] != 0) */
  } /* for x */

  x = FahPop(FAH);
#ifdef PARANO
  if (x != -1)
  {   
     printf("segmenti() : ORDRE FIFO NON RESPECTE PAR LA FAH !!!\n");
     return(0);
  }
#endif

  /* ================================================================= */
  /* INONDATION */
  /* ================================================================= */

  while (! FahVide(FAH))
  {
    x = FahPop(FAH);

    if (M[x] == 0)
    {
      ncc = 0;
      h = F[x];
      tailleplateau = 0;
      /* 1er parcours du plateau de x : enregistre les etiquettes voisines */
      /* les points du plateau sont marques (MARQUE1) */
      /* les points voisins du plateau sont marques (MARQUE2) */

      Set(x, MARQUE1);
      LifoPush(LIFO, x);
      while (! LifoVide(LIFO))
      {
        w = LifoPop(LIFO);     /* w est un point du h-plateau */
        tailleplateau++;
        for (k = 0; k < 8; k += incr_vois)
        {
          y = voisin(w, k, rs, N);
          if (y != -1)
          {
            if  ((M[y] != 0) && (!IsSet(y, MARQUE2)))
            {
              if (ncc >= nccmax)
              {   
                printf("segmenti() : ncc = %d ; nccmax = %d\n", ncc, nccmax);
                return(0);
              }
              etiqcc[ncc] = M[y];
              ncc += 1;
              Set(y, MARQUE2);
            }
            if ((F[y] == h) && (!IsSet(y, MARQUE1)))
            {
              Set(y, MARQUE1);
              LifoPush(LIFO, y);
            }
          } /* if (y != -1) */
        } /* for (k = 0; k < 8; k += incr_vois) */
      } /* while (! LifoVide(LIFO)) */
      ncc = DiffAnc(CBT, ncc, etiqcc);

#ifdef PARANO
      if (ncc == 0)
      {
        printf("segmenti() : construction cbt : PAS DE cc DANS LE VOISINAGE !!!\n"); 
        return(0);  
      } else
#endif
      if (ncc == 1)
      {
        /* 2nd parcours du plateau de x : positionne les etiquettes M */
        /* les points du plateau sont demarques (MARQUE1) */
        /* les points voisins du plateau sont demarques (MARQUE2) */
        /* les voisins du plateau sont eventuellement mis en FAH */

        UnSet(x, MARQUE1);
        LifoPush(LIFO, x);
        while (! LifoVide(LIFO))
        {
          int32_t masque = 0, imasque = 1;

          w = LifoPop(LIFO);     /* w est un point du h-plateau */
          M[w] = etiqcc[0];
          for (k = 0; k < 8; k += incr_vois)
          {
            y = voisin(w, k, rs, N);
            if (y != -1)
            {
              UnSet(y, MARQUE2);
              if ((! IsSet(y, EN_FAH)) && (M[y] == 0) && (!IsSet(y, MARQUE1)))
              {
                FahPush(FAH, y, F[y]);
                Set(y, EN_FAH);
              }
              if ((F[y] == h) && (IsSet(y, MARQUE1)))
              {
                UnSet(y, MARQUE1);
                LifoPush(LIFO, y);
              }
            } /* if (y != -1) */
          } /* for (k = 0; k < 8; k += incr_vois) */

          /* mise a jour du perimetre pour w */
          for (k = 0; k < 8; k += 1)
          {
            y = voisin(w, k, rs, N);
            if (y != -1)
            {
              if (F[y] < h)
              {
#ifdef DEBUGPERI
printf("ncc=%d h=%d p=%d,%d M=%d vois=%d,%d -%d", ncc, h, w%rs, w/rs, M[w], y%rs, y/rs, CONTRIPER[y]);
#endif
                PER[M[w]] -= CONTRIPER[y];   /* mise a jour du perimetre */
                CONTRIPER[y] = contribperim(y, F, h, rs, N);
#ifdef DEBUGPERI
printf(" +%d\n", CONTRIPER[y]);
#endif
                PER[M[w]] += CONTRIPER[y];
              }
            } /* if (y != -1) */
          } /* for (k = 0; k < 8; k += 1) */

          CONTRIPER[w] = contribperim(w, F, h, rs, N);
#ifdef DEBUGPERI
printf("p=%d,%d +%d\n", w%rs, w/rs, CONTRIPER[w]);
#endif
          PER[M[w]] += CONTRIPER[w]; 

          /* mise a jour du nombre de trous pour w */
          /* fabrique le masque des voisins de w appartenant a la composante */
          for (k = 0; k < 8; k += 1)
          {
            y = voisin(w, k, rs, N);
            if ((y != -1) && IsAnc(CBT, M[w], M[y]))
              masque |= imasque; 
            imasque = imasque << 1;
          } /* for k ... */
          if (connex == 4) 
            { TROU[M[w]] += (t4(masque) - 1); if (t8b(masque) == 0) TROU[M[w]]--; }
          else
            { TROU[M[w]] += (t8(masque) - 1); if (t4b(masque) == 0) TROU[M[w]]--; }
#ifdef DEBUGTROU
printf("ncc=%d: M=%d, p=%d,%d h=%d masque=%x t4=%d t8b=%d, nbtrous=%d\n", ncc, M[w], w%rs, w/rs, F[w], masque, t4(masque), t8b(masque), TROU[M[w]]);
#endif

#ifdef MOMENTS
          /* mise a jour des moments pour w */
          mx1[M[w]] += w%rs; 
          my1[M[w]] += w/rs; 
          mx2[M[w]] += (w%rs) * (w%rs); 
          my2[M[w]] += (w/rs) * (w/rs);
          mxy2[M[w]] += (w%rs) * (w/rs);
#endif
        } /* while (! LifoVide(LIFO)) */

        /* mise a jour de la surface, profondeur, volume pour le plateau */
        SURF[M[w]] += tailleplateau;
#ifdef DEBUGSURF
printf("pl. h=%d; ncc = %d; M=%d; taille =%d\n", h, ncc, M[w], tailleplateau);
#endif
        if (F[w] + 1 > DERPROF[M[w]])
        {
          VOL[M[w]] += SURF[M[w]] * (F[w] + 1 - DERPROF[M[w]]);
          DERPROF[M[w]] = F[w] + 1;
        }
#ifdef MOMENTS
        EXCEN[M[w]] = excentricity(mx1[M[w]], my1[M[w]], mx2[M[w]], 
                                   my2[M[w]], mxy2[M[w]], SURF[M[w]]);
#endif
      } /* if (ncc == 1) */
      else
      {
        int32_t p = 0;                   /* pour la mise a jour du perimetre */
        new = CreateCell(CBT, &nbcell, nbmaxcell);
        SetData(CBT, new, F[x]);     /* conceptuellement : F[x] + 1 */
        for (i = 0; i < ncc; i++) SetFather(CBT, etiqcc[i], new);

        /* 2nd parcours du plateau de x : positionne les etiquettes M */
        /* les points du plateau sont demarques (MARQUE1) */
        /* les points voisins du plateau sont demarques (MARQUE2) */
        /* les voisins du plateau sont eventuellement mis en FAH */

        UnSet(x, MARQUE1);
        LifoPush(LIFO, x);
        while (! LifoVide(LIFO))
        {
          int32_t masque = 0, imasque = 1;

          w = LifoPop(LIFO);     /* w est un point du h-plateau */
          UnSet(w, MARQUE1);
          M[w] = new;
          for (k = 0; k < 8; k += incr_vois)
          {
            y = voisin(w, k, rs, N);
            if (y != -1)
            {
              UnSet(y, MARQUE2);
              if ((! IsSet(y, EN_FAH)) && (M[y] == 0) && (!IsSet(y, MARQUE1)))
              {
                FahPush(FAH, y, F[y]);
                Set(y, EN_FAH);
              }
              if ((F[y] == h) && (IsSet(y, MARQUE1)))
              {
                UnSet(y, MARQUE1);
                LifoPush(LIFO, y);
              }
            } /* if (y != -1) */
          } /* for (k = 0; k < 8; k += incr_vois) */

          /* mise a jour du perimetre pour w */
          for (k = 0; k < 8; k += 1)
          {
            y = voisin(w, k, rs, N);
            if (y != -1)
            {
              if (F[y] < h)
              {
#ifdef DEBUGPERI
printf("ncc=%d h=%d p=%d,%d M=%d vois=%d,%d -%d", ncc, h, w%rs, w/rs, M[w], y%rs, y/rs, CONTRIPER[y]);
#endif
                p -= CONTRIPER[y];   /* mise a jour du perimetre */
                CONTRIPER[y] = contribperim(y, F, h, rs, N);
#ifdef DEBUGPERI
printf(" +%d\n", CONTRIPER[y]);
#endif
                p += CONTRIPER[y];
              }
            } /* if (y != -1) */
          } /* for (k = 0; k < 8; k += 1) */
          CONTRIPER[w] = contribperim(w, F, h, rs, N);
#ifdef DEBUGPERI
printf("p=%d,%d +%d\n", w%rs, w/rs, CONTRIPER[w]);
#endif
          p += CONTRIPER[w]; 

          /* mise a jour du nombre de trous pour w */
          /* fabrique le masque des voisins de w appartenant a la composante */
          for (k = 0; k < 8; k += 1)
          {
            y = voisin(w, k, rs, N);
            if ((y != -1) && IsAnc(CBT, new, M[y]))
              masque |= imasque; 
            imasque = imasque << 1;
          } /* for k ... */
          if (connex == 4) 
            { TROU[new] += (t4(masque) - 1); if (t8b(masque) == 0) TROU[new]--; }
          else
            { TROU[new] += (t8(masque) - 1); if (t4b(masque) == 0) TROU[new]--; }
#ifdef DEBUGTROU
printf("ncc=%d: M=%d, p=%d,%d h=%d masque=%x t4=%d t8b=%d, nbtrous=%d\n", ncc, new, w%rs, w/rs, F[w], masque, t4(masque), t8b(masque), TROU[new]);
#endif
#ifdef MOMENTS
          /* mise a jour des moments pour w */
          mx1[new] += w%rs; 
          my1[new] += w/rs; 
          mx2[new] += (w%rs) * (w%rs); 
          my2[new] += (w/rs) * (w/rs);
          mxy2[new] += (w%rs) * (w/rs);
#endif
        } /* while (! LifoVide(LIFO)) */

        PER[M[x]] = p;
#ifdef DEBUGPERI
printf("ncc=%d M=%d PER=%d\n", ncc, M[w], p);
#endif
        SURF[M[x]] = tailleplateau;
#ifdef DEBUGSURF
printf("pl. h=%d; ncc = %d; M=%d; taille =%d\n", h, ncc, M[x], tailleplateau);
#endif
        prof = 0;
        for (i = 0; i < ncc; i++)
        {
          SURF[M[x]] += SURF[etiqcc[i]];
#ifdef MOMENTS
          mx1[M[x]] += mx1[etiqcc[i]];
          my1[M[x]] += my1[etiqcc[i]];
          mx2[M[x]] += mx2[etiqcc[i]];
          my2[M[x]] += my2[etiqcc[i]];
          mxy2[M[x]] += mxy2[etiqcc[i]];
#endif
#ifdef DEBUGSURF
printf("   ajoute %d venant de %d\n", SURF[etiqcc[i]], etiqcc[i]);
#endif
          k = PROF[etiqcc[i]]+F[x]-Data(CBT,etiqcc[i]); /* conceptuellement : F[x] + 1 */
          if (k > prof) prof = k;
          if (F[x] + 1 > DERPROF[etiqcc[i]])
          {
            VOL[etiqcc[i]] += SURF[etiqcc[i]] * (F[x] + 1 - DERPROF[etiqcc[i]]);
            DERPROF[etiqcc[i]] = F[x] + 1;
          }
          VOL[M[x]] += VOL[etiqcc[i]];
          TROU[M[x]] += TROU[etiqcc[i]];
#ifdef DEBUGTROU
printf("ncc=%d M=%d TROU += %d (venant de %d)\n", ncc, M[x], TROU[etiqcc[i]], etiqcc[i]);
#endif
          PER[M[x]] += PER[etiqcc[i]];
#ifdef DEBUGPERI
printf("ncc=%d M=%d PER += %d (venant de %d)\n", ncc, M[x], PER[etiqcc[i]], etiqcc[i]);
#endif
        } /* for i */
        PROF[M[x]] = prof;
        DERPROF[M[x]] = F[x] + 1;
        TROU[M[x]] -= (ncc - 1);
#ifdef DEBUGTROU
printf("ncc=%d M=%d TROU[M[x]] = %d\n", ncc, M[x], TROU[M[x]]);
#endif
#ifdef MOMENTS
        EXCEN[M[x]] = excentricity(mx1[M[x]], my1[M[x]], mx2[M[x]], 
                                   my2[M[x]], mxy2[M[x]], SURF[M[x]]);
#endif
      } /* else if (ncc == 1) */
    } /* if (!IsSet(x, TRAITE)) */
  } /* while (! FahVide(FAH)) */
  /* FIN INONDATION */

  free(etiqcc);
  free(CONTRIPER);
#ifdef MOMENTS
  free(mx1);
  free(my1);
  free(mx2);
  free(my2);
  free(mxy2);
#endif
  FahFlush(FAH);

#ifdef VERBOSE
  printf("FIN DE LA CONSTRUCTION DU CBT\n");
#endif

  T = Regularise(CBT, nminima, nbcell);

  for (x = 0; x < N; x++)
    if ((M[x] >= nminima) && (T[M[x] - nminima]))
    {
      M[x] = T[M[x] - nminima]; /* nouvelle etiquette pour le point x */
    }

#ifdef VERBOSE
  printf("FIN DE LA REGULARISATION DU CBT\n");
#endif

  free(T);

#ifdef PARANO
  for (i = 0; i < nbcell; i++)
    if (Label(CBT,i)) printf("ERROR : label = %d\n", Label(CBT, i));
#endif

#ifdef TRACECBT
    PrintCbt(CBT, nbcell);
    for (i = 0; i < N; i++)
    {
      if (i % rs == 0) printf("\n");
      printf("%3d ", M[i]);
    }
    printf("\n");
    printf("SURF = ");
    for (i = 1; i < nbcell; i++)
      printf("%3d ", SURF[i]);
    printf("\n");
#endif

  /* ================================================ */
  /* CALCUL DE RDIST                               */
  /* ================================================ */

  I = InverseCBT(CBT, nminima, nbcell);

  /* calcule l'attribut "distance a la racine RDIST" */
  /* methode : propogation descendante, en largeur, a partir de la racine. */

  FahFlush(FAH);
  FahPush(FAH, nbcell - 1, 0);   /* empile la racine */
  RDIST[nbcell - 1] = 0;

  while (! FahVide(FAH))
  {
    int32_t n;
    x = FahPop(FAH);
    k = FirstSon(CBT, x);
    if (k != NIL)
    {
      if (x < nbcell - 1)              /* x non racine */
        n = FirstSon(CBT, x + 1) - FirstSon(CBT, x);
      else
        n = nbcell - 2 - FirstSon(CBT, x);
      for (i = 0; i < n; i++)
      {
        FahPush(FAH, I[k + i], 0);
        RDIST[ I[k + i] ] = RDIST[x] + 1;
      } 
    } /* if (k != NIL) */
  } /* while (! FahVide(FAH)) */

  /* ================================================ */
  /* SEGMENTATION                               */
  /* ================================================ */

  /* pour la mesure de la profondeur, il faut rajouter la difference de niveau avec le pere */
  /* on remplace donc les PROF dans l'arbre */
  for (x = 1; x < nbcell; x++) UnSetAll(x);
  FahFlush(FAH);
  for (i = 1; i < nminima+1; i++) FahPush(FAH, i, 0);
  while (! FahVide(FAH))
  {
    k = FahNiveau(FAH);
    x = FahPop(FAH);
    y = Father(CBT,x);
    if (y != NIL)
    { 
      PROF[x] += ((Data(CBT,y) - 1) - Data(CBT,x));
      if (!IsSet(y, EN_FAH))
      { 
        Set(y, EN_FAH);
        FahPush(FAH, y, k + 1); 
      }
    }
    else PROF[x] = NDG_MAX - NDG_MIN; /* prof. maximum pour la racine */
  } /* while (! FahVide(FAH)) */

ReSegment:
  /* init:  ranger au niveau 0 de la FAH les feuilles du CBT */
  FahFlush(FAH);
  for (i = 0; i < nbcell; i++) Label(CBT,i) = NONMARQUE;
  for (i = 1; i < nminima+1; i++) FahPush(FAH, i, 0);

  if (conjonction)
  {
    while (! FahVide(FAH))
    {
      k = FahNiveau(FAH);
      x = FahPop(FAH);
      y = Father(CBT,x);
  
      if (Label(CBT,x) == NONMARQUE)
      {
        if (((SURF[x] >= seuilsurf) || (!(mesure & SURFACE))) &&
            ((PROF[x] >= seuilprof) || (!(mesure & PROFONDEUR))) &&
            ((PER[x] >= seuilperim) || (!(mesure & PERIMETRE))) &&
            ((TROU[x] >= seuiltrou) || (!(mesure & NBTROUS))) &&
            ((RDIST[x] <= seuilrdist) || (!(mesure & ROOTDIST))) &&
#ifdef MOMENTS
            ((EXCEN[x] >= seuilexcen) || (!(mesure & EXCENTRICITE))) &&
#endif
            ((dtrou(SURF[x], TROU[x]) <= seuildtrou) || (!(mesure & DENSTROUS))) &&
            ((circ(SURF[x], PER[x]) >= seuilcirc) || (!(mesure & CIRCULARITE))) &&
            ((VOL[x] >= seuilvol) || (!(mesure & VOLUME))))
        {
          Label(CBT,x) = PERTINENT;
          while ((y != NIL) && (Label(CBT,y) != INVALIDE))
          {
            Label(CBT,y) = INVALIDE;
            y = Father(CBT,y);
          } /* while ((y != NIL) && (Label(CBT,y) != INVALIDE)) */
        } /* if (MU[x] >= seuil) */
        else
          if ((y != NIL) && (Label(CBT,y) != INVALIDE))
            FahPush(FAH, y, k + 1);
      } /* if (Label(CBT,x) != INVALIDE) */
    } /* while (! FahVide(FAH)) */
  } /* if (conjonction) */
  else
  {
    while (! FahVide(FAH))
    {
      k = FahNiveau(FAH);
      x = FahPop(FAH);
      y = Father(CBT,x);
  
      if (Label(CBT,x) == NONMARQUE)
      {
        if (((SURF[x] >= seuilsurf) && (mesure & SURFACE)) ||
            ((PROF[x] >= seuilprof) && (mesure & PROFONDEUR)) ||
            ((PER[x] >= seuilperim) && (!(mesure & PERIMETRE))) ||
            ((TROU[x] >= seuilperim) && (!(mesure & NBTROUS))) ||
            ((RDIST[x] <= seuilrdist) && (!(mesure & ROOTDIST))) ||
#ifdef MOMENTS
            ((EXCEN[x] >= seuilexcen) && (!(mesure & EXCENTRICITE))) ||
#endif
            ((dtrou(SURF[x], TROU[x]) <= seuildtrou) && (!(mesure & DENSTROUS))) ||
            ((circ(SURF[x], PER[x]) >= seuilcirc) && (!(mesure & CIRCULARITE))) ||
            ((VOL[x] >= seuilvol) && (mesure & VOLUME)))
        {
          Label(CBT,x) = PERTINENT;
          while ((y != NIL) && (Label(CBT,y) != INVALIDE))
          {
            Label(CBT,y) = INVALIDE;
            y = Father(CBT,y);
          } /* while ((y != NIL) && (Label(CBT,y) != INVALIDE)) */
        } /* if (MU[x] >= seuil) */
        else
          if ((y != NIL) && (Label(CBT,y) != INVALIDE))
            FahPush(FAH, y, k + 1);
      } /* if (Label(CBT,x) != INVALIDE) */
    } /* while (! FahVide(FAH)) */
  } /* else if (conjonction) */

  if (maximise)
  {
    /*  2EME ETAPE : MAXIMISE AU SENS DE L'INCLUSION */

    FahFlush(FAH);
    for (x = 1; x < nbcell - 1; x++)   /* pas la racine */
      if (Label(CBT, x) == PERTINENT)
        FahPush(FAH, x, Data(CBT, x)); /* empile les composantes pertinentes */

    while (! FahVide(FAH))
    {
      int32_t n, nbfilspert;
  
      x = FahPop(FAH);
      y = Father(CBT, x);
      k = FirstSon(CBT, y);
      if (y < nbcell - 1)              /* pas la racine */
        n = FirstSon(CBT, y + 1) - FirstSon(CBT, y);
      else
        n = nbcell - 2 - FirstSon(CBT, y);
      nbfilspert = 0;
      for (i = 0; i < n; i++)
        if ((Label(CBT, I[k+i]) == PERTINENT) || (Label(CBT, I[k+i]) == INVALIDE))
          nbfilspert++;
#ifdef MAXCOND
      if ((nbfilspert == 1)  /* si y, le pere de x, n'a pas d'autre fils pertinent */
          && (circ(SURF[y], PER[y]) >= seuilcirc)) /* et s'il est circulaire */
#else
      if (nbfilspert == 1)  /* si y, le pere de x, n'a pas d'autre fils pertinent */
#endif
      {                     /* alors demarque x et marque y pertinent */
        Label(CBT, x) = NONMARQUE;
        Label(CBT, y) = PERTINENT;
        FahPush(FAH, y, Data(CBT, y));
      } /* if (nbfilspert == 1) */
    } /* while (! FahVide(FAH)) */
  } /* if (maximise) */

/* RECUPERATION DE LA SEGMENTATION */

  TEMP = (uint8_t *)calloc(1,N * sizeof(char));
  if (TEMP == NULL)
  {   fprintf(stderr, "lsegmenti() : malloc failed for TEMP\n");
      return(0);
  }

  for (i = 0; i < N; i++)
  {
    x = M[i];

    while ((x != NIL) && (Label(CBT,x) == NONMARQUE)) 
      x = Father(CBT,x);
    if (Label(CBT,x) == PERTINENT)
      TEMP[i] = 255;
    else
      TEMP[i] = 0;
  } /* for x */  

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
    | SURF | 111  |  +   |  -   |  1   |  10  | MAX  |        BoxHeight
    --------------------------------------------------
    | PROF | 111  |  +   |  -   | 100  | 1000 |  ET  |        BoxHeight
    --------------------------------------------------
    | VOL  | 111  |  +   |  -   | 10000|  OK  | QUIT |        BoxHeight
    --------------------------------------------------
    | RDIST| 111  |  +   |  -   |      |      |      |        BoxHeight
    --------------------------------------------------
    | CIRC | 111  |  +   |  -   |      |      |      |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

  /* send output to the screen */
#ifdef COLOR
  for (x = 0; x < N; x++)
  {
    if (TEMP[x])    
      Color(MC_RED);
    else
      Color(F[x]/4);
#ifdef ZOOM4
    Point(2*(x%rs), 2*(x/rs));
    Point(2*(x%rs)+1, 2*(x/rs));
    Point(2*(x%rs), 2*(x/rs)+1);
    Point(2*(x%rs)+1, 2*(x/rs)+1);
#else
    Point(x%rs, x/rs);
#endif
  }
#else
  ColToWhite();
  FRectangle(0, 0, ImageWidth, ImageHeight);
  ColToBlack();
  for (x = 0; x < N; x++)
    if (TEMP[x])
    {
#ifdef ZOOM4
      Point(2*(x%rs), 2*(x/rs));
      Point(2*(x%rs)+1, 2*(x/rs));
      Point(2*(x%rs), 2*(x/rs)+1);
      Point(2*(x%rs)+1, 2*(x/rs)+1);
#else
      Point(x%rs, x/rs);
#endif
    }
#endif

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

  switch (increment)
  {
    case 1: FRectangle(4 * BoxWidth, ImageHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, ImageHeight + BoxHeight - 2, ButtonStrings[0][4]);
            ColToBlack();
            break;
   case 10: FRectangle(5 * BoxWidth, ImageHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, ImageHeight + BoxHeight - 2, ButtonStrings[0][5]);
            ColToBlack();
            break;
  case 100: FRectangle(4 * BoxWidth, ImageHeight + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, ImageHeight + 2*BoxHeight - 2, ButtonStrings[1][4]);
            ColToBlack();
            break;
 case 1000: FRectangle(5 * BoxWidth, ImageHeight + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, ImageHeight + 2*BoxHeight - 2, ButtonStrings[1][5]);
            ColToBlack();
            break;
case 10000: FRectangle(4 * BoxWidth, ImageHeight + 2*BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, ImageHeight + 3*BoxHeight - 2, ButtonStrings[2][4]);
            ColToBlack();
            break;
  } /* switch (increment) */

  if (mesure & SURFACE)
  {
    FRectangle(0, ImageHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + BoxHeight - 2, ButtonStrings[0][0]);
    ColToBlack();
  }
  if (mesure & PROFONDEUR)
  {
    FRectangle(0, ImageHeight + BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 2*BoxHeight - 2, ButtonStrings[1][0]);
    ColToBlack();
  }
#ifdef VOL2
  if (mesure & VOLUME)
#endif
#ifdef PER2
  if (mesure & PERIMETRE)
#endif
#ifdef TROU2
  if (mesure & NBTROUS)
#endif
  {
    FRectangle(0, ImageHeight + 2*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 3*BoxHeight - 2, ButtonStrings[2][0]);
    ColToBlack();
  }
  if (mesure & ROOTDIST)
  {
    FRectangle(0, ImageHeight + 3*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 4*BoxHeight - 2, ButtonStrings[3][0]);
    ColToBlack();
  }
  if (mesure & CIRCULARITE)
  {
    FRectangle(0, ImageHeight + 4*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 5*BoxHeight - 2, ButtonStrings[4][0]);
    ColToBlack();
  }
  if (mesure & DENSTROUS)
  {
    FRectangle(0, ImageHeight + 5*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 6*BoxHeight - 2, ButtonStrings[5][0]);
    ColToBlack();
  }
#ifdef MOMENTS
  if (mesure & EXCENTRICITE)
  {
    FRectangle(0, ImageHeight + 6*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 7*BoxHeight - 2, ButtonStrings[6][0]);
    ColToBlack();
  }
#endif

  FlushGraphics();  

  WaitMouseEvent(&xx, &yy);

  if ((xx < ImageWidth) && (yy < ImageHeight))
  {
#ifdef MOMENTS
    signature(mesure, xx, yy, M, CBT, rs, SURF, PROF, VOL, PER, RDIST, TROU, EXCEN);
    printcomp(xx, yy, M, CBT, rs, SURF, PROF, VOL, PER, RDIST, TROU, EXCEN);
#else
    signature(mesure, xx, yy, M, CBT, rs, SURF, PROF, VOL, PER, RDIST, TROU, NULL);
    printcomp(xx, yy, M, CBT, rs, SURF, PROF, VOL, PER, RDIST, TROU, NULL);
#endif
  }
  xx /= BoxWidth;
  yy -= ImageHeight;
  yy /= BoxHeight;

  switch(yy)
  {
    case 0: switch(xx)
            {
              case 0: if ((mesure & SURFACE) && (mesure != SURFACE)) 
                        mesure &= ~SURFACE;
                      else if (!(mesure & SURFACE))
                        mesure |= SURFACE;
                      break;
              case 1: break;
              case 2: seuilsurf = seuilsurf + increment;
                      sprintf(ButtonStrings[0][1], "%d", seuilsurf);
                      break;
              case 3: seuilsurf = seuilsurf - increment;
                      if (seuilsurf < 1) seuilsurf = 1;
                      sprintf(ButtonStrings[0][1], "%d", seuilsurf);
                      break;
              case 4: increment = 1;
                      break;
              case 5: increment = 10;
                      break;
              case 6: maximise = !maximise;
                      if (maximise)
                        sprintf(ButtonStrings[0][6], " MAX");
                      else
                        sprintf(ButtonStrings[0][6], " MIN");
                      break;
            } /* switch(xx) */
            break;
    case 1: switch(xx)
            {
              case 0: if ((mesure & PROFONDEUR) && (mesure != PROFONDEUR)) 
                        mesure &= ~PROFONDEUR;
                      else if (!(mesure & PROFONDEUR))
                        mesure |= PROFONDEUR;
                      break;
              case 1: break;
              case 2: seuilprof = seuilprof + increment;
                      sprintf(ButtonStrings[1][1], "%d", seuilprof);
                      break;
              case 3: seuilprof = seuilprof - increment;
                      if (seuilprof < 0) seuilprof = 0;
                      sprintf(ButtonStrings[1][1], "%d", seuilprof);
                      break;
              case 4: increment = 100;
                      break;
              case 5: increment = 1000;
                      break;
              case 6: conjonction = !conjonction;
                      if (conjonction)
                        sprintf(ButtonStrings[1][6], "  ET");
                      else
                        sprintf(ButtonStrings[1][6], "  OU");
                      break;
            } /* switch(xx) */
            break;
    case 2: switch(xx)
            {
#ifdef VOL2
              case 0: if ((mesure & VOLUME) && (mesure != VOLUME)) 
                        mesure &= ~VOLUME;
                      else if (!(mesure & VOLUME))
                        mesure |= VOLUME;
                      break;
              case 1: break;
              case 2: seuilvol = seuilvol + increment;
                      sprintf(ButtonStrings[2][1], "%d", seuilvol);
                      break;
              case 3: seuilvol = seuilvol - increment;
                      if (seuilvol < 0) seuilvol = 0;
                      sprintf(ButtonStrings[2][1], "%d", seuilvol);
                      break;
#endif
#ifdef PER2
              case 0: if ((mesure & PERIMETRE) && (mesure != PERIMETRE)) 
                        mesure &= ~PERIMETRE;
                      else if (!(mesure & PERIMETRE))
                        mesure |= PERIMETRE;
                      break;
              case 1: break;
              case 2: seuilperim = seuilperim + increment;
                      sprintf(ButtonStrings[2][1], "%d", seuilperim);
                      break;
              case 3: seuilperim = seuilperim - increment;
                      if (seuilperim < 0) seuilperim = 0;
                      sprintf(ButtonStrings[2][1], "%d", seuilperim);
                      break;
#endif
#ifdef TROU2
              case 0: if ((mesure & NBTROUS) && (mesure != NBTROUS)) 
                        mesure &= ~NBTROUS;
                      else if (!(mesure & NBTROUS))
                        mesure |= NBTROUS;
                      break;
              case 1: break;
              case 2: seuiltrou = seuiltrou + increment;
                      sprintf(ButtonStrings[2][1], "%d", seuiltrou);
                      break;
              case 3: seuiltrou = seuiltrou - increment;
                      if (seuiltrou < 0) seuiltrou = 0;
                      sprintf(ButtonStrings[2][1], "%d", seuiltrou);
                      break;
#endif
              case 4: increment = 10000;
                      break;
              case 5: goto ReSegment;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
    case 3: switch(xx)
            {
              case 0: if ((mesure & ROOTDIST) && (mesure != ROOTDIST)) 
                        mesure &= ~ROOTDIST;
                      else if (!(mesure & ROOTDIST))
                        mesure |= ROOTDIST;
                      break;
              case 1: break;
              case 2: seuilrdist = seuilrdist + increment;
                      if (seuilrdist > NDG_MAX) seuilrdist = NDG_MAX;
                      sprintf(ButtonStrings[3][1], "%d", seuilrdist);
                      break;
              case 3: seuilrdist = seuilrdist - increment;
                      if (seuilrdist < NDG_MIN) seuilrdist = NDG_MIN;
                      sprintf(ButtonStrings[3][1], "%d", seuilrdist);
                      break;
              case 4: 
              case 5: 
              case 6: break;
            } /* switch(xx) */
            break;
    case 4: switch(xx)
            {
              case 0: if ((mesure & CIRCULARITE) && (mesure != CIRCULARITE)) 
                        mesure &= ~CIRCULARITE;
                      else if (!(mesure & CIRCULARITE))
                        mesure |= CIRCULARITE;
                      break;
              case 1: break;
              case 2: seuilcirc = seuilcirc + increment;
                      if (seuilcirc > 100) seuilcirc = 100;
                      sprintf(ButtonStrings[4][1], "%d", seuilcirc);
                      break;
              case 3: seuilcirc = seuilcirc - increment;
                      if (seuilcirc < 0) seuilcirc = 0;
                      sprintf(ButtonStrings[4][1], "%d", seuilcirc);
                      break;
              case 4: 
              case 5: 
              case 6: break;
            } /* switch(xx) */
            break;
    case 5: switch(xx)
            {
              case 0: if ((mesure & DENSTROUS) && (mesure != DENSTROUS)) 
                        mesure &= ~DENSTROUS;
                      else if (!(mesure & DENSTROUS))
                        mesure |= DENSTROUS;
                      break;
              case 1: break;
              case 2: seuildtrou = seuildtrou + increment;
                      if (seuildtrou > 1000) seuildtrou = 1000;
                      sprintf(ButtonStrings[5][1], "%d", seuildtrou);
                      break;
              case 3: seuildtrou = seuildtrou - increment;
                      if (seuildtrou < 0) seuildtrou = 0;
                      sprintf(ButtonStrings[5][1], "%d", seuildtrou);
                      break;
              case 4: 
              case 5: 
              case 6: break;
            } /* switch(xx) */
            break;
#ifdef MOMENTS
    case 6: switch(xx)
            {
              case 0: if ((mesure & EXCENTRICITE) && (mesure != EXCENTRICITE)) 
                        mesure &= ~EXCENTRICITE;
                      else if (!(mesure & EXCENTRICITE))
                        mesure |= EXCENTRICITE;
                      break;
              case 1: break;
              case 2: seuilexcen = seuilexcen + increment;
                      if (seuilexcen > 255) seuilexcen = 255;
                      sprintf(ButtonStrings[6][1], "%d", seuilexcen);
                      break;
              case 3: seuilexcen = seuilexcen - increment;
                      if (seuilexcen < 0) seuilexcen = 0;
                      sprintf(ButtonStrings[6][1], "%d", seuilexcen);
                      break;
              case 4: 
              case 5: 
              case 6: break;
            } /* switch(xx) */
            break;
#endif
  } /* switch(yy) */
  goto ReDisplay;

Terminate:
  TerminateGraphics();

  for (x = 0; x < N; x++) F[x] = TEMP[x];

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  for (i = 0; i < NBLIGBUTT; i++) 
    for (k = 0; k < NBBUTTONS; k++) 
      free(ButtonStrings[i][k]);
  free(TEMP);
  free(I);
  free(SURF);
  free(PROF);
  free(DERPROF);
  free(VOL);
  free(PER);
  free(RDIST);
  free(TROU);
#ifdef MOMENTS
  free(EXCEN);
#endif
  IndicsTermine();
  FahTermine(FAH);
  LifoTermine(LIFO);
  free(CBT);
  free(M);
  return(1);
}
