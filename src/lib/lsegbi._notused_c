/* $Id: lsegbi.c,v 1.1.1.1 2008-11-25 08:01:41 mcouprie Exp $ */
/* operateur de segmentation par inondation "double" a partir des min et des max */
/* utilise une File d'Attente Hierarchique */
/* Michel Couprie - fevrier 1997 */
/* version 1.0 du 18/02/97 */
/* version 2.0 du 13/05/99 : version homotopique */

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mcfahpure.h>
#include <mclifo.h>
#include <mcindic.h>
#include <mctopo.h>
#include <mcutil.h>
#include <llabelextrema.h>
#include <lsegbi.h>

#define PARANO                 /* even paranoid people have ennemies */
#define VERBOSE

/*
#define TRACEPOP
#define TRACEPUSH
#define TRACEFUS
#define TRACEDEM
*/

//#define GRADMAX 255
#define GRADMAX 128

/*
   Methode 1: segmentation HOMOTOPIQUE

   etiquette les minima avec 1, les maxima avec 0.

   pour chaque point y dans voisinage d'un point x dans un extremum, on empile dans la Fah
   au niveau | SOURCE[x] - SOURCE[y] |, le point y ainsi qu'un code sur 3 bits indiquant
   la direction de x.

   pour un minimum, on utilise le 4-voisinage, et pour un maximum le 8-voisinage.

   on retire iterativement un point y de la Fah, tant que Fah non vide.
   si y n'est toujours pas extremum, et si le gradient n'a pas augmente, et
     (si son voisin x code est maximum et y constructible jusqu'au niveau de x OU
      si son voisin x code est minimum et y destructible jusqu'au niveau de x)
   alors on fusionne y avec l'extremum voisin x
         on empile les 'gradients' entre y et ses voisins non-extrema

   CORRECTION DU GRADIENT

   le gradient inf ou sup peut etre multiplie par un coefficient C: 0<C<=1
   pour favoriser soit l'objet, soit le fond

====================================================================================

   Methode 2: segmentation NON HOMOTOPIQUE

   etiquette les minima avec 1, les maxima avec 0.

   pour chaque point y dans voisinage d'un point x dans un extremum, on empile dans la Fah
   au niveau | SOURCE[x] - SOURCE[y] |, le point y ainsi qu'un code sur 3 bits indiquant
   la direction de x.

   pour un minimum, on utilise le 4-voisinage, et pour un maximum le 8-voisinage.

   on retire iterativement un point y de la Fah, tant que Fah non vide. 
   si y n'est toujours pas extremum, et si son voisin x code est toujours extremum,
                                     et si le gradient n'a pas augmente,
                                     et si y n'est ni convergent ni divergent,
                                     et si y peut subir la transformation suivante sans
                                       changer la topologie : 

     on fusionne y avec l'extremum voisin x
 
     si deux minima d'altitudes differentes deviennent 4-voisins (resp. deux maxima 8-voisins) 
     il faut demarquer le plus haut en tant que minimum (resp. le plus bas en tant que maximum) 

     si y est reste extremum, on empile les 'gradients' entre y et ses voisins non-extrema

     si y a ete demarque, on empile les 'gradients' entre y et ses voisin extrema

   CORRECTION DU GRADIENT

   le gradient inf ou sup peut etre multiplie par un coefficient C: 0<C<=1
   pour favoriser soit l'objet, soit le fond

*/

/*
#define Cmin 1.0
#define Cmax 1.0
*/

#define CORRIGEMIN(g) ((uint8_t)(Cmin*g))
#define CORRIGEMAX(g) ((uint8_t)(Cmax*g))

#define MAXIMUM 0
#define MINIMUM 1

/* ==================================== */
static void Demarque(
  Lifo * LIFO,
  uint8_t *I,
  int32_t rs,
  int32_t N,
  int32_t incrvois,
  int32_t x)
/* ==================================== */
{
  int32_t y, k;
  uint8_t niv = I[x];


#ifdef TRACEDEM
printf("DEMARQUE extremum incluant le point %d (%d,%d) ; incrvois = %d\n", 
                  x, x%rs, x/rs, incrvois);
#endif

  LifoPush(LIFO, x);
  while (! LifoVide(LIFO))
  {
    x = LifoPop(LIFO);
    UnSetAll(x);
    for (k = 0; k < 8; k += incrvois)
    {
      y = voisin(x, k, rs, N);
      if ((y != -1) && (I[y] == niv) && (IsSetAny(y)))
        LifoPush(LIFO, y);
    } /* for k ... */
  } /* while (! LifoVide(LIFO)) */
} /* Demarque() */


/*
   y est un non-extremum, voisin de x extremum.
   principe de l'encodage du gradient : 
   le numero de voisin de y par rapport a x est stocke sur 3 bits, avec le codage suivant: 
		3	2	1			
		4	x	0
		5	6	7
   soit k.
   un point y est stocke avec la direction dans laquelle on trouve
   l'extremum x, avec le codage reciproque : 
                7       6       5
                0       y       4
                1       2       3
   soit (k+4) % 8.
   
   ce code est place dans les bits 29 a 31 de l'int32_t contenant y
                                                      29
   il reste donc 29 bits pour coder les pixels, soit 2   = 1/2 gigapixels.

*/

#define ENCODE(y,k) (y|(((k+4)%8)<<29))
#define ENCODEINV(y,k) (y|(k<<29))
#define DECODEPIX(y) (y&0x1fffffff)
#define DECODEDIR(y) (y>>29)

/* ==================================== */
int32_t lsegbinonhomotopique(
        struct xvimage *image,
        int32_t connexmin,
        double Cmin,
        double Cmax)
/* ==================================== */
{
  register uint32_t x, y, z, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *lab;
  uint32_t *M;            /* pour stocker temporairement les extrema  */
  int32_t nextrema;                /* nombre d'extrema differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  Lifo * LIFO;
  uint32_t direction;
  uint8_t niveau;
  int32_t tracedate = 0;
  int32_t grad;
  int32_t connexmax, incrvoismin, incrvoismax;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lsegbi: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connexmin)
  {
    case 4: incrvoismin = 2; incrvoismax = 1; connexmax = 8; break;
    case 8: incrvoismin = 1; incrvoismax = 2; connexmax = 4; break;
    default: 
      fprintf(stderr, "lsegbi: mauvaise connexite: %d\n", connexmin);
      return 0;
  } /* switch (connexmin) */

  IndicsInit(N);

  /* ===================================================== */
  /* etiquetage des min et des max en deux passes */
  /* pas optimal en temps de calul mais alloue la memoire pendant peu de temps */
  /* ===================================================== */

  lab = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if (lab == NULL)
  {   
    fprintf(stderr, "lsegbi: allocimage failed\n");
    exit(0);
  }
  M = ULONGDATA(lab);
  if (! llabelextrema(image, connexmax, LABMAX, lab, &nextrema))
  {
    fprintf(stderr, "lsegbi: llabelextrema failed\n");
    exit(0);
  }
  for (x = 0; x < N; x++) if (M[x]) Set(x, MAXIMUM);
  if (! llabelextrema(image, connexmin, LABMIN, lab, &nextrema))
  {
    fprintf(stderr, "lsegbi: llabelextrema failed\n");
    exit(0);
  }
  for (x = 0; x < N; x++) if (M[x]) Set(x, MINIMUM);
  freeimage(lab);

  FAH = CreeFahVide(4 * N);
  if (FAH == NULL)
  {   fprintf(stderr, "lsegbi() : CreeFah failed\n");
      return(0);
  }

  LIFO = CreeLifoVide(N);
  if (LIFO == NULL)
  {   fprintf(stderr, "lsegbi() : CreeLifoVide failed\n");
      return(0);
  }

  /* ===================================================== */
  /*                INITIALISATION DE LA FAH               */
  /* ===================================================== */
  
  for (x = 0; x < N; x++)
  {
    if (IsSet(x, MINIMUM))           
    {                                /* place les voisins non extrema de x dans la Fah */
      for (k = 0; k < 8; k += incrvoismin)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (!IsSetAny(y)))
	{
          FahPush(FAH, ENCODE(y,k), CORRIGEMIN((F[y]-F[x])));
#ifdef TRACEPUSH
printf("%d: empile(1) point %d (%d,%d) au niveau %d ; voisin MIN = %d (%d,%d)\n", 
       tracedate++, y, y%rs, y/rs, CORRIGEMIN((F[y]-F[x])), x, x%rs, x/rs);
#endif
	}
      } /* for k = 0 ... */
    } /* if (IsSet(x, MINIMUM)) */
    else
    if (IsSet(x, MAXIMUM))
    {                                /* place les voisins non extrema de x dans la Fah */
      for (k = 0; k < 8; k += incrvoismax)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (!IsSetAny(y)))
	{
          FahPush(FAH, ENCODE(y,k), CORRIGEMAX((F[x]-F[y])));
#ifdef TRACEPUSH
printf("%d: empile(2) point %d (%d,%d) au niveau %d ; voisin MAX = %d (%d,%d)\n", 
       tracedate++, y, y%rs, y/rs, CORRIGEMAX((F[x]-F[y])), x, x%rs, x/rs);
#endif
	}
      } /* for k = 0 ... */
    } /* if (IsSet(x, MAXIMUM)) */
  } /* for x */

  x = FahPop(FAH);

  /* ===================================================== */
  /* INONDATION */
  /* ===================================================== */

  while (! FahVide(FAH)) 
  {
    niveau = FahNiveau(FAH);
    y = FahPop(FAH);
    direction = DECODEDIR(y);
    y = DECODEPIX(y);
    x = voisin(y, direction, rs, N);

#ifdef TRACEPOP
printf("%d: POP point %d (%d,%d) ; niveau %d ; dir. %d ; extr. %d (%d,%d)\n", 
       tracedate++, y, y%rs, y/rs, niveau, direction, x, x%rs, x/rs);
#endif
    
    if ((!IsSetAny(y)) && (IsSetAny(x)))  /* y non extremum et x extremum */
    {
      if (IsSet(x, MAXIMUM)) grad = CORRIGEMAX((F[x]-F[y]));
      else                   grad = CORRIGEMIN((F[y]-F[x]));

      if (grad <= niveau)                  /* le gradient n'a pas augmente */
      {                                    /* "fusionne" le point y a l'extremum x */
#ifdef TRACEFUS
if (IsSet(x, MINIMUM))
  printf("%d: FUSIONNE point %d (%d,%d) au mINimum %d (%d,%d)\n", 
         tracedate++, y, y%rs, y/rs, x, x%rs, x/rs);
else
  printf("%d: FUSIONNE point %d (%d,%d) au mAXimum %d (%d,%d)\n", 
         tracedate++, y, y%rs, y/rs, x, x%rs, x/rs);
#endif
        Indics[y] = Indics[x];
        F[y] = F[x];

        /* teste l'extinction d'extrema */

        if (IsSet(y, MINIMUM))
        for (k = 0; k < 8; k += incrvoismin)
        {
          z = voisin(y, k, rs, N);
          if (z != -1)
          {
            if ((IsSet(z, MINIMUM)) && (F[z] > F[y]))  /* rencontre de 2 minima */
            Demarque(LIFO, F, rs, N, incrvoismin, z);
            else
            if (F[y] > F[z])         /* le "minimum" de y rencontre un point inferieur */ 
              Demarque(LIFO, F, rs, N, incrvoismin, y);
          } /* if (z != -1) */
        } /* for k = 0 ... */
        else
        if (IsSet(y, MAXIMUM))
        for (k = 0; k < 8; k += incrvoismax)
        {
          z = voisin(y, k, rs, N);
          if (z != -1)
          {
            if ((IsSet(z, MAXIMUM)) && (F[z] < F[y]))   /* rencontre de 2 maxima */
              Demarque(LIFO, F, rs, N, incrvoismax, z);
            else
            if (F[y] < F[z])         /* le "maximum" de y rencontre un point superieur */ 
              Demarque(LIFO, F, rs, N, incrvoismax, y);
          } /* if (z != -1) */
        } /* for k = 0 ... */
  
        /* recherche ou actualisation de candidats dans le voisinage */

        if (IsSet(y, MINIMUM))
        {
          for (k = 0; k < 8; k += incrvoismin)
          {
            z = voisin(y, k, rs, N);
            if ((z != -1) && (!IsSetAny(z)))
            {
              FahPush(FAH, ENCODE(z,k), CORRIGEMIN((F[z]-F[y])));
#ifdef TRACEPUSH
printf("%d: empile(3) point %d (%d,%d) au niveau %d ; voisin MIN = %d (%d,%d)\n", 
       tracedate++, z, z%rs, z/rs, CORRIGEMIN((F[z]-F[y])), y, y%rs, y/rs);
#endif
            }
          } /* for k = 0 ... */
        } /* if (IsSet(y, MINIMUM)) */
        else
        if (IsSet(y, MAXIMUM))
        {
          for (k = 0; k < 8; k += incrvoismax)
          {
            z = voisin(y, k, rs, N);
            if ((z != -1) && (!IsSetAny(z)))
            {
              FahPush(FAH, ENCODE(z,k), CORRIGEMAX((F[y]-F[z])));
#ifdef TRACEPUSH
printf("%d: empile(4) point %d (%d,%d) au niveau %d ; voisin MAX = %d (%d,%d)\n", 
       tracedate++, z, z%rs, z/rs, CORRIGEMAX((F[y]-F[z])), y, y%rs, y/rs);
#endif
	    }
          } /* for k = 0 ... */
        } /* if (IsSet(y, MAXIMUM)) */
        else                             /* y a ete "demarque" */
        {                                /* on doit chercher un extremum dans le voisinage */
          for (k = 0; k < 8; k += incrvoismax)
          {
            z = voisin(y, k, rs, N);
            if ((z != -1) && (IsSet(z, MAXIMUM)))
            {
              FahPush(FAH, ENCODEINV(y,k), CORRIGEMAX((F[z]-F[y])));
#ifdef TRACEPUSH
printf("%d: empile(5) point %d (%d,%d) au niveau %d ; voisin MAX = %d (%d,%d)\n", 
       tracedate++, y, y%rs, y/rs, CORRIGEMAX((F[z]-F[y])), z, z%rs, z/rs);
#endif
	    }
	  } /* for k = 0 ... */
          for (k = 0; k < 8; k += incrvoismin)
          {
            z = voisin(y, k, rs, N);
            if ((z != -1) && (IsSet(z, MINIMUM)))
            {
              FahPush(FAH, ENCODEINV(y,k), CORRIGEMIN((F[y]-F[z])));
#ifdef TRACEPUSH
printf("%d: empile(6) point %d (%d,%d) au niveau %d ; voisin MIN = %d (%d,%d)\n", 
       tracedate++, y, y%rs, y/rs, CORRIGEMIN((F[y]-F[z])), z, z%rs, z/rs);
#endif
	    }          
          } /* for k = 0 ... */
        } /* y a ete "demarque" */
      } /* if (grad <= niveau) */
    } /* if ((!IsSetAny(x)) ... */
  } /* while (! FahVide(FAH)) */
  /* FIN INONDATION */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  IndicsTermine();
  FahTermine(FAH);
  LifoTermine(LIFO);
  return(1);
} /* segbinonhomotopique() */

/* ==================================== */
int32_t lsegbihomotopique(
        struct xvimage *image,
        int32_t connexmin,
        double Cmin,
        double Cmax)
/* ==================================== */
{
  register uint32_t x, y, z, k;
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  struct xvimage *labmin;
  struct xvimage *labmax;
  int32_t *Min, *Max;
  int32_t nmin, nmax;              /* nombre d'extrema differents */
  Fah * FAH;                   /* la file d'attente hierarchique */
  uint32_t direction;
  uint8_t niveau;
  int32_t tracedate = 0;
  int32_t grad;
  int32_t connexmax, incrvoismin, incrvoismax;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lsegbi: cette version ne traite pas les images volumiques\n");
    exit(0);
  }

  switch (connexmin)
  {
    case 4: incrvoismin = 2; incrvoismax = 1; connexmax = 8; break;
    case 8: incrvoismin = 1; incrvoismax = 2; connexmax = 4; break;
    default: 
      fprintf(stderr, "lsegbi: mauvaise connexite: %d\n", connexmin);
      return 0;
  } /* switch (connexmin) */

  /* ===================================================== */
  /* etiquetage des min et des max en deux passes */
  /* pas optimal */
  /* ===================================================== */

  labmin = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  labmax = allocimage(NULL, rs, cs, 1, VFF_TYP_4_BYTE);
  if ((labmin == NULL) || (labmax == NULL))
  {   
    fprintf(stderr, "lsegbi: allocimage failed\n");
    exit(0);
  }
  Min = ULONGDATA(labmin);
  Max = ULONGDATA(labmax);

  if (! llabelextrema(image, connexmax, LABMAX, labmax, &nmax))
  {
    fprintf(stderr, "lsegbi: llabelextrema failed\n");
    exit(0);
  }

  if (! llabelextrema(image, connexmin, LABMIN, labmin, &nmin))
  {
    fprintf(stderr, "lsegbi: llabelextrema failed\n");
    exit(0);
  }

  for (x = 0; x < N; x++) /* on range dans labmin les etquettes des minima */
    if (Min[x] != 0)      /* (en negatif) et celles des maxima (en positif) */
      Min[x] = -Min[x];
    else if (Max[x] != 0) 
      Min[x] = Max[x];

  freeimage(labmax);

  /* ===================================================== */
  /*                INITIALISATION DE LA FAH               */
  /* ===================================================== */

  FAH = CreeFahVide(4 * N);
  if (FAH == NULL)
  {   fprintf(stderr, "lsegbi() : CreeFah failed\n");
      return(0);
  }
  
  for (x = 0; x < N; x++)
  {
    if (Min[x] < 0) /* MINIMUM */
    {                                /* place les voisins non extrema de x dans la Fah */
      for (k = 0; k < 8; k += incrvoismin)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (Min[y] == 0) && (CORRIGEMIN((F[y]-F[x])) <= GRADMAX))
	{
          FahPush(FAH, ENCODE(y,k), CORRIGEMIN((F[y]-F[x])));
#ifdef TRACEPUSH
printf("%d: empile(1) point (%d,%d|%d) prio %d ; voisin MIN = (%d,%d|%d)\n", 
       tracedate++, y%rs, y/rs, F[y], CORRIGEMIN((F[y]-F[x])), x%rs, x/rs, F[x]);
#endif
	}
      } /* for k = 0 ... */
    } /* if x MINIMUM */
    else
    if (Min[x] > 0) /* MAXIMUM */
    {                                /* place les voisins non extrema de x dans la Fah */
      for (k = 0; k < 8; k += incrvoismax)
      {
        y = voisin(x, k, rs, N);
        if ((y != -1) && (Min[y] == 0) && (CORRIGEMAX((F[x]-F[y])) <= GRADMAX))
	{
          FahPush(FAH, ENCODE(y,k), CORRIGEMAX((F[x]-F[y])));
#ifdef TRACEPUSH
printf("%d: empile(2) point (%d,%d|%d) prio %d ; voisin MAX = (%d,%d|%d)\n", 
       tracedate++, y%rs, y/rs, F[y], CORRIGEMAX((F[x]-F[y])), x%rs, x/rs, F[x]);
#endif
	}
      } /* for k = 0 ... */
    } /* if x MAXIMUM */
  } /* for x */

  x = FahPop(FAH);

  /* ===================================================== */
  /* INONDATION */
  /* ===================================================== */

fprintf(stderr, "debut inondation\n");

  while (! FahVide(FAH)) 
  {
    niveau = FahNiveau(FAH);
    y = FahPop(FAH);
    direction = DECODEDIR(y);
    y = DECODEPIX(y);
    x = voisin(y, direction, rs, N);

#ifdef TRACEPOP
printf("%d: POP point (%d,%d|%d) ; prio %d ; dir. %d ; extr. (%d,%d|%d)\n", 
       tracedate++, y%rs, y/rs, F[y], niveau, direction, x%rs, x/rs, F[x]);
#endif
    
    /*
     si y n'est toujours pas extremum, et si le gradient n'a pas augmente, et
       (si son voisin x code est maximum et y constructible jusqu'au niveau de x 
        OU
        si son voisin x code est minimum et y destructible jusqu'au niveau de x
       )
     alors on fusionne y avec l'extremum voisin x
           on empile les 'gradients' entre y et ses voisins non-extrema
    */
    if (Min[y] == 0)
    {
      if (Min[x] > 0) /* x MAXIMUM */
      { 
        grad = CORRIGEMAX((F[x]-F[y]));
        if (grad <= niveau)                  /* le gradient n'a pas augmente */
        {
          uint8_t oldvy = F[y];
          int32_t pp;
          if (connexmin == 4)
	  {
            pp = beta4m(F, y, rs, N); /* A AMELIORER: si le test pp echoue, ne pas faire la suite */
#define LAMBDA 255
#ifdef LAMBDA    
            while (lambdaconstr4(F, y, LAMBDA, rs, N)) F[y] = alpha8p(F, y, rs, N); /* 8p: sic */
#else                                                  
            while (pconstr4(F, y, rs, N)) F[y] = alpha8p(F, y, rs, N); /* 8p: sic */
#endif
	  }
          else /* if (connexmin == 8) */
	  {
            pp = beta8m(F, y, rs, N);
#ifdef LAMBDA    
            printf("lambdaconstr8: not yet implemented\n"); exit(0);
#else                                                  
            while (pconstr8(F, y, rs, N)) F[y] = alpha8p(F, y, rs, N);
#endif
	  }
          if ((F[y] != F[x]) || ((pp != -1) && PlusProche(oldvy,pp,F[x])))
            F[y] = oldvy;              /* on remet y a sa valeur originale */
          else
	  {
#ifdef TRACEFUS
            printf("%d: FUSIONNE point (%d,%d|%d) au mAXimum (%d,%d|%d)\n", 
            tracedate++, y%rs, y/rs, F[y], x%rs, x/rs, F[x]);
#endif
            Min[y] = Min[x];
            for (k = 0; k < 8; k += incrvoismax)
            {
              z = voisin(y, k, rs, N);
              if ((z != -1) && (Min[z] == 0) && (CORRIGEMAX((F[y]-F[z])) <= GRADMAX))
              {
                FahPush(FAH, ENCODE(z,k), CORRIGEMAX((F[y]-F[z])));
#ifdef TRACEPUSH
printf("%d: empile(4) point (%d,%d|%d) au niveau %d ; voisin MAX = (%d,%d|%d)\n", 
        tracedate++, z%rs, z/rs, F[z], CORRIGEMAX((F[y]-F[z])), y%rs, y/rs, F[y]);
#endif
	      }
            } /* for k = 0 ... */
	  }
	} /* if (grad <= niveau) */
      } /* if x MAXIMUM */
      else
      { /* if x  MINIMUM */
        grad = CORRIGEMIN((F[y]-F[x]));
        if (grad <= niveau)                  /* le gradient n'a pas augmente */
        {
          uint8_t oldvy = F[y];
          int32_t pp;
          if (connexmin == 4)
	  {
            pp = beta8p(F, y, rs, N);
#ifdef LAMBDA    
            while (lambdadestr4(F, y, LAMBDA, rs, N)) F[y] = alpha8m(F, y, rs, N); /* 8p: sic */
#else                                                  
            while (pdestr4(F, y, rs, N)) F[y] = alpha8m(F, y, rs, N); /* 8m: sic */
#endif
	  }
          else /* if (connexmin == 8) */
	  {
            pp = beta4p(F, y, rs, N);
#ifdef LAMBDA    
            printf("lambdadestr8: not yet implemented\n"); exit(0);
#else                                                  
            while (pdestr8(F, y, rs, N)) F[y] = alpha8m(F, y, rs, N);
#endif
	  }
          if ((F[y] != F[x]) || ((pp != -1) && PlusProche(oldvy,pp,F[x])))
            F[y] = oldvy;        /* on remet y a sa valeur originale */
          else
	  {
#ifdef TRACEFUS
            printf("%d: FUSIONNE point (%d,%d|%d) au mINimum (%d,%d|%d)\n", 
            tracedate++, y%rs, y/rs, F[y], x%rs, x/rs, F[x]);
#endif
            Min[y] = Min[x];
            for (k = 0; k < 8; k += incrvoismin)
            {
              z = voisin(y, k, rs, N);
              if ((z != -1) && (Min[z] == 0) && (CORRIGEMAX((F[z]-F[y])) <= GRADMAX))
              {
                FahPush(FAH, ENCODE(z,k), CORRIGEMIN((F[z]-F[y])));
#ifdef TRACEPUSH
printf("%d: empile(3) point (%d,%d|%d) au niveau %d ; voisin MIN = (%d,%d|%d)\n", 
       tracedate++, z%rs, z/rs, F[z], CORRIGEMIN((F[z]-F[y])), y%rs, y/rs, F[y]);
#endif
              }
            } /* for k = 0 ... */
	  }
        } /* if (grad <= niveau) */
      } /* if x MINIMUM */
    } /* if y non extremum ... */
  } /* while (! FahVide(FAH)) */
  /* FIN INONDATION */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  FahTermine(FAH);
  freeimage(labmin);
  return(1);
} /* segbihomotopique() */
