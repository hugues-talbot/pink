/* $Id: llambdakern.c,v 1.2 2009-03-13 14:46:14 mcouprie Exp $ */
/* noyau lambda-homotopique ou de nivellement par abaissement */
/* Michel Couprie - avril 1999 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mclifo.h>
#include <mcfahpure.h>
#include <mctopo.h>
#include <mcindic.h>
#include <mcutil.h>
#include <llabelextrema.h>
#include <llambdakern.h>

#define EN_FAHP       0
#define EN_LIFO      1
#define PARANO
#define VERBOSE
/*
#define DEBUG
*/

/* ==================================== */
static int32_t testabaisse4(uint8_t *F, int32_t x, int32_t rs, int32_t N, int32_t lambda)
/* ==================================== */
{
  int32_t modifie = 0;

#ifdef DEBUG
printf("testabaisse : point %d (%d %d), val = %d\n", x, x%rs, x/rs, F[x]);
#endif

#ifdef SQUEL
  if (!(extremite8(F,x,rs,N) && ((F[x] - alpha8m(F,x,rs,N)) > lambda)))
#endif
#ifdef NIVEL
  while ((lambdadestr4(F,x,lambda,rs,N)) || (peak4(F,x,rs,N)))
#else
  while (lambdadestr4(F,x,lambda,rs,N))
#endif
  { 
    modifie = 1; 
    F[x] = alpha8m(F,x,rs,N); /* alpha8m : sic */
  }

#ifdef DEBUG
if (modifie) printf("========> ABAISSE : %d\n", F[x]);
#endif

  return modifie;
} /* testabaisse4() */

/* ==================================== */
int32_t llambdakern(
  struct xvimage *image, 
  struct xvimage *imcon, 
  int32_t connex,
  int32_t lambda)
/* ==================================== */
#undef F_NAME
#define F_NAME "llambdakern"
{ 
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *G;            /* l'image de contrainte */
  struct xvimage *lab;
  uint32_t *M;            /* l'image d'etiquettes de composantes connexes */
  int32_t nminima;                 /* nombre de minima differents */
  Fahp * FAHP;

#ifdef VERBOSE
  printf("%s: connex=%d\n", F_NAME, connex);
  printf("%s: lambda=%d\n", F_NAME, lambda);
#endif

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  if (!imcon) 
  {
    imcon = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
    if (imcon == NULL)
    {   fprintf(stderr, "%s : allocimage failed\n", F_NAME);
        return(0);
    }
    G = UCHARDATA(imcon);
    memset(G, 0, N);
  }
  else
    G = UCHARDATA(imcon);
  

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  M = ULONGDATA(lab);

  if (!llabelextrema(image, connex, LABMIN, lab, &nminima))
  {   
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  IndicsInit(N);

  FAHP = CreeFahpVide(N);
  if (FAHP == NULL)
  {   fprintf(stderr, "%s : CreeFahpVide failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DE LA FAHP: empile les voisins des minima */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if ((F[x] > G[x]) && (M[x] != 0)) /* le pixel appartient a un minimum */
    {
      for (k = 0; k < 8; k++)
      {
        y = voisin(x, k, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FAHP) && nonbord(y,rs,N))
        { FahpPush(FAHP, y, F[y]); Set(y, EN_FAHP); }
      }
    } /* if (M[x] != 0) */
  } /* for x */

  freeimage(lab);

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (! FahpVide(FAHP))
    {
      x = FahpPop(FAHP);
      UnSet(x, EN_FAHP);
      if ((F[x] > G[x]) && testabaisse4(F, x, rs, N, lambda))         
        /* modifie l'image le cas echeant */
        for (k = 0; k < 8; k++)
        {
          y = voisin(x, k, rs, N);
          if ((y!=-1) && !IsSet(y,EN_FAHP) && nonbord(y,rs,N))
          { FahpPush(FAHP, y, F[y]); Set(y, EN_FAHP); }
        }
    } /* while (! FahpVide(FAHP)) */
  }
  else /* connex == 8 */
  {
    fprintf(stderr, "%s: connex 8 not yet implemented\n", F_NAME);
    return 0;
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahpTermine(FAHP);
  return(1);
} // llambdakern()

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/*
          LAMBDA-THIN, LAMBDA-THICK
 */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/*
   principe de l'encodage du alpha-: 
   la valeur alpha- de x est placee dans les bits 24 a 31 de l'int32_t contenant x
                                                      24
   il reste donc 24 bits pour coder les pixels, soit 2   = 16 megapixels.
   (ex: image 4k x 4k)

   une valeur -1 pour nitermax signifie de laisser calculer jusqu'a stabilite (en fait
   2000000000 iterations) 
*/

#define ENCODE(y,a) (y|(a<<24))
#define DECODEX(y) (y&0x00ffffff)
#define DECODEA(y) ((y>>24)&0xff)

/* ==================================== */
int32_t llambdathin(
  struct xvimage *image,
  struct xvimage *imagecond,
  int32_t nitermax,
  int32_t connex,
  int32_t lambda)
/* ==================================== */
#undef F_NAME
#define F_NAME "llambdathin"
{ 
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  uint32_t y;              /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *G;            /* l'image de conditionnement */
  int32_t niter;                   /* nombre d'iterations effectuees */
  Lifo * LIFO1;
  Lifo * LIFO2;
  Lifo * LIFOtmp;
  int32_t incr_vois;
  int32_t a;

#ifdef VERBOSE
  printf("%s: lambda=%d\n", F_NAME, lambda);
  printf("%s: nitermax = %d\n", F_NAME, nitermax);
#endif

  if (nitermax == -1) nitermax = 2000000000;   

  if (connex == 4) incr_vois = 2; else incr_vois = 1;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  if (N > (1<<24)) 
  {
    fprintf(stderr, "%s: image trop grande (limite 16 M)\n", F_NAME);
    return(0);
  }

  if (imagecond != NULL)
  {
    G = UCHARDATA(imagecond);
    if ((rowsize(imagecond) != rs) || (colsize(imagecond) != cs) || (depth(imagecond) != 1))
    {
      fprintf(stderr, "%s: tailles image et imagecond incompatibles\n", F_NAME);
      return(0);
    }
  }

  IndicsInit(N);

  LIFO1 = CreeLifoVide(N);
  LIFO2 = CreeLifoVide(N);
  if ((LIFO1 == NULL) || (LIFO2 == NULL))
  {   fprintf(stderr, "%s() : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DE LA LIFO: empile tous les points destructibles avec leur alpha-  */
  /* ========================================================= */

  if (connex == 4)
  {
    if (imagecond != NULL)
    {
      for (x = 0; x < N; x++) 
#ifdef NIVEL
        if ((G[x] < F[x]) && ((lambdadestr4(F,x,lambda,rs,N)) || (peak4(F,x,rs,N))))
#else
        if ((G[x] < F[x]) && (lambdadestr4(F,x,lambda,rs,N))) 
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
    }
    else
    {
      for (x = 0; x < N; x++) 
#ifdef NIVEL
        if ((lambdadestr4(F,x,lambda,rs,N)) || (peak4(F,x,rs,N)))
#else
        if (lambdadestr4(F,x,lambda,rs,N)) 
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
    }
  }
  else
  {
    if (imagecond != NULL)
    {
      for (x = 0; x < N; x++) 
#ifdef NIVEL
        if ((G[x] < F[x]) && ((lambdadestr8(F,x,lambda,rs,N)) || (peak8(F,x,rs,N))))
#else
        if ((G[x] < F[x]) && (lambdadestr8(F,x,lambda,rs,N))) 
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
    }
    else
    {
      for (x = 0; x < N; x++) 
#ifdef NIVEL
        if ((lambdadestr8(F,x,lambda,rs,N)) || (peak8(F,x,rs,N)))
#else
        if (lambdadestr8(F,x,lambda,rs,N))
#endif
        {
#ifdef DEBUG
          printf("init : push %d,%d, alpha = %d\n", x%rs, x/rs, alpha8m(F,x,rs,N));
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
	}
    }
  }

  /* ================================================ */
  /*                  DEBUT ITERATIONS                */
  /* ================================================ */

  niter = 0;

  if (connex == 4)
  {
    while (!LifoVide(LIFO1) && (niter < nitermax))
    {
      niter++;

#ifdef VERBOSE
  fprintf(stderr, "%s: niter = %d\n", F_NAME, niter);
#endif

  /* --------------------------------------------------------- */
  /* 1ere demi iteration : on abaisse les points destructibles */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO1))
      {
        y = LifoPop(LIFO1);
        x = DECODEX(y);
        a = DECODEA(y);
        UnSet(x,EN_LIFO);
#ifdef DEBUG
        printf("Pop x = %d,%d ; F[x] = %d ; a = %d\n", x%rs, x/rs, F[x], a);
#endif
#ifdef NIVEL
	if ((lambdadestr4(F,x,lambda,rs,N)) || (peak4(F,x,rs,N)))
#else
        if (lambdadestr4(F,x,lambda,rs,N))
#endif
	{
          F[x] = max(alpha8m(F, x, rs, N),a);
#ifdef DEBUG
          printf("Abaisse x a %d\n", F[x]);
#endif
          if (imagecond != NULL) F[x] = max(F[x],G[x]);
          LifoPush(LIFO2, x);
        } /* if (lambdadestr4(F,x,lambda,rs,N)) */
      } /* while (!LifoVide(LIFO1)) */

  /* --------------------------------------------------------- */
  /* 2eme demi iteration : on empile les voisins */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO2))
      {
        x = LifoPop(LIFO2);
#ifdef NIVEL
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] < F[x])) && 
	    ((lambdadestr4(F,x,lambda,rs,N)) || (peak4(F,x,rs,N))))
#else
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] < F[x])) && 
	    (lambdadestr4(F,x,lambda,rs,N)))
#endif
        {
#ifdef DEBUG
          printf("Push x = %d,%d ; F[x] = %d ; a- = %d\n", 
                  x%rs, x/rs, F[x], alpha8m(F, x, rs, N));
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
          Set(x,EN_LIFO);
	}
        for (k = 0; k < 8; k += 1)             /* parcourt les voisins */
        {                                      /* pour empiler les voisins */
          y = voisin(x, k, rs, N);             /* non deja empiles */
#ifdef NIVEL
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] < F[y])) && 
	      ((lambdadestr4(F,y,lambda,rs,N)) || (peak4(F,y,rs,N))))
#else
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] < F[y])) && 
	      (lambdadestr4(F,y,lambda,rs,N)))
#endif
          {
#ifdef DEBUG
            printf("Push y = %d,%d ; F[y] = %d ; a- = %d\n", 
                    y%rs, y/rs, F[y], alpha8m(F, y, rs, N));
#endif
            LifoPush(LIFO1, ENCODE(y,alpha8m(F,y,rs,N)));
            Set(y,EN_LIFO);
          } /* if y */
        } /* for k */      
      } /* while (!LifoVide(LIFO2)) */

    } /* while (! (LifoVide(LIFO1) ...)) */
  } /* if (connex == 4) */
  else
  { /* if (connex == 8) */
    while (!LifoVide(LIFO1) && (niter < nitermax))
    {
      niter++;

#ifdef VERBOSE
  fprintf(stderr, "%s: niter = %d\n", F_NAME, niter);
#endif

  /* --------------------------------------------------------- */
  /* 1ere demi iteration : on abaisse les points destructibles */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO1))
      {
        y = LifoPop(LIFO1);
        x = DECODEX(y);
        a = DECODEA(y);
        UnSet(x,EN_LIFO);
#ifdef DEBUG
        printf("Pop x = %d,%d ; F[x] = %d ; a = %d\n", x%rs, x/rs, F[x], a);
#endif
#ifdef NIVEL
        if ((lambdadestr8(F,x,lambda,rs,N)) || (peak8(F,x,rs,N)))
#else
        if (lambdadestr8(F,x,lambda,rs,N))
#endif
	{
          F[x] = max(alpha8m(F, x, rs, N),a);
#ifdef DEBUG
          printf("Abaisse x a %d\n", F[x]);
#endif
          if (imagecond != NULL) F[x] = max(F[x],G[x]);
          LifoPush(LIFO2, x);
        } /* if (lambdadestr8(F,x,lambda,rs,N)) */
      } /* while (!LifoVide(LIFO1)) */

  /* --------------------------------------------------------- */
  /* 2eme demi iteration : on empile les voisins */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO2))
      {
        x = LifoPop(LIFO2);
#ifdef NIVEL
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] < F[x])) && 
	    ((lambdadestr8(F,x,lambda,rs,N)) || (peak8(F,x,rs,N))))
#else
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] < F[x])) && 
	    (lambdadestr8(F,x,lambda,rs,N)))
#endif
        {
#ifdef DEBUG
          printf("Push x = %d,%d ; F[x] = %d ; a- = %d\n", 
                  x%rs, x/rs, F[x], alpha8m(F, x, rs, N));
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8m(F,x,rs,N)));
          Set(x,EN_LIFO);
	}
        for (k = 0; k < 8; k += 1)             /* parcourt les voisins */
        {                                      /* pour empiler les voisins */
          y = voisin(x, k, rs, N);             /* non deja empiles */
#ifdef NIVEL
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] < F[y])) && 
	      ((lambdadestr8(F,y,lambda,rs,N)) || (peak8(F,y,rs,N))))
#else
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] < F[y])) && 
	      (lambdadestr8(F,y,lambda,rs,N)))
#endif
          {
#ifdef DEBUG
            printf("Push y = %d,%d ; F[y] = %d ; a- = %d\n", 
                    y%rs, y/rs, F[y], alpha8m(F, y, rs, N));
#endif
            LifoPush(LIFO1, ENCODE(y,alpha8m(F,y,rs,N)));
            Set(y,EN_LIFO);
          } /* if y */
        } /* for k */      
      } /* while (!LifoVide(LIFO2)) */

    } /* while (! (LifoVide(LIFO1) ...)) */
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  LifoTermine(LIFO1);
  LifoTermine(LIFO2);
  return(1);
} /* llambdathin() */

/* ==================================== */
int32_t llambdathick(
  struct xvimage *image,
  struct xvimage *imagecond,
  int32_t nitermax,
  int32_t connex,
  int32_t lambda)
/* ==================================== */
#undef F_NAME
#define F_NAME "llambdathick"
{ 
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  uint32_t y;              /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *G;            /* l'image de conditionnement */
  int32_t niter;                   /* nombre d'iterations effectuees */
  Lifo * LIFO1;
  Lifo * LIFO2;
  Lifo * LIFOtmp;
  int32_t incr_vois;
  int32_t a;

  if (nitermax == -1) nitermax = 2000000000;   

  if (connex == 4) incr_vois = 2; else incr_vois = 1;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    return(0);
  }

  if (N > (1<<24)) 
  {
    fprintf(stderr, "%s: image trop grande (limite 16 M)\n", F_NAME);
    return(0);
  }

  if (imagecond != NULL)
  {
    G = UCHARDATA(imagecond);
    if ((rowsize(imagecond) != rs) || (colsize(imagecond) != cs) || (depth(imagecond) != 1))
    {
      fprintf(stderr, "%s: tailles image et imagecond incompatibles\n", F_NAME);
      return(0);
    }
  }

  IndicsInit(N);

  LIFO1 = CreeLifoVide(N);
  LIFO2 = CreeLifoVide(N);
  if ((LIFO1 == NULL) || (LIFO2 == NULL))
  {   fprintf(stderr, "%s() : CreeLifoVide failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DE LA LIFO: empile tous les points constructibles avec leur alpha+  */
  /* ========================================================= */

  if (connex == 4)
  {
    if (imagecond != NULL)
    {
      for (x = 0; x < N; x++) 
        if ((G[x] > F[x]) && (lambdaconstr4(F,x,lambda,rs,N))) 
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
    }
    else
    {
      for (x = 0; x < N; x++) 
        if (lambdaconstr4(F,x,lambda,rs,N)) 
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
    }
  }
  else
  {
    if (imagecond != NULL)
    {
      for (x = 0; x < N; x++) 
        if ((G[x] > F[x]) && (lambdaconstr8(F,x,lambda,rs,N))) 
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
    }
    else
    {
      for (x = 0; x < N; x++) 
        if (lambdaconstr8(F,x,lambda,rs,N)) 
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
    }
  }

  /* ================================================ */
  /*                  DEBUT ITERATIONS                */
  /* ================================================ */

  niter = 0;

  if (connex == 4)
  {
    while (!LifoVide(LIFO1) && (niter < nitermax))
    {
      niter++;

#ifdef VERBOSE
  fprintf(stderr, "%s: niter = %d\n", F_NAME, niter);
#endif

  /* --------------------------------------------------------- */
  /* 1ere demi iteration : on eleve les points constructibles */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO1))
      {
        y = LifoPop(LIFO1);
        x = DECODEX(y);
        a = DECODEA(y);
        UnSet(x,EN_LIFO);
#ifdef DEBUG
        printf("Pop x = %d,%d ; F[x] = %d ; a = %d\n", x%rs, x/rs, F[x], a);
#endif
        if (lambdaconstr4(F,x,lambda,rs,N))
	{
          F[x] = min(alpha8p(F, x, rs, N),a);
          if (imagecond != NULL) F[x] = min(F[x],G[x]);
          LifoPush(LIFO2, x);
        } /* if (lambdaconstr4(F,x,lambda,rs,N)) */
      } /* while (!LifoVide(LIFO1)) */

  /* --------------------------------------------------------- */
  /* 2eme demi iteration : on empile les voisins */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO2))
      {
        x = LifoPop(LIFO2);
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] > F[x])) && (lambdaconstr4(F,x,lambda,rs,N)))
        {
#ifdef DEBUG
          printf("Push x = %d,%d ; F[x] = %d ; a- = %d\n", 
                  x%rs, x/rs, F[x], alpha8p(F, x, rs, N));
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
          Set(x,EN_LIFO);
	}
        for (k = 0; k < 8; k += 1)             /* parcourt les voisins */
        {                                      /* pour empiler les voisins */
          y = voisin(x, k, rs, N);             /* non deja empiles */
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] > F[y])) && (lambdaconstr4(F,y,lambda,rs,N)))
          {
#ifdef DEBUG
            printf("Push y = %d,%d ; F[y] = %d ; a- = %d\n", 
                    y%rs, y/rs, F[y], alpha8p(F, y, rs, N));
#endif
            LifoPush(LIFO1, ENCODE(y,alpha8p(F,y,rs,N)));
            Set(y,EN_LIFO);
          } /* if y */
        } /* for k */      
      } /* while (!LifoVide(LIFO2)) */

    } /* while (! (LifoVide(LIFO1) ...)) */
  } /* if (connex == 4) */
  else
  { /* if (connex == 8) */
    while (!LifoVide(LIFO1) && (niter < nitermax))
    {
      niter++;

#ifdef VERBOSE
  fprintf(stderr, "%s: niter = %d\n", F_NAME, niter);
#endif

  /* --------------------------------------------------------- */
  /* 1ere demi iteration : on eleve les points constructibles */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO1))
      {
        y = LifoPop(LIFO1);
        x = DECODEX(y);
        a = DECODEA(y);
        UnSet(x,EN_LIFO);
#ifdef DEBUG
        printf("Pop x = %d,%d ; F[x] = %d ; a = %d\n", x%rs, x/rs, F[x], a);
#endif
        if (lambdaconstr8(F,x,lambda,rs,N))
	{
          F[x] = min(alpha8p(F, x, rs, N),a);
          if (imagecond != NULL) F[x] = min(F[x],G[x]);
          LifoPush(LIFO2, x);
        } /* if (lambdaconstr8(F,x,lambda,rs,N)) */
      } /* while (!LifoVide(LIFO1)) */

  /* --------------------------------------------------------- */
  /* 2eme demi iteration : on empile les voisins */
  /* --------------------------------------------------------- */

      while (!LifoVide(LIFO2))
      {
        x = LifoPop(LIFO2);
        if ((! IsSet(x,EN_LIFO)) &&
            ((imagecond == NULL) || (G[x] > F[x])) && (lambdaconstr8(F,x,lambda,rs,N)))
        {
#ifdef DEBUG
          printf("Push x = %d,%d ; F[x] = %d ; a- = %d\n", 
                  x%rs, x/rs, F[x], alpha8p(F, x, rs, N));
#endif
          LifoPush(LIFO1, ENCODE(x,alpha8p(F,x,rs,N)));
          Set(x,EN_LIFO);
	}
        for (k = 0; k < 8; k += 1)             /* parcourt les voisins */
        {                                      /* pour empiler les voisins */
          y = voisin(x, k, rs, N);             /* non deja empiles */
          if ((y != -1) && (! IsSet(y,EN_LIFO)) &&
              ((imagecond == NULL) || (G[y] > F[y])) && (lambdaconstr8(F,y,lambda,rs,N)))
          {
#ifdef DEBUG
            printf("Push y = %d,%d ; F[y] = %d ; a- = %d\n", 
                    y%rs, y/rs, F[y], alpha8p(F, y, rs, N));
#endif
            LifoPush(LIFO1, ENCODE(y,alpha8p(F,y,rs,N)));
            Set(y,EN_LIFO);
          } /* if y */
        } /* for k */      
      } /* while (!LifoVide(LIFO2)) */

    } /* while (! (LifoVide(LIFO1) ...)) */
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  LifoTermine(LIFO1);
  LifoTermine(LIFO2);
  return(1);
} /* llambdathick() */

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/*
          GRAYSKEL
 */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/* ==================================== */
int32_t lgrayskel(
  struct xvimage *image, 
  struct xvimage *imcon, 
  int32_t connex,
  int32_t lambda)
/* ==================================== */
#undef F_NAME
#define F_NAME "lgrayskel"
{ 
  int32_t i;
  int32_t x;                       /* index muet de pixel */
  int32_t y;                       /* index muet (generalement un voisin de x) */
  int32_t z;                       /* index muet (generalement un voisin de y) */
  int32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *G;            /* l'image de contrainte */
  struct xvimage *lab;
  uint32_t *M;            /* l'image d'etiquettes de composantes connexes */
  int32_t nminima;                 /* nombre de minima differents */
  Fahp * FAHP;

#ifdef VERBOSE
  printf("%s: connex=%d\n", F_NAME, connex);
  printf("%s: lambda=%d\n", F_NAME, lambda);
#endif

  if (depth(image) != 1) 
  {
    fprintf(stderr, "%s: cette version ne traite pas les images volumiques\n", F_NAME);
    exit(0);
  }

  if (!imcon) 
  {
    imcon = allocimage(NULL, rs, cs, 1, VFF_TYP_1_BYTE);
    if (imcon == NULL)
    {   fprintf(stderr, "%s : allocimage failed\n", F_NAME);
        return(0);
    }
    G = UCHARDATA(imcon);
    memset(G, 0, N);
  }
  else
    G = UCHARDATA(imcon);

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "%s: allocimage failed\n", F_NAME);
    return 0;
  }
  M = ULONGDATA(lab);

  if (!llabelextrema(image, connex, LABMIN, lab, &nminima))
  {   
    fprintf(stderr, "%s: llabelextrema failed\n", F_NAME);
    return 0;
  }

  IndicsInit(N);

  FAHP = CreeFahpVide(N);
  if (FAHP == NULL)
  {   fprintf(stderr, "%s : CreeFahpVide failed\n", F_NAME);
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  /* ========================================================= */
  /*   INITIALISATION DE LA FAHP: empile les voisins des minima */
  /* ========================================================= */

  for (x = 0; x < N; x++)
  {
    if ((F[x] > G[x]) && (M[x] != 0)) /* le pixel appartient a un minimum */
    {
      for (k = 0; k < 8; k++)
      {
        y = voisin(x, k, rs, N);
        if ((y!=-1) && (M[y]==0) && !IsSet(y,EN_FAHP) && nonbord(y,rs,N))
        { FahpPush(FAHP, y, F[y]); Set(y, EN_FAHP); }
      }
    } /* if (M[x] != 0) */
  } /* for x */

  freeimage(lab);

  /* ================================================ */
  /*                  DEBUT SATURATION                */
  /* ================================================ */

  if (connex == 4)
  {
    while (! FahpVide(FAHP))
    {
      x = FahpPop(FAHP);
      UnSet(x, EN_FAHP);
      if ((F[x] > G[x]) && testabaisse4(F, x, rs, N, lambda))         
        /* modifie l'image le cas echeant */
        if (museparant4(F, x, rs, N, lambda))
          G[x] = F[x];
        else
          for (k = 0; k < 8; k++)
          {
            y = voisin(x, k, rs, N);
            if ((y!=-1) && !IsSet(y,EN_FAHP) && nonbord(y,rs,N))
            { FahpPush(FAHP, y, F[y]); Set(y, EN_FAHP); }
          }
    } /* while (! FahpVide(FAHP)) */
  }
  else /* connex == 8 */
  {
    fprintf(stderr, "%s: connex 8 not yet implemented\n", F_NAME);
    return 0;
  }

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahpTermine(FAHP);
  return(1);
} // lgrayskel()
