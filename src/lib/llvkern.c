/* $Id: llvkern.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* operateur de calcul du noyau de nivellement */
/* methode: saturation avec politique 4-directionnelle 
   utilise 4 fifo */
/* Michel Couprie -  avril 1997 */
/* juin 1999 : OBSOLETE - voir lhtkern.c */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfifo.h>
#include <mctopo.h>
#include <mcindic.h>
#include <llabelextrema.h>
#include <llvkern.h>

#define EN_FIFO       0
#define PARANO
/*
#define DEBUG
*/

/* ==================================== */
static 
int32_t testabaisse4(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t t4mm, t4m, t8p, t8pp;
  int32_t modifie = 0;

#ifdef DEBUG
printf("testabaisse : point %d (%d %d), val = %d\n", x, x%rs, x/rs, SOURCE[x]);
#endif

  nbtopo(SOURCE, x, rs, N, &t4mm, &t4m, &t8p, &t8pp);
  while (t4mm == 1)
  {
    modifie = 1;
    SOURCE[x] = alpha8m(SOURCE, x, rs, N);
    nbtopo(SOURCE, x, rs, N, &t4mm, &t4m, &t8p, &t8pp);
  }

#ifdef DEBUG
if (modifie) printf("========> ABAISSE : %d\n", SOURCE[x]);
#endif

  return modifie;
} /* testabaisse4() */

/* ==================================== */
static 
int32_t testabaisse8(uint8_t *SOURCE, int32_t x, int32_t rs, int32_t N)
/* ==================================== */
{
  int32_t t8mm, t8m, t4p, t4pp;
  int32_t modifie = 0;

#ifdef DEBUG
printf("testabaisse : point %d (%d %d), val = %d\n", x, x%rs, x/rs, SOURCE[x]);
#endif

  nbtopo2(SOURCE, x, rs, N, &t8mm, &t8m, &t4p, &t4pp);
  while (t8mm == 1)
  {
    modifie = 1;
    SOURCE[x] = alpha8m(SOURCE, x, rs, N);
    nbtopo2(SOURCE, x, rs, N, &t8mm, &t8m, &t4p, &t4pp);
  }

#ifdef DEBUG
if (modifie) printf("========> ABAISSE : %d\n", SOURCE[x]);
#endif

  return modifie;
} /* testabaisse8() */

/* ==================================== */
static
void empilevoisins(int32_t x, int32_t rs, int32_t N, Fifo *FIFOna, Fifo *FIFOea, Fifo *FIFOsa, Fifo *FIFOoa)
/* ==================================== */
{
  int32_t y;

  y = voisin(x, NORD, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOna, y); Set(y, EN_FIFO); }
  y = voisin(x, NORD+1, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOna, y); Set(y, EN_FIFO); }
  y = voisin(x, EST, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOea, y); Set(y, EN_FIFO); }
  y = voisin(x, EST+1, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOea, y); Set(y, EN_FIFO); }
  y = voisin(x, SUD, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOsa, y); Set(y, EN_FIFO); }
  y = voisin(x, SUD+1, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOsa, y); Set(y, EN_FIFO); }
  y = voisin(x, OUEST, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOoa, y); Set(y, EN_FIFO); }
  y = voisin(x, OUEST+1, rs, N);
  if ((y!=-1) && !IsSet(y,EN_FIFO) && nonbord(y,rs,N))
    { FifoPush(FIFOoa, y); Set(y, EN_FIFO); }
} /* empilevoisins() */

/* ==================================== */
int32_t llvkern_llvkern(struct xvimage *image, int32_t nitermax, int32_t connex)
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
  int32_t nminima;                 /* nombre de minima differents */
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

  if (depth(image) != 1) 
  {
    fprintf(stderr, "llvkern: cette version ne traite pas les images volumiques\n");
    return 0;
  }

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "lhtkern: allocimage failed\n");
    return 0;
  }
  M = ULONGDATA(lab);

  if (!llabelextrema(image, connex, LABMIN, lab, &nminima))
  {   
    fprintf(stderr, "lhtkern: llabelextrema failed\n");
    return 0;
  }

  IndicsInit(N);

  FIFOn = CreeFifoVide(N/2);
  FIFOs = CreeFifoVide(N/2);
  FIFOe = CreeFifoVide(N/2);
  FIFOo = CreeFifoVide(N/2);
  if ((FIFOn == NULL) && (FIFOs == NULL) && (FIFOe == NULL) && (FIFOo == NULL))
  {   fprintf(stderr, "llvkern() : CreeFifoVide failed\n");
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DES FIFOs: empile les voisins des minima */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if (M[x] != 0)                  /* le pixel appartient a un minimum */
    {
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
  {   fprintf(stderr, "llvkern() : CreeFifoVide failed\n");
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
      if (testabaisse4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOn)) */

    while (! FifoVide(FIFOs))
    {
      x = FifoPop(FIFOs);
      UnSet(x, EN_FIFO);
      if (testabaisse4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOs)) */

    while (! FifoVide(FIFOe))
    {
      x = FifoPop(FIFOe);
      UnSet(x, EN_FIFO);
      if (testabaisse4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOe)) */

    while (! FifoVide(FIFOo))
    {
      x = FifoPop(FIFOo);
      UnSet(x, EN_FIFO);
      if (testabaisse4(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
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
      if (testabaisse8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOn)) */

    while (! FifoVide(FIFOs))
    {
      x = FifoPop(FIFOs);
      UnSet(x, EN_FIFO);
      if (testabaisse8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOs)) */

    while (! FifoVide(FIFOe))
    {
      x = FifoPop(FIFOe);
      UnSet(x, EN_FIFO);
      if (testabaisse8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
    } /* while (! FifoVide(FIFOe)) */

    while (! FifoVide(FIFOo))
    {
      x = FifoPop(FIFOo);
      UnSet(x, EN_FIFO);
      if (testabaisse8(SOURCE, x, rs, N))         /* modifie l'image le cas echeant */
        empilevoisins(x, rs, N, FIFOna, FIFOea, FIFOsa, FIFOoa);
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
  save_time(N, read_chrono(&chrono1), "llvkern", image->name);    
#endif
  return(1);
}
/* -library_code_end */
