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
/* operateur interactif de segmentation numerique par seuillage */
/* utilise une File d'Attente Hierarchique */
/* utilise un arbre des bassins versants (captation basin tree, CBT) */
/* Michel Couprie - juin 1997 */
/* version 1.1 du 05/8/97 */
/*
  5/08/97 : affichage en niveaux de gris
  25/08/97 : nettoyage et unification
*/
/*
#define TRACECBT
*/

/*
Principe: l'idee est d'appliquer l'operateur de segmentation binaire
  sur un "gradient" de l'image originale. 

Pour eviter tous les problemes lies aux gradients discrets habituels, on considerera
  le gradient comme non nul seulement aux frontieres de pixels de l'image originale
  (ou aux points de la maille derivee, cf. []) :

  image O:                                         image G:

  ------------------------------------             ------------------------------------
  |      |      |      |      |      |             |      |      |      |      |      |
  |  1   |  1   |  12  |  8   |  16  |             |  0   0  0  11  0   4  0   8  0   |
  |      |      |      |      |      |             |      |      |      |      |      |
  ------------------------------------             ---1------0------0------1------12---
  |      |      |      |      |      |             |      |      |      |      |      |
  |  0   |  1   |  12  |  7   |  4   |             |  0   1  0  11  0   5  0   3  0   |
  |      |      |      |      |      |             |      |      |      |      |      |
  ------------------------------------             ---0------3------8------3------3----
  |      |      |      |      |      |             |      |      |      |      |      |
  |  0   |  4   |  4   |  4   |  7   |             |  0   4  0   0  0   0  0   3  0   |
  |      |      |      |      |      |             |      |      |      |      |      |
  ------------------------------------             ------------------------------------

Les minima du gradient sont lies aux plateaux de l'image originale. 

Contrairement a la situation de la segmentation binaire, les plateaux sont "en contact"
  les uns avec les autres, separes seulement par la ligne de gradient d'epaisseur nulle.

Il faut etiqueter les plateaux de O et construire le CBT du gradient:

  image E:                                     CBT:

  ------------------------------------    1   2   3   4   5   6   7   8   9        niveau
  |      |      |      |      |      |    10              10                          1
  |  1   |  1   |  2   |  3   |  4   |            11          11                      1
  |      |      |      |      |      |    12                  12  12  12  12          3
  ------------------------------------        13              13                      5       
  |      |      |      |      |      |        14      14                              8
  |  5   |  1   |  2   |  6   |  7   |
  |      |      |      |      |      |
  ------------------------------------
  |      |      |      |      |      |
  |  5   |  8   |  8   |  8   |  9   |
  |      |      |      |      |      |
  ------------------------------------

On selectionnera ensuite les "coupes consistantes" (representant les plateaux devant
  "fusionner") par la methode desormais classique.

Il ne restera plus qu'a decider du sort des plateaux fusionnant: quelle valeur leur
  donner ? idees de variantes: le min, le max, la valeur du plateau le plus etendu,
  la moyenne ponderee par la surface.
*/

#define PARANO                 /* even paranoid people have ennemies */
#define VERBOSE

/*
   Methode : 

===============================================
1ere etape : CONSTRUCTION DU CBT DU GRADIENT
===============================================
  entree : une image F, N = |Dom(F)|
  sortie : le CBT, l'application M: Dom(F) -> CBT

  M <- etiquettes des minima du gradient de F (ie - des plateaux de F)
  nminima <- nombre de minima du gradient de F
  init(CBT, nminima)

  pour tout point x de Dom(F)
    y = VoisinEst(x)
    si y existe ET M[x] != M[y] alors
      g = abs(F[y] - F[x])
      FahPush(FAH, x, g)
    finsi
    y = VoisinSud(x)
    si y existe ET M[x] != M[y] alors
      g = abs(F[y] - F[x])
      FahPush(FAH, x + N, g)   ;; il faudra retirer N pour recuperer la coord. du pixel 
    finsi
  finpour


  tant que FAH non vide
    g = FahNiveau(FAH)
    x = FahPop(FAH)
    etiqcc[0] = M[x]
    si x < N et VoisinEst(x) existe alors etiqcc[1] = M[VoisinEst(x)] finsi
    si x >= N et VoisinSud(x) existe alors etiqcc[1] = M[VoisinSud(x)] finsi

    diffanc = liste des ancetres differents des etiquettes de etiqcc dans le CBT
    si |diffanc| > 1
      new = CreateCell(CBT)
      SetData(CBT, new, g)
      pour tout a dans diffanc 
        SetFather(CBT, a, new)
      finpour
    fin si
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
      marque les ancetres de x "PERTINENT"
      marque x "PERTINENT"
    sinon
      si pere(x) existe et non PERTINENT alors FAHPUSH(pere(x), k+1) finsi
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
#include <mcindic.h>
#include <mccbt.h>
#include <mcxbib.h>
#include <mcutil.h>
#include <llabelplateaux.h>
#include <lsegmentnumi.h>
#include <unistd.h>

#define EN_FAH 0

#define NONMARQUE 0
#define INVALIDE  1
#define PERTINENT 2

/* ==================================== */
int32_t lsegmentnumi(struct xvimage *image, struct xvimage *result)
/* 
   L'image resultat "result" est une image d'entiers longs. 
   Elle servira temporairement a stocker les etiquettes de 4-plateaux. 
*/
/* ==================================== */
{
  register int32_t i;                       /* index muet */
  register int32_t x;                       /* index muet de pixel */
  register int32_t y;                       /* index muet (generalement un voisin de x) */
  register int32_t z;                       /* index muet (generalement un voisin de y) */
  register int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  uint32_t *M = SLONGDATA(result);
  struct xvimage *temp;
  uint32_t *TEMP; 
  int32_t *MU;                     /* pour la mesure des regions */
  int32_t *PROF;                   /* pour la mesure des regions */
  int32_t *T;                      /* table de correspondance pour regularisation */
  int32_t *I;                      /* pour l'arbre inverse */
  int32_t nminima;                 /* nombre de minima differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  cbtcell * CBT;               /* arbre des bassins versants (Catchment Basin Tree) */
  int32_t nbcell;
  int32_t nbmaxcell;
  int32_t etiqcc[4];
  int32_t ncc;
  int32_t new;
  int32_t prof;
  int32_t tmp;
  int32_t seuilsurf = 1;
  int32_t seuilprof = 0;
  int32_t seuilvol = 0;
  int32_t mesure = SURFACE;
  int32_t increment = 1;
  int32_t nbcomp = 0;              /* nombre de composantes selectionnees */
#define NBBUTTONS 7
#define NBLIGBUTT 3
#define BOXWIDTH 70
#define BOXHEIGHT 20
  char * ButtonStrings[NBLIGBUTT][NBBUTTONS];
  int32_t xx, yy;
  int32_t BoxWidth = BOXWIDTH;
  int32_t BoxHeight = BOXHEIGHT;
  int32_t Height = cs + BoxHeight * NBLIGBUTT;
  int32_t Width = rs;
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
  strcpy(ButtonStrings[0][6], " SEG ");

  strcpy(ButtonStrings[1][0], " PROF");
  sprintf(ButtonStrings[1][1], "%d", seuilprof);
  strcpy(ButtonStrings[1][2], "  +  ");
  strcpy(ButtonStrings[1][3], "  -  ");
  strcpy(ButtonStrings[1][4], " 100 ");
  strcpy(ButtonStrings[1][5], " 1000");
  strcpy(ButtonStrings[1][6], "     ");

  strcpy(ButtonStrings[2][0], " VOL ");
  sprintf(ButtonStrings[2][1], "%d", seuilvol);
  strcpy(ButtonStrings[2][2], "  +  ");
  strcpy(ButtonStrings[2][3], "  -  ");
  strcpy(ButtonStrings[2][4], "10000");
  strcpy(ButtonStrings[2][5], " OK  ");
  strcpy(ButtonStrings[2][6], " QUIT");

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lsegmentnumi: cette version ne traite pas les images volumiques\n");
    exit(0);
  }
  
  IndicsInit(N*4);

  FAH = CreeFahVide(2*N+1);
  if (FAH == NULL)
  {   fprintf(stderr, "lsegmentnumi() : CreeFah failed\n");
      return(0);
  }

  temp = allocimage(NULL, rowsize(image), colsize(image), depth(image), VFF_TYP_4_BYTE);
  if (temp == NULL)
  {   
    fprintf(stderr, "lsegmentnumi: allocimage failed\n");
    exit(0);
  }
  TEMP = SLONGDATA(temp);

  if (!llabelplateaux(image, 4, temp, &nminima))
  {   
    fprintf(stderr, "lsegmentnumi: llabelplateaux failed\n");
    return 0;
  }

  nbcell = nminima + 1;
  nbmaxcell = nbcell * 2;
  CBT = (cbtcell *)calloc(1,nbmaxcell * sizeof(cbtcell));
  if (CBT == NULL)
  {   fprintf(stderr, "lsegmentnumi() : malloc failed for CBT\n");
      return(0);
  }

  MU = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  PROF = (int32_t *)calloc(nbmaxcell, sizeof(int32_t));   /* init a 0 */
  if ((MU == NULL) || (PROF == NULL))
  {   fprintf(stderr, "lsegmentnumi() : malloc failed for MU or PROF\n");
      return(0);
  }
  /* calcul des tailles des minima (histogramme des labels) */
  for (x = 0; x < N; x++) MU[TEMP[x]] += 1;

  /* ================================================ */
  /*                CONSTRUCTION DU CBT               */
  /* ================================================ */

  InitCbt(CBT, nbcell);
  
  FahPush(FAH, -1, 0);               /* force la creation du niveau 0 dans la Fah. */
                                     /* NECESSAIRE pour eviter la creation prematuree */
                                     /* de la file d'urgence */ 
  for (x = 0; x < N; x++)
  {
    y = voisin(x, EST, rs, N);
    if ((y != -1) && (TEMP[x] != TEMP[y]))
      FahPush(FAH, x, abs((int32_t)SOURCE[y] - (int32_t)SOURCE[x]));
    y = voisin(x, SUD, rs, N);
    if ((y != -1) && (TEMP[x] != TEMP[y]))
      FahPush(FAH, N + x, abs((int32_t)SOURCE[y] - (int32_t)SOURCE[x]));
  } /* for x */

  x = FahPop(FAH);

  /* INONDATION */
  while (! FahVide(FAH))
  {
    ncc = 0;
    k = FahNiveau(FAH);
    x = FahPop(FAH);

    etiqcc[ncc] = TEMP[x%N];
    ncc += 1;
    
    if (x < N)
    {
      y = voisin(x, EST, rs, N);
      if (y != 1) 
      {
        etiqcc[ncc] = TEMP[y];
        ncc += 1;
      }
    }
    else /* x >= N */
    {
      y = voisin(x - N, SUD, rs, N);
      if (y != 1) 
      {
        etiqcc[ncc] = TEMP[y];
        ncc += 1;
      }
    } /* else if (x < N) */

    ncc = DiffAnc(CBT, ncc, etiqcc);

    if (ncc > 1)
    {
      new = CreateCell(CBT, &nbcell, nbmaxcell);
      SetData(CBT, new, k);
      prof = 0;
      for (i = 0; i < ncc; i++)
      {
        MU[new] += MU[etiqcc[i]];
        tmp = PROF[etiqcc[i]] + k - Data(CBT,etiqcc[i]);
        if (tmp > prof) prof = tmp;
        SetFather(CBT, etiqcc[i], new);
      } /* for i */
      PROF[new] = prof;
    } /* if (ncc > 1) */

  } /* while (! FahVide(FAH)) */
  /* FIN INONDATION */

  T = Regularise(CBT, nminima+1, nbcell);
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
    printf("%3d ", TEMP[i]);
  }
  printf("\n");

  printf("MU   = ");
  for (i = 1; i < nbcell; i++)
    printf("%3d ", MU[i]);
  printf("\n");

  printf("PROF = ");
  for (i = 1; i < nbcell; i++)
    printf("%3d ", PROF[i]);
  printf("\n");
#endif

  /* ================================================ */
  /* SEGMENTATION                               */
  /* ================================================ */

  I = InverseCBT(CBT, nminima+1, nbcell);

  /* pour la mesure de la profondeur, il faut rajouter la difference de niveau avec le pere */
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
      PROF[x] += (Data(CBT,y) - Data(CBT,x));
      if (!IsSet(y, EN_FAH))
      {
        Set(y, EN_FAH);
        FahPush(FAH, y, k + 1); 
      }
    }
    else PROF[x] = NDG_MAX - NDG_MIN; /* prof. maximum pour la racine */
  }

#ifdef TRACECBT
  printf("PROF = ");
  for (i = 1; i < nbcell; i++)
    printf("%3d ", PROF[i]);
  printf("\n");
#endif

ReSegment:

  for (i = 0; i < nbcell; i++) Label(CBT,i) = NONMARQUE;

/*  1ERE ETAPE : SELECTIONNE LE NB MAX DE CC PERTINENTES */

  /* init:  ranger au niveau 0 de la FAH les feuilles du CBT */
  FahFlush(FAH);
  for (i = 1; i < nminima+1; i++) FahPush(FAH, i, 0);
  nbcomp = 0;

  while (! FahVide(FAH))
  {
    k = FahNiveau(FAH);
    x = FahPop(FAH);

    if (Label(CBT,x) == NONMARQUE)
    {
      if (((MU[x] >= seuilsurf)          || !(mesure & SURFACE)) &&
          ((PROF[x] >= seuilprof)        || !(mesure & PROFONDEUR)) &&
          ((MU[x] * PROF[x] >= seuilvol) || !(mesure & VOLUME)))
      {
        Label(CBT,x) = PERTINENT;
        y = Father(CBT,x);
        while ((y != NIL) && (Label(CBT,y) != INVALIDE))
        {
          Label(CBT,y) = INVALIDE;
          y = Father(CBT,y);
        } /* while ((y != NIL) && (Label(CBT,y) != INVALIDE)) */
      } /* if (((MU[x] >= seuilsurf) && ... */
      else
      {
        y = Father(CBT,x);
        if ((y != NIL) && (Label(CBT,y) != INVALIDE))
          FahPush(FAH, y, k + 1);
      } /* else if (((MU[x] >= seuilsurf) && ... */
    } /* if (Label(CBT,x) != INVALIDE) */
  } /* while (! FahVide(FAH)) */

/* RECUPERATION DE LA SEGMENTATION */

  for (x = 1; x < nbcell; x++) Aux(CBT,x) = 0; /* pour la renumerotation */
  nbcomp = 1;                                  /* pour la renumerotation */

  for (i = 0; i < N; i++)
  {
    x = TEMP[i];
    
    while ((x != NIL) && (Label(CBT,x) == NONMARQUE)) 
      x = Father(CBT,x);
    if (Label(CBT,x) == PERTINENT)
    {
      if (Aux(CBT, x) == 0) Aux(CBT, x) = nbcomp++;
      M[i] = Aux(CBT, x);
    }
    else
    {
      M[i] = 0;
    }
  } /* for i */  

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
    | SURF | 111  |  +   |  -   |  1   |  10  | SEG  |        BoxHeight
    --------------------------------------------------
    | PROF | 111  |  +   |  -   | 100  | 1000 |      |        BoxHeight
    --------------------------------------------------
    | VOL  | 111  |  +   |  -   | 10000|  OK  | QUIT |        BoxHeight
    --------------------------------------------------
    <------>
    BoxWidth

*/

  /* send output to the screen */
#ifdef COLOR
  for (x = 0; x < N; x++)
  {
    if (M[x])
      Color((M[x] % 10) + 64);  
    else
      Color(SOURCE[x]/4);
    Point(x%rs, x/rs);
  }
#else
  ColToWhite();
  FRectangle(0, 0, rs, cs);
  ColToBlack();
  for (x = 0; x < N; x++)
    if (M[x])
    {
      Col((M[x] % 10) + 2);  
      Point(x%rs, x/rs);
    }
#endif

  sprintf(ButtonStrings[1][6], "%d", nbcomp-1);

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
   case 10: FRectangle(5 * BoxWidth, cs, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, cs + BoxHeight - 2, ButtonStrings[0][5]);
            ColToBlack();
            break;
  case 100: FRectangle(4 * BoxWidth, cs + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, cs + 2*BoxHeight - 2, ButtonStrings[1][4]);
            ColToBlack();
            break;
 case 1000: FRectangle(5 * BoxWidth, cs + BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(5 * BoxWidth + 3, cs + 2*BoxHeight - 2, ButtonStrings[1][5]);
            ColToBlack();
            break;
case 10000: FRectangle(4 * BoxWidth, cs + 2*BoxHeight, BoxWidth, BoxHeight);
            ColToWhite();
            String(4 * BoxWidth + 3, cs + 3*BoxHeight - 2, ButtonStrings[2][4]);
            ColToBlack();
            break;
  } /* switch (increment) */

  if (mesure & SURFACE)
  {
    FRectangle(0, cs, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, cs + BoxHeight - 2, ButtonStrings[0][0]);
    ColToBlack();
  }
  if (mesure & PROFONDEUR)
  {
    FRectangle(0, cs + BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, cs + 2*BoxHeight - 2, ButtonStrings[1][0]);
    ColToBlack();
  }
  if (mesure & VOLUME)
  {
    FRectangle(0, cs + 2*BoxHeight, BoxWidth, BoxHeight);
    ColToWhite();
    String(0 + 3, cs + 3*BoxHeight - 2, ButtonStrings[2][0]);
    ColToBlack();
  }

  FlushGraphics();  

  WaitMouseEvent(&xx, &yy);

  xx /= BoxWidth;
  yy -= cs;
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
              case 6: 
                      writeimage(image,"_tmplsni1");
                      writeimage(result,"_tmplsni2");
                      if (fork() == 0) /* fils - processus reconsreg */
                      {
                        execlp("reconsreg", "reconsreg", "_tmplsni1", "_tmplsni2", NULL);
                        perror("reconsreg");
                      }
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
            } /* switch(xx) */
            break;
    case 2: switch(xx)
            {
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
              case 4: increment = 10000;
                      break;
              case 5: goto ReSegment;
              case 6: goto Terminate;
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
  freeimage(temp);
  free(I);
  free(CBT);
  free(MU);
  free(PROF);
  FahTermine(FAH);
  IndicsTermine();
  return(1);
}
