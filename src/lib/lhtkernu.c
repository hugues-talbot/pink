/* $Id: lhtkernu.c,v 1.1.1.1 2008-11-25 08:01:39 mcouprie Exp $ */
/* operateur de calcul du noyau de homotopique par elevation */
/* methode: saturation avec politique 4-directionnelle 
   utilise 4 fifo */
/* Michel Couprie -  avril 1997 */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfifo.h>
#include <mctopo.h>
#include <mcindic.h>
#ifdef PERF
#include <mcchrono.h>
#endif
#include <llabelextrema.h>
#include <lhtkernu.h>

#define EN_FIFO       0
#define MAXI          1
#define PARANO
/*
#define DEBUG
*/

/* ==================================== */
static
int32_t testeleve4(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;

#ifdef DEBUG
printf("testeleve : point %d (%d %d), val = %d\n", x, x%rs, x/rs, SOURCE[x]);
#endif

  while (pconstr4(SOURCE,x,rs,N)) 
  { 
    modifie = 1; 
    SOURCE[x] = alpha8p(SOURCE,x,rs,N);  /* alpha8p : sic */
  }

#ifdef DEBUG
if (modifie) printf("========> ELEVE : %d\n", SOURCE[x]);
#endif

  return modifie;
} /* testeleve4() */


/* ==================================== */
static
int32_t testeleve8(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t modifie = 0;

#ifdef DEBUG
printf("testeleve : point %d (%d %d), val = %d\n", x, x%rs, x/rs, SOURCE[x]);
#endif

  while (pconstr8(SOURCE,x,rs,N)) 
  { 
    modifie = 1; 
    SOURCE[x] = alpha8p(SOURCE,x,rs,N); 
  }

#ifdef DEBUG
if (modifie) printf("========> ELEVE : %d\n", SOURCE[x]);
#endif

  return modifie;
} /* testeleve8() */

/* ==================================== */
void testmaxi4(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t k, y;

  for (k = 0; k < 8; k += 2)               /* parcourt les voisins en 4-connexite */
  {                                        /* pour voir s'il existe un maximum */
    y = voisin(x, k, rs, N);               /* au niveau atteint par x */
    if ((y != -1) && (IsSet(y, MAXI)) && (SOURCE[y] == SOURCE[x])) 
    { 
      Set(x, MAXI); 
      break; 
    }
  } /* for k */
} /* testmaxi() */

/* ==================================== */
void testmaxi8(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t k, y;

  for (k = 0; k < 8; k += 1)               /* parcourt les voisins en 8-connexite */
  {                                        /* pour voir s'il existe un maximum */
    y = voisin(x, k, rs, N);               /* au niveau atteint par x */
    if ((y != -1) && (IsSet(y, MAXI)) && (SOURCE[y] == SOURCE[x])) 
    { 
      Set(x, MAXI); 
      break; 
    }
  } /* for k */
} /* testmaxi8() */

/* ==================================== */
static
void empilevoisins(int32_t x, int32_t rs, int32_t N, Fifo *FIFOna, Fifo *FIFOea, Fifo *FIFOsa, Fifo *FIFOoa)
/* ==================================== */
{
  int32_t y;

  y = voisin(x, NORD, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOna, y); Set(y, EN_FIFO); }
  y = voisin(x, NORD+1, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOna, y); Set(y, EN_FIFO); }
  y = voisin(x, EST, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOea, y); Set(y, EN_FIFO); }
  y = voisin(x, EST+1, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOea, y); Set(y, EN_FIFO); }
  y = voisin(x, SUD, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOsa, y); Set(y, EN_FIFO); }
  y = voisin(x, SUD+1, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOsa, y); Set(y, EN_FIFO); }
  y = voisin(x, OUEST, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOoa, y); Set(y, EN_FIFO); }
  y = voisin(x, OUEST+1, rs, N);
  if ((y!=-1) && !IsSet(y,MAXI) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOoa, y); Set(y, EN_FIFO); }
} /* empilevoisins() */

/* ==================================== */
int32_t lhtkernu_lhtkernu(struct xvimage *image, int32_t nitermax, int32_t connex)
/* ==================================== */
{ 
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *SOURCE = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *lab;
  uint32_t *M;            /* l'image d'etiquettes de composantes connexes */
  int32_t nmaxima;                 /* nombre de maxima differents */
  int32_t niter;                   /* nombre d'iterations effectuees (4 directions) */
  Fifo * FIFOn;
  Fifo * FIFOs;
  Fifo * FIFOe;
  Fifo * FIFOo;
  Fifo * FIFOna;
  Fifo * FIFOsa;
  Fifo * FIFOea;
  Fifo * FIFOoa;
  Fifo * FIFOtmp;
#ifdef PERF
  chrono chrono1;
#endif

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lhtkernu: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

#ifdef PERF  
  start_chrono(&chrono1);  /* pour l'analyse de performances */
#endif

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "lhtkernu: allocimage failed\n");
    return 0;
  }
  M = ULONGDATA(lab);

  if (!llabelextrema(image, connex, LABMAX, lab, &nmaxima))
  {   
    fprintf(stderr, "lhtkernu: llabelextrema failed\n");
    return 0;
  }

  IndicsInit(N);

  FIFOn = CreeFifoVide(N/2);
  FIFOs = CreeFifoVide(N/2);
  FIFOe = CreeFifoVide(N/2);
  FIFOo = CreeFifoVide(N/2);
  if ((FIFOn == NULL) && (FIFOs == NULL) && (FIFOe == NULL) && (FIFOo == NULL))
  {   fprintf(stderr, "lhtkernu() : CreeFifoVide failed\n");
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DES FIFOs: empile les voisins des maxima */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (M[x] != 0)                  /* le pixel appartient a un maximum */
    {
        Set(x, MAXI);
        y = voisin(x, NORD, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOn, y); Set(y, EN_FIFO); }
        y = voisin(x, NORD+1, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOn, y); Set(y, EN_FIFO); }
        y = voisin(x, EST, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOe, y); Set(y, EN_FIFO); }
        y = voisin(x, EST+1, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOe, y); Set(y, EN_FIFO); }
        y = voisin(x, SUD, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOs, y); Set(y, EN_FIFO); }
        y = voisin(x, SUD+1, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOs, y); Set(y, EN_FIFO); }
        y = voisin(x, OUEST, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOo, y); Set(y, EN_FIFO); }
        y = voisin(x, OUEST+1, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
          { FifoPush(FIFOo, y); Set(y, EN_FIFO); }
    } /* if (M[x] != 0) */
  } /* for x */

  freeimage(lab);

  FIFOna = CreeFifoVide(N/4);
  FIFOsa = CreeFifoVide(N/4);
  FIFOea = CreeFifoVide(N/4);
  FIFOoa = CreeFifoVide(N/4);
  if ((FIFOna == NULL) && (FIFOsa == NULL) && (FIFOea == NULL) && (FIFOoa == NULL))
  {   fprintf(stderr, "lhtkernu() : CreeFifoVide failed\n");
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  niter = 0;

if (connex == 4)  
{
  while (! (FifoVide(FIFOn) && FifoVide(FIFOe) && FifoVide(FIFOs) && FifoVide(FIFOo))
          && ((nitermax == 0) || (niter < nitermax))
        )
  {
    niter++;
    while (! FifoVide(FIFOn))
    {
      x = FifoPop(FIFOn);
      UnSet(x, EN_FIFO);
      if (testeleve4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi4(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve4(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOn)) */

    while (! FifoVide(FIFOs))
    {
      x = FifoPop(FIFOs);
      UnSet(x, EN_FIFO);
      if (testeleve4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi4(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve4(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOs)) */

    while (! FifoVide(FIFOe))
    {
      x = FifoPop(FIFOe);
      UnSet(x, EN_FIFO);
      if (testeleve4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi4(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve4(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOe)) */

    while (! FifoVide(FIFOo))
    {
      x = FifoPop(FIFOo);
      UnSet(x, EN_FIFO);
      if (testeleve4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi4(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve4(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOo)) */

    FIFOtmp = FIFOn; FIFOn = FIFOna; FIFOna = FIFOtmp;
    FIFOtmp = FIFOe; FIFOe = FIFOea; FIFOea = FIFOtmp;
    FIFOtmp = FIFOs; FIFOs = FIFOsa; FIFOsa = FIFOtmp;
    FIFOtmp = FIFOo; FIFOo = FIFOoa; FIFOoa = FIFOtmp;

  } /* while (! (FifoVide(FIFOn) && FifoVide(FIFOe) && FifoVide(FIFOs) && FifoVide(FIFOo))) */
}

else /* connex == 8 */

{
  while (! (FifoVide(FIFOn) && FifoVide(FIFOe) && FifoVide(FIFOs) && FifoVide(FIFOo))
          && ((nitermax == 0) || (niter < nitermax))
        )
  {
    niter++;
    while (! FifoVide(FIFOn))
    {
      x = FifoPop(FIFOn);
      UnSet(x, EN_FIFO);
      if (testeleve8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi8(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve8(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOn)) */

    while (! FifoVide(FIFOs))
    {
      x = FifoPop(FIFOs);
      UnSet(x, EN_FIFO);
      if (testeleve8(SOURCE, x, rs, N))        /* modifie l'image le cas echeant */
      {
        testmaxi8(SOURCE, x, rs, N);           /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve8(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOs)) */

    while (! FifoVide(FIFOe))
    {
      x = FifoPop(FIFOe);
      UnSet(x, EN_FIFO);
      if (testeleve8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi8(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve8(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOe)) */

    while (! FifoVide(FIFOo))
    {
      x = FifoPop(FIFOo);
      UnSet(x, EN_FIFO);
      if (testeleve8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
      {
        testmaxi8(SOURCE, x, rs, N);            /* reactualise l'image MAXI */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
      } /* if (testeleve8(SOURCE, x, rs, N)) */
    } /* while (! FifoVide(FIFOo)) */

    FIFOtmp = FIFOn; FIFOn = FIFOna; FIFOna = FIFOtmp;
    FIFOtmp = FIFOe; FIFOe = FIFOea; FIFOea = FIFOtmp;
    FIFOtmp = FIFOs; FIFOs = FIFOsa; FIFOsa = FIFOtmp;
    FIFOtmp = FIFOo; FIFOo = FIFOoa; FIFOoa = FIFOtmp;

  } /* while (! (FifoVide(FIFOn) && FifoVide(FIFOe) && FifoVide(FIFOs) && FifoVide(FIFOo))) */
}

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FifoTermine(FIFOn);
  FifoTermine(FIFOe);
  FifoTermine(FIFOs);
  FifoTermine(FIFOo);
  FifoTermine(FIFOna);
  FifoTermine(FIFOea);
  FifoTermine(FIFOsa);
  FifoTermine(FIFOoa);

#ifdef PERF
  save_time(N, read_chrono(&chrono1), "lhtkernu", image->name);    
#endif
  return(1);
}
/* -library_code_end */
