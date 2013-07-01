/* $Id: lsegment3di.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur interactif de segmentation par seuillage pour image 3d numerique */
/* utilise une File d'Attente Hierarchique */
/* utilise un arbre des bassins versants (captation basin tree, CBT) */
/* Michel Couprie - juillet 1997 */
/* version 0.2 du 05/8/97 */
/* 
   5/07/97 : affichage en niveaux de gris
   25/08/97 : nettoyage et unification 
   1/2/98 : retire   UnSetAll(x);   apres        ncc = 0; x = FahPop(FAH);
            remplace
               n = FirstSon(CBT, y + 1) - FirstSon(CBT, y);
            par
               if (y < nbcell - 1)
                 n = FirstSon(CBT, y + 1) - FirstSon(CBT, y);
               else
                 n = nbcell - 2 - FirstSon(CBT, y);
   4/11/99 : nouvelle construction de l'arbre : travaille au niveau des plateaux et
             non plus des pixels

   20/4/00 : corrige bug 
*/

#define ZOOM4

#define PARANO                 /* even paranoid people have ennemies */
#define VERBOSE

/*
#define MAXIMISE
*/

/*
#define DEBUG
#define TRACEIMAGE
*/
#define TRACEPOP
#define TRACEPUSH
#define TRACEBAISSE
#define TRACEETIQ
#define SEGMENT
#define TRACECBT

/*
#define NOSURFVOL
*/

/*
   Methode : 

===============================================
1ere etape : CONSTRUCTION DU CBT
===============================================
  entree : une image F
  sortie : le CBT, l'application M: Dom(F) -> CBT

  M <- etiquettes des minima de F
  nminima <- nombre de minima de F
  init(CBT, nminima)
  pour tout point x de Dom(F)
    si x appartient a un minimum
      SetData(CBT, M[x], SOURCE[x] + 1);
      pour tout y dans gamma(x)
        si y n'appartient pas a un minimum et pas deja dans FAH
          FahPush(FAH, y, SOURCE[y]);
        finsi
      finpour
    finsi
  finpour

  tant que FAH non vide
    x = FahPop(FAH);
    etiqcc = liste des M[y], y dans gamma(x)
    diffanc = liste des ancetres differents des etiquettes de etiqcc dans le CBT
    si |diffanc| == 1
      M[x] = first(diffanc)
    sinon
      si il existe une ou des cellules de niveau F[x]+1 dans gamma(x)
        new = fusion de ces cellules
      sinon
        new = CreateCell(CBT)
      M[x] = new
      SetData(CBT, new, F[x] + 1)
      pour tout a dans diffanc 
        SetFather(CBT, a, new)
      finpour
    fin si
    pour tout y dans gamma(x) pas deja dans FAH
      si M[y] == 0
        FahPush(FAH, y, SOURCE[y]);
      finsi
    finpour
  fintantque

===============================================
2eme etape: 
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

  2EME ETAPE : MAXIMISE AU SENS DE L'INCLUSION

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
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfah.h>
#include <mclifo.h>
#include <mcindic.h>
#include <mccbt.h>
#include <mcxbib.h>
#include <llabelextrema.h>
#include <lsegment3di.h>

#define EN_FAH 0
#define MINIMUM 1
#define CC_EN_FAH 2

#define NONMARQUE 0
#define INVALIDE 1
#define PERTINENT 2

#define MARQUE1    2
#define MARQUE2    3

/* ==================================== */
static int32_t NotIn(
  int32_t e,
  int32_t *list,                   
  int32_t n)                       
/* ==================================== */
{
/* renvoie 1 si e n'est pas dans list, 0 sinon */
/* e : l'element a rechercher */
/* list : la liste (tableau d'entiers) */
/* n : le nombre d'elements dans la liste */
  while (n > 0)
    if (list[--n] == e) return 0;
  return 1;
} /* NotIn() */

/* ==================================== */
int32_t lsegment3di(struct xvimage *image, int32_t connex)
/* ==================================== */
{
  char buf[128];
  register int32_t i;                       /* index muet */
  register int32_t x;                       /* index muet de pixel */
  register int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t w;                       /* index muet (generalement un voisin de x) */
  register int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t d = depth(image);        /* nb plans */
  int32_t n = rs * cs;             /* taille plan */
  int32_t N = n * d;               /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *temp;        /* image de travail et resultat */
  uint8_t *TEMP;
  struct xvimage *lab;
  uint32_t *M;            /* l'image d'etiquettes de composantes connexes */
  int32_t *T;                      /* table de correspondance pour regularisation */
  int32_t *I;                      /* pour l'arbre inverse */
  int32_t nminima;                 /* nombre de minima differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  cbtcell * CBT;               /* arbre des bassins versants (Catchment Basin Tree) */
  int32_t nbcell;
  int32_t nbmaxcell;
  Lifo * LIFO;                 /* pour parcourir un plateau */
  int32_t *etiqcc;                 /* etiquettes au voisinage d'un plateau */
  int32_t nccmax;                  /* nombre max. d'etiquettes pouvant voisiner un plateau */
  int32_t h;
  int32_t tailleplateau;
  int32_t ncc;
  int32_t new;
  int32_t prof;
  int32_t *PROF;                   /* pour le stockage de la profondeur */
#ifndef NOSURFVOL
  int32_t *SURF;                   /* pour le stockage de la surface */
  int32_t *DERPROF;                /* pour le stockage de la derniere profondeur */
  int32_t *VOL;                    /* pour le stockage du volume */
#endif
  int32_t seuilsurf = 1;
  int32_t seuilprof = 0;
  int32_t seuilvol = 0;
  int32_t mesure = PROFONDEUR;
  int32_t increment = 1;
  int32_t maximise = 0;
  int32_t iplan = 0;
#define NBBUTTONS 7
#define NBLIGBUTT 4
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
  int32_t nsave = 0;

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

#ifndef NOSURFVOL
  strcpy(ButtonStrings[0][0], " SURF");
#else
  strcpy(ButtonStrings[0][0], "     ");
#endif
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
  strcpy(ButtonStrings[1][6], "     ");

#ifndef NOSURFVOL
  strcpy(ButtonStrings[2][0], " VOL ");
#else
  strcpy(ButtonStrings[2][0], "     ");
#endif
  sprintf(ButtonStrings[2][1], "%d", seuilvol);
  strcpy(ButtonStrings[2][2], "  +  ");
  strcpy(ButtonStrings[2][3], "  -  ");
  strcpy(ButtonStrings[2][4], " 10K ");
  strcpy(ButtonStrings[2][5], " 100K");
  strcpy(ButtonStrings[2][6], " SAVE");

  strcpy(ButtonStrings[3][0], " PLAN");
  sprintf(ButtonStrings[3][1], "%d", iplan);
  strcpy(ButtonStrings[3][2], "  +  ");
  strcpy(ButtonStrings[3][3], "  -  ");
  strcpy(ButtonStrings[3][4], "1000K");
  strcpy(ButtonStrings[3][5], " OK  ");
  strcpy(ButtonStrings[3][6], " QUIT");

  IndicsInit(N);
  FAH = CreeFahVide(N+1);
  if (FAH == NULL)
  {   fprintf(stderr, "lsegment3di() : CreeFah failed\n");
      return(0);
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lsegment3di() : CreeLifoVide failed\n");
      return(0);
  }

  lab = allocimage(NULL, rs, cs, d, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "lsegment3di: allocimage failed\n");
    return 0;
  }
  M = ULONGDATA(lab);

  if (!llabelextrema(image, connex, LABMIN, lab, &nminima))
  {   
    fprintf(stderr, "lsegment3di: llabelextrema failed\n");
    return 0;
  }
  nminima -= 1;  /* le label 0 ne represente pas un minimum */

  nbcell = nminima+1;  /* +1 : pour le decalage entre no minima et no neuds - 20/4/00 */
  nbmaxcell = nbcell * 2;  /* 21/4/00: nbcell au lieu de nminima */
  CBT = (cbtcell *)calloc(1,nbmaxcell * sizeof(cbtcell));
  if (CBT == NULL)
  {   fprintf(stderr, "lsegment3di() : malloc failed for CBT\n");
      return(0);
  }

#ifdef VERBOSE
  fprintf(stderr, "nminima = %d\n", nminima);
#endif

  PROF = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  if (PROF == NULL)
  {   fprintf(stderr, "lsegment3di() : calloc failed\n");
      return(0);
  }
#ifndef NOSURFVOL
  SURF = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  if (SURF == NULL)
  {   fprintf(stderr, "lsegment3di() : calloc failed\n");
      return(0);
  }
  DERPROF = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  VOL = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  if ((DERPROF == NULL) || (VOL == NULL))
  {   fprintf(stderr, "lsegment3di() : calloc failed\n");
      return(0);
  }
#endif

  /* calcul des tailles des minima (histogramme des labels) */
#ifndef NOSURFVOL
  for (x = 0; x < N; x++) SURF[M[x]] += 1;
  for (x = 0; x < N; x++) DERPROF[M[x]] = F[x] + 1;
#endif

  nccmax = 2*N / 3;  /* majore la longueur de la frontiere exterieure d'un plateau */
  etiqcc = (int32_t *)calloc(1,nccmax * sizeof(int32_t));
  if (etiqcc == NULL)
  {   fprintf(stderr, "lsegment() : malloc failed for etiqcc\n");
      return(0);
  }

  /* ================================================ */
  /*                CONSTRUCTION DU CBT               */
  /* ================================================ */

  InitCbt(CBT, nbcell);
  
  FahPush(FAH, -1, 0);               /* force la creation du niveau 0 dans la Fah. */
                                     /* NECESSAIRE pour eviter la creation prematuree */
                                     /* de la file d'urgence */ 
  for (x = 0; x < N; x++)
  {
    if (M[x] != 0)                   /* le pixel appartient a un minimum */
    {
      Set(x, MINIMUM);
      SetData(CBT, M[x], F[x]);     /* conceptuellement : F[x] + 1 */

      switch (connex)
      {
      case 6:
        for (k = 0; k <= 10; k += 2)       /* parcourt les 6 voisins */
        {                                  /* si un voisin n'est pas deja dans la FAH */
          y = voisin6(x, k, rs, n, N);     /* et n'est pas dans un minimum, on le met en FAH */
          if ((y != -1) && (! IsSet(y, EN_FAH)) && (M[y] == 0))
	  {
            FahPush(FAH, y, F[y]);
            Set(y, EN_FAH);
          } /* if y */
        } /* for k */
        break;

      case 18:
        for (k = 0; k < 18; k++)           /* parcourt les 18 voisins */
        {                                  /* si un voisin n'est pas deja dans la FAH */
          y = voisin18(x, k, rs, n, N);    /* et n'est pas dans un minimum, on le met en FAH */
          if ((y != -1) && (! IsSet(y, EN_FAH)) && (M[y] == 0))
	  {
            FahPush(FAH, y, F[y]);
            Set(y, EN_FAH);
          } /* if y */
        } /* for k */
        break;

      case 26:
        for (k = 0; k < 26; k++)           /* parcourt les 26 voisins */
        {                                  /* si un voisin n'est pas deja dans la FAH */
          y = voisin26(x, k, rs, n, N);    /* et n'est pas dans un minimum, on le met en FAH */
          if ((y != -1) && (! IsSet(y, EN_FAH)) && (M[y] == 0))
	  {
            FahPush(FAH, y, F[y]);
            Set(y, EN_FAH);
#ifdef DEBUG
  printf("Init: empile y = (%d,%d,%d) voisin de x = (%d,%d,%d)\n", 
                y%rs, (y%n)/rs, y/n, x%rs, (x%n)/rs, x/n);
#endif
          } /* if y */
        } /* for k */
        break;

      } /* switch (connex) */
    } /* if (M[x] != 0) */
  } /* for x */

  x = FahPop(FAH);
#ifdef PARANO
  if (x != -1)
  {   
     printf("segment3di() : ORDRE FIFO NON RESPECTE PAR LA FAH !!!\n");
     return(0);
  }
#endif

  /* ================================================================= */
  /* INONDATION */
  /* ================================================================= */

  while (! FahVide(FAH))
  {
    x = FahPop(FAH);
#ifdef DEBUG
  printf("FAH POP x = (%d,%d,%d)\n", 
                   x%rs, (x%n)/rs, x/n);
#endif
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
#ifdef DEBUG
  printf("Lifo Pop w = (%d,%d,%d), plateau de x = (%d,%d,%d)\n", 
                   w%rs, (w%n)/rs, w/n, x%rs, (x%n)/rs, x/n);
#endif
        tailleplateau++;
        switch (connex)
        {
          case 6:
            for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
            {
              y = voisin6(w, k, rs, n, N);
              if (y != -1)
              {
                if  ((M[y] != 0) && (!IsSet(y, MARQUE2)))
                {
                  if (ncc >= nccmax)
                  {   
                    printf("segment() : ncc = %d ; nccmax = %d\n", ncc, nccmax);
                    return(0);
                  }
                  if (NotIn(M[y], etiqcc, ncc))
		  {
                    etiqcc[ncc] = M[y];
                    ncc += 1;
		  }
                  Set(y, MARQUE2);
                }
                if ((F[y] == h) && (!IsSet(y, MARQUE1)))
                {
                  Set(y, MARQUE1);
                  LifoPush(LIFO, y);
                }
              } /* if (y != -1) */
            } /* for (k = 0; k < 8; k += incr_vois) */
            break;
          case 18:
            for (k = 0; k < 18; k++) /* parcourt les 18 voisins */
            {
              y = voisin18(w, k, rs, n, N);
              if (y != -1)
              {
                if  ((M[y] != 0) && (!IsSet(y, MARQUE2)))
                {
                  if (ncc >= nccmax)
                  {   
                    printf("segment() : ncc = %d ; nccmax = %d\n", ncc, nccmax);
                    return(0);
                  }
                  if (NotIn(M[y], etiqcc, ncc))
		  {
                    etiqcc[ncc] = M[y];
                    ncc += 1;
		  }
                  Set(y, MARQUE2);
                }
                if ((F[y] == h) && (!IsSet(y, MARQUE1)))
                {
                  Set(y, MARQUE1);
                  LifoPush(LIFO, y);
                }
              } /* if (y != -1) */
            } /* for (k = 0; k < 8; k += incr_vois) */
            break;
          case 26:
            for (k = 0; k < 26; k++) /* parcourt les 26 voisins */
            {
              y = voisin26(w, k, rs, n, N);
              if (y != -1)
              {
                if  ((M[y] != 0) && (!IsSet(y, MARQUE2)))
                {
                  if (ncc >= nccmax)
                  {   
                    printf("segment() : ncc = %d ; nccmax = %d\n", ncc, nccmax);
                    return(0);
                  }
                  if (NotIn(M[y], etiqcc, ncc))
		  {
                    etiqcc[ncc] = M[y];
                    ncc += 1;
		  }
                  Set(y, MARQUE2);
                }
                if ((F[y] == h) && (!IsSet(y, MARQUE1)))
                {
                  Set(y, MARQUE1);
                  LifoPush(LIFO, y);
                }
              } /* if (y != -1) */
            } /* for (k = 0; k < 8; k += incr_vois) */
            break;
	} /* switch (connex) */
      } /* while (! LifoVide(LIFO)) */
      ncc = DiffAnc(CBT, ncc, etiqcc);

#ifdef PARANO
      if (ncc == 0)
      {
        fprintf(stderr, "segment3di() : construction arbre : PAS DE cc DANS LE VOISINAGE !!!\n"); 
        fprintf(stderr, "x = (%d,%d,%d)\n", x%rs, (x%n)/rs, x/n);
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
          switch (connex)
          {
            case 6:
              for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
              {
                y = voisin6(w, k, rs, n, N);
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
              } /* for (k = 0; ... */
              break;
            case 18:
              for (k = 0; k < 18; k++) /* parcourt les 18 voisins */
              {
                y = voisin18(w, k, rs, n, N);
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
              } /* for (k = 0; ... */
              break;
            case 26:
              for (k = 0; k < 26; k++) /* parcourt les 26 voisins */
              {
                y = voisin26(w, k, rs, n, N);
                if (y != -1)
                {
                  UnSet(y, MARQUE2);
                  if ((! IsSet(y, EN_FAH)) && (M[y] == 0) && (!IsSet(y, MARQUE1)))
                  {
                    FahPush(FAH, y, F[y]);
                    Set(y, EN_FAH);
#ifdef DEBUG
  printf("empile y = (%d,%d,%d) voisin de w = (%d,%d,%d)\n", 
                   y%rs, (y%n)/rs, y/n, w%rs, (w%n)/rs, w/n);
#endif
                  }
                  if ((F[y] == h) && (IsSet(y, MARQUE1)))
                  {
                    UnSet(y, MARQUE1);
                    LifoPush(LIFO, y);
                  }
                } /* if (y != -1) */
              } /* for (k = 0; ... */
              break;
	  } /* switch (connex) */
        } /* while (! LifoVide(LIFO)) */

        /* mise a jour de la surface, profondeur, volume pour le plateau */
#ifndef NOSURFVOL
        SURF[M[w]] += tailleplateau;
        if (F[w] + 1 > DERPROF[M[w]])
        {
          VOL[M[w]] += SURF[M[w]] * (F[w] + 1 - DERPROF[M[w]]);
          DERPROF[M[w]] = F[w] + 1;
        }
#endif
      } /* if (ncc == 1) */
      else
      {
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
          switch (connex)
          {
            case 6:
              for (k = 0; k <= 10; k += 2) /* parcourt les 6 voisins */
              {
                y = voisin6(w, k, rs, n, N);
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
              } /* for (k = 0; ... */
              break;
            case 18:
              for (k = 0; k < 18; k++) /* parcourt les 18 voisins */
              {
                y = voisin18(w, k, rs, n, N);
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
              } /* for (k = 0; ... */
              break;
            case 26:
              for (k = 0; k < 26; k++) /* parcourt les 26 voisins */
              {
                y = voisin26(w, k, rs, n, N);
                if (y != -1)
                {
                  UnSet(y, MARQUE2);
                  if ((! IsSet(y, EN_FAH)) && (M[y] == 0) && (!IsSet(y, MARQUE1)))
                  {
                    FahPush(FAH, y, F[y]);
                    Set(y, EN_FAH);
#ifdef DEBUG
  printf("newcell: empile y = (%d,%d,%d) voisin de w = (%d,%d,%d)\n", 
                   y%rs, (y%n)/rs, y/n, w%rs, (w%n)/rs, w/n);
#endif
                  }
                  if ((F[y] == h) && (IsSet(y, MARQUE1)))
                  {
                    UnSet(y, MARQUE1);
                    LifoPush(LIFO, y);
                  }
                } /* if (y != -1) */
              } /* for (k = 0; ... */
              break;
	  } /* switch (connex) */
        } /* while (! LifoVide(LIFO)) */

#ifndef NOSURFVOL
        SURF[M[x]] = tailleplateau;
#endif
        prof = 0;
        for (i = 0; i < ncc; i++)
        {
          k = PROF[etiqcc[i]]+F[x]-Data(CBT,etiqcc[i]); /* conceptuellement : F[x] + 1 */
          if (k > prof) prof = k;
#ifndef NOSURFVOL
          SURF[M[x]] += SURF[etiqcc[i]];
          if (F[x] + 1 > DERPROF[etiqcc[i]])
          {
            VOL[etiqcc[i]] += SURF[etiqcc[i]] * (F[x] + 1 - DERPROF[etiqcc[i]]);
            DERPROF[etiqcc[i]] = F[x] + 1;
          }
          VOL[M[x]] += VOL[etiqcc[i]];
#endif
        } /* for i */
        PROF[M[x]] = prof;
#ifndef NOSURFVOL
        DERPROF[M[x]] = F[x] + 1;
#endif
      } /* else if (ncc == 1) */
    } /* if (!IsSet(x, TRAITE)) */
  } /* while (! FahVide(FAH)) */
  /* FIN INONDATION */

#ifdef VERBOSE
  printf("FIN DE LA CONSTRUCTION DE L'ARBRE DES COMPOSANTES\n");
#endif

  free(etiqcc);

  T = Regularise(CBT, nminima, nbcell);

  for (x = 0; x < N; x++)
    if ((M[x] >= nminima) && (T[M[x] - nminima]))
      M[x] = T[M[x] - nminima]; /* nouvelle etiquette pour le point x */

  free(T);

#ifdef PARANO
  for (i = 0; i < nbcell; i++)
    if (Label(CBT,i)) printf("ERROR : label = %d\n", Label(CBT, i));
#endif

#ifdef VERBOSE
  printf("FIN DE LA REGULARISATION DE L'ARBRE DES COMPOSANTES\n");
#endif

  /* ================================================ */
  /* SEGMENTATION                               */
  /* ================================================ */

  temp = allocimage(NULL, rs, cs, d, VFF_TYP_1_BYTE);
  if (temp == NULL)
  {   fprintf(stderr, "lsegmenti() : allocimage failed\n");
      return(0);
  }
  TEMP = UCHARDATA(temp);

  /* pour la mesure de la profondeur, il faut rajouter la difference de niveau avec le pere */
  /* on remplace donc les MU dans l'arbre */
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
  } 

  I = InverseCBT(CBT, nminima, nbcell);

/*  1ERE ETAPE : SELECTIONNE LE NB MAX DE CC PERTINENTES */

ReSegment:
  /* init:  ranger au niveau 0 de la FAH les feuilles du CBT */
  FahFlush(FAH);
  for (i = 1; i < nminima+1; i++) FahPush(FAH, i, 0);

  while (! FahVide(FAH))
  {
    k = FahNiveau(FAH);
    x = FahPop(FAH);

    if (Label(CBT,x) == NONMARQUE)
    {
      if (((PROF[x] >= seuilprof) || (!(mesure & PROFONDEUR))) 
#ifndef NOSURFVOL
          && ((SURF[x] >= seuilsurf) || (!(mesure & SURFACE)))
          && ((VOL[x] >= seuilvol) || (!(mesure & VOLUME)))
#endif
         )
      {
        Label(CBT,x) = PERTINENT;
        y = Father(CBT,x);
        while ((y != NIL) && (Label(CBT,y) != INVALIDE))
        {
          Label(CBT,y) = INVALIDE;
          y = Father(CBT,y);
        } /* while ((y != NIL) && (Label(CBT,y) != INVALIDE)) */
      } /* if (MU[x] >= seuil) */
      else
      {
        y = Father(CBT,x);
        if ((y != NIL) && (Label(CBT,y) != INVALIDE))
          FahPush(FAH, y, k + 1);
      } /* else if (MU[x] >= seuil) */
    } /* if (Label(CBT,x) != INVALIDE) */
  } /* while (! FahVide(FAH)) */

/*  2EME ETAPE : MAXIMISE AU SENS DE L'INCLUSION */

  if (maximise)
  {
    FahFlush(FAH);
    for (x = 1; x < nbcell - 1; x++)   /* pas la racine */
      if (Label(CBT, x) == PERTINENT)
        FahPush(FAH, x, Data(CBT, x));

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
      if (nbfilspert == 1)
      {
        Label(CBT, x) = NONMARQUE;
        Label(CBT, y) = PERTINENT;
        FahPush(FAH, y, Data(CBT, y));
      } /* if (nbfilspert == 1) */
    } /* while (! FahVide(FAH)) */
  } /* if (maximise) */

/* RECUPERATION DE LA SEGMENTATION */

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

  FahFlush(FAH);
  for (i = 0; i < nbcell; i++) Label(CBT,i) = 0;

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
    | PROF | 111  |  +   |  -   | 100  | 1000 |      |        BoxHeight
    --------------------------------------------------
    | VOL  | 111  |  +   |  -   | 10K  | 100K | SAVE |        BoxHeight
    --------------------------------------------------
    | PLAN | 111  |  +   |  -   | 1000K|  OK  | QUIT |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

ReDisplayImage:

  /* send output to the screen */
#ifdef COLOR
  for (x = iplan * n; x < (iplan+1)*n; x++)
  {
    if (TEMP[x])    
      Color(MC_RED);
    else
      Color(F[x]/4);
#ifdef ZOOM4
    Point(2*((x%n)%rs), 2*((x%n)/rs));
    Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
    Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
    Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
#else
    Point((x%n)%rs, (x%n)/rs);
#endif
  }
#else
  ColToWhite();
  FRectangle(0, 0, ImageWidth, ImageHeight);
  ColToBlack();
  for (x = iplan * n; x < (iplan+1)*n; x++)
    if (TEMP[x])
    {   
#ifdef ZOOM4
      Point(2*((x%n)%rs), 2*((x%n)/rs));
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs));
      Point(2*((x%n)%rs), 2*((x%n)/rs)+1);
      Point(2*((x%n)%rs)+1, 2*((x%n)/rs)+1);
#else
      Point((x%n)%rs, (x%n)/rs);
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
case 100000: FRectangle(5 * BoxWidth, ImageHeight + 2*BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, ImageHeight + 3*BoxHeight - 2, ButtonStrings[2][5]);
            ColToBlack();
            break;
case 1000000: FRectangle(4 * BoxWidth, ImageHeight + 3*BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, ImageHeight + 4*BoxHeight - 2, ButtonStrings[3][4]);
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
  if (mesure & VOLUME)
  {
    FRectangle(0, ImageHeight + 2*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, ImageHeight + 3*BoxHeight - 2, ButtonStrings[2][0]);
    ColToBlack();
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
              case 0: 
#ifndef NOSURFVOL
                      if ((mesure & SURFACE) && (mesure != SURFACE)) 
                        mesure &= ~SURFACE;
                      else if (!(mesure & SURFACE))
                        mesure |= SURFACE;
#endif
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
              case 6: break;
            } /* switch(xx) */
            break;
    case 2: switch(xx)
            {
              case 0: 
#ifndef NOSURFVOL
                      if ((mesure & VOLUME) && (mesure != VOLUME)) 
                        mesure &= ~VOLUME;
                      else if (!(mesure & VOLUME))
                        mesure |= VOLUME;
#endif
                      break;
              case 1: break;
              case 2: seuilvol = seuilvol + increment;
                      sprintf(ButtonStrings[2][1], "%d", seuilvol);
                      break;
              case 3: seuilvol = seuilvol - increment;
                      if (seuilvol < 0) seuilvol = 0;
                      sprintf(ButtonStrings[2][1], "%d", seuilvol);
                      break;
              case 4: increment = 10000;
                      break;
              case 5: increment = 100000;
                      break;
              case 6: 
                      sprintf(buf, "tmp_segment3di_%d", nsave);
                      writeimage(temp, buf);
                      nsave++;
                      break;
            } /* switch(xx) */
            break;
    case 3: switch(xx)
            {
              case 0: break;
              case 1: break;
              case 2: iplan++;
                      if (iplan > d-1) iplan = d-1;
                      sprintf(ButtonStrings[3][1], "%d", iplan);
                      goto ReDisplayImage;
              case 3: iplan--;
                      if (iplan < 0) iplan = 0;
                      sprintf(ButtonStrings[3][1], "%d", iplan);
                      goto ReDisplayImage;
              case 4: increment = 1000000;
                      break;
              case 5: ColToWhite();
                      FRectangle(10, 0, 160, 30);
                      ColToBlack();
                      String(20, 20, "processing..."); 
                      FlushGraphics();  
                      goto ReSegment;
              case 6: goto Terminate;
            } /* switch(xx) */
            break;
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
  freeimage(temp);
  free(I);
  free(PROF);
#ifndef NOSURFVOL
  free(SURF);
  free(DERPROF);
  free(VOL);
#endif
  IndicsTermine();
  FahTermine(FAH);
  LifoTermine(LIFO);
  free(CBT);
  freeimage(lab);
  return(1);
}
