/* $Id: lhthiniso.c,v 1.2 2009-01-06 13:18:15 mcouprie Exp $ */
/* 
   hthiniso: amincissement avec reduction d'anisotropie

   Michel Couprie - aout 2000
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <math.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <mctopo.h>
#include <mcutil.h>
#include <lhthiniso.h>
/*
#define DEBUGRBT
#define DEBUGISO
*/

#define PARANO

#define ENCODEDIR(x,k) (x|(k<<29))
#define DECODEPIX(x) (x&0x1fffffff)
#define DECODEDIR(x) (x>>29)

/* ========================================================================== 
   Fonctions pour la gestion d'un arbre rouge et noir

   Implementation specifique pour hthiniso. Cette implementation specifique
   est rendue necessaire par le maintien d'une structure de 
   pointeurs sur les noeuds de l'arbre. Or, ces noeuds peuvent changer de 
   place a l'occasion de certaines mises a jour (destrutions). 
   Une modification du code de "RbtDeleteAux" a donc ete effectuee.

   D'apres "Introduction a l'algorithmique", 
     T. Cormen, C. Leiserson, R. Rivest, pp. 258, Dunod Ed. 
   ========================================================================== */

#define Black 0
#define Red   1

typedef double TypRbtKey;
typedef uint8_t TypRbtKey2;
typedef uint32_t TypRbtAuxData;

typedef struct RBTELT {
  TypRbtAuxData auxdata;
  TypRbtKey key;
  TypRbtKey2 key2;
  char color;
  struct RBTELT * left;
  struct RBTELT * right;
  struct RBTELT * parent;
} RbtElt;

typedef struct {
  int32_t max;             /* taille max du rbt (en nombre de points) */
  int32_t util;            /* nombre de points courant dans le rbt */
  int32_t maxutil;         /* nombre de points utilises max (au cours du temps) */
  RbtElt *root;        /* racine de l'arbre */
  RbtElt *nil;         /* sentinelle et element dont l'adresse joue le role de NIL */
  RbtElt *libre;       /* pile des cellules libres */
  RbtElt elts[1];      /* tableau des elements physiques */
} Rbt;

/* ==================================== */
Rbt * CreeRbtVide(
  int32_t taillemax)
/* ==================================== */
{
  int32_t i;
  Rbt * T = (Rbt *)calloc(1,sizeof(Rbt) + taillemax*sizeof(RbtElt));
  /* le tableau Elts du Rbt peut stocker taillemax+1 elements, dont 1 pour nil */
  if (T == NULL)
  {   fprintf(stderr, "CreeRbtVide() : malloc failed\n");
      return NULL;
  }
  T->max = taillemax;
  T->util = 0;
  T->maxutil = 0;

  /* chaine les elements libres a l'aide du pointeur right */
  for (i = 0; i < taillemax - 1; i++) T->elts[i].right = &(T->elts[i+1]);

  T->elts[taillemax - 1].right = NULL;
  T->libre = &(T->elts[0]);
  T->nil = &(T->elts[taillemax]);
  T->nil->left = T->nil->right = T->nil->parent = NULL;
  T->root = T->nil;

  return T;
} /* CreeRbtVide() */

/* ==================================== */
int32_t RbtVide(
  Rbt * T)
/* ==================================== */
{
  return (T->util == 0);
} /* RbtVide() */

/* ==================================== */
void RbtTermine(
  Rbt * T)
/* ==================================== */
{
#ifdef VERBOSE
  printf("Rbt: taux d'utilisation: %g\n", (double)T->maxutil / (double)T->max);
#endif
  free(T);
} /* RbtTermine() */

/* ==================================== */
RbtElt * RbtMinimum(
  Rbt * T, RbtElt * x)
/* ==================================== */
{
  while (x->left != T->nil) x = x->left;
  return x;
} /* RbtMinimum() */

/* ==================================== */
RbtElt * RbtSuccessor(
  Rbt * T, RbtElt * x)
/* ==================================== */
{
  RbtElt * y;
  if (x->right != T->nil) return RbtMinimum(T, x->right);
  y = x->parent;
  while ((y != T->nil) && (x == y->right))
  {
    x = y;
    y = y->parent;
  }
  return y;
} /* RbtSuccessor() */

/* ==================================== */
void RbtInsertSimple(
  Rbt * T, RbtElt * z)
/* ==================================== */
{
  RbtElt * x;
  RbtElt * y;

  y = T->nil;
  x = T->root;
  while (x != T->nil)
  {
    y = x;
    if ((z->key < x->key) || ((z->key == x->key) && (z->key2 < x->key2)))
      x = x->left; else x = x->right;
  }
  z->parent = y;
  if (y == T->nil)
    T->root = z;
  else
  {
    if ((z->key < y->key) || ((z->key == y->key) && (z->key2 < y->key2))) 
      y->left = z; else y->right = z;
  }
} /* RbtInsertSimple() */

/* ==================================== */
RbtElt * RbtInsertAux(  /* allocation et insertion simple */
  Rbt * T, TypRbtKey k, TypRbtKey k2, TypRbtAuxData d)
/* ==================================== */
{
  RbtElt * z;

  if (T->libre == NULL)
  {
    fprintf(stderr, "RbtInsertAux: Fatal error: Rbt full\n");
    exit(1);
  }

  T->util++;
  if (T->util > T->maxutil) T->maxutil = T->util;
  z = T->libre;
  T->libre = T->libre->right;
  z->key = k;
  z->key2 = k2;
  z->auxdata = d;
  z->left = T->nil;
  z->right = T->nil;
  RbtInsertSimple(T, z);
  return z;
} /* RbtInsertAux() */

/* ==================================== */
void LeftRotate(
  Rbt * T, RbtElt * x)
/* ==================================== */
{
  RbtElt * y;

  y = x->right;                    /* assume right(x) != NIL */
  x->right = y->left;              /* move y's child over */
  if (y->left != T->nil)
    y->left->parent = x;
  y->parent = x->parent;           /* move y up to x's position */
  if (x->parent == T->nil)
    T->root = y;
  else 
  {
    if (x == x->parent->left)
      x->parent->left = y;
    else x->parent->right = y;
  }
  y->left = x;                     /* move x down */
  x->parent = y;
} /* LeftRotate() */

/* ==================================== */
void RightRotate(
  Rbt * T, RbtElt * x)
/* ==================================== */
{
  RbtElt * y;

  y = x->left;              /* assume left(x) != NIL */
  x->left = y->right;
  if (y->right != T->nil)
    y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == T->nil)
    T->root = y;
  else 
  {
    if (x == x->parent->right)
       x->parent->right = y;
    else x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
} /* RightRotate() */

/* ==================================== */
RbtElt * RbtInsert(
  Rbt * T, TypRbtKey k, TypRbtKey2 k2, TypRbtAuxData d)
/* ==================================== */
{
  RbtElt * x;
  RbtElt * xcopy;
  RbtElt * uncle;

  xcopy = x = RbtInsertAux(T, k, k2, d);          /* allocation et insertion simple */
  x->color = Red;

  /* re-equilibrage de l'arbre */
  while ((x != T->root) && (x->parent->color == Red))
  {
    if (x->parent == x->parent->parent->left)
    {
      uncle = x->parent->parent->right;
      if (uncle->color == Red)
      {
        x->parent->color = Black;                    /* Case I */
        uncle->color = Black;
        x->parent->parent->color = Red;
        x = x->parent->parent;
      }
      else 
      {
        if (x == x->parent->right)
        {
          x = x->parent;                             /* Case II */
          LeftRotate(T,x);
        }
        x->parent->color = Black;                    /* Case III */
        x->parent->parent->color = Red;
        RightRotate(T, x->parent->parent);
      }
    }
    else /* same as "then" with "right" and "left" swapped */
    {
      uncle = x->parent->parent->left;
      if (uncle->color == Red)
      {
        x->parent->color = Black;                     /* Case I */
        uncle->color = Black;
        x->parent->parent->color = Red;
        x = x->parent->parent;
      }
      else 
      {
        if (x == x->parent->left)
        {
          x = x->parent;                             /* Case II */
          RightRotate(T,x);
        }
        x->parent->color = Black;                    /* Case III */
        x->parent->parent->color = Red;
        LeftRotate(T, x->parent->parent);
      }
    }
  } /* while */
  T->root->color = Black;
  return xcopy;
}

/* ==================================== */
void RbtDeleteFixup(
  Rbt * T, RbtElt * x)
/* ==================================== */
{
  RbtElt * s;

  while ((x != T->root) && (x->color == Black))
  {
    if (x == x->parent->left)
    {
      s = x->parent->right;               /* Get x's sibling */
      if (s->color == Red)
      {
        s->color = Black;              /* Case I */
        x->parent->color = Red;
        LeftRotate(T, x->parent);
        s = x->parent->right;
      }
      if ((s->left->color == Black) && (s->right->color == Black))
      {
        s->color = Red;                /* Case II */
        x = x->parent;
      }              
      else 
      {
        if (s->right->color == Black)
	{
          s->left->color = Black;      /* Case III */
          s->color = Red;                        
          RightRotate(T,s);
          s = x->parent->right;
        }
        s->color = x->parent->color;   /* Case IV */
        x->parent->color = Black;
        s->right->color = Black;
        LeftRotate(T, x->parent);                   
        x = T->root;
      }
    }
    else
    {            /* Same as "then" with right and left swapped */
      s = x->parent->left;               /* Get x's sibling */
      if (s->color == Red)
      {
        s->color = Black;              /* Case I */
        x->parent->color = Red;
        RightRotate(T, x->parent);
        s = x->parent->left;
      }
      if ((s->right->color == Black) && (s->left->color == Black))
      {
        s->color = Red;                /* Case II */
        x = x->parent;
      }              
      else 
      {
        if (s->left->color == Black)
	{
          s->right->color = Black;     /* Case III */
          s->color = Red;                        
          LeftRotate(T,s);
          s = x->parent->left;
        }
        s->color = x->parent->color;   /* Case IV */
        x->parent->color = Black;
        s->left->color = Black;
        RightRotate(T, x->parent);                   
        x = T->root;
      }
    }
  } /* while */
  x->color = Black;
} /* RbtDeleteFixup() */

/* ==================================== */
RbtElt * RbtDeleteAux(         /* return deleted node */
  Rbt * T, RbtElt * z, RbtElt **R)
/* ==================================== */
{
  RbtElt * c;
  RbtElt * d;

  if ((z->left == T->nil) || (z->right == T->nil))
    d = z;
  else 
    d = RbtSuccessor(T, z);
  if (d->left != T->nil)
    c = d->left;
  else 
    c = d->right;
  c->parent = d->parent;      /* no test for NIL with sentinel */
  if (d->parent == T->nil)
    T->root = c;
  else 
  {
    if (d == d->parent->left)
      d->parent->left = c;
    else 
      d->parent->right = c;
  }
  if (d != z)
  {
    int32_t x;
    z->key = d->key;
    z->key2 = d->key2;
    z->auxdata = d->auxdata;
    x = z->auxdata;               /* modif pour hthiniso */
    x = DECODEPIX(x);
    R[x] = z;
  }
  if (d->color == Black)
    RbtDeleteFixup(T, c);     /* c is now "Double-Black" */
  return d;
} /* RbtDeleteAux() */

/* ==================================== */
void RbtDelete(
  Rbt * T, RbtElt * z, RbtElt **R)
/* ==================================== */
{
  z = RbtDeleteAux(T, z, R);
  z->right = T->libre;
  T->libre = z;
  T->util -= 1;
} /* RbtDelete() */

#ifdef DEBUGRBT
/* ==================================== */
void RbtCheck(
  Rbt * T, RbtElt * x, RbtElt ** R, int32_t rs, int32_t N)
/* ==================================== */
{
  uint32_t y, k;
  if (x == T->nil) return;
  y = x->auxdata;
  k = DECODEDIR(y);
  if ((k < 0) || (k > 7)) 
  {
    printf("RbtCheck: fatal error: k = %d, y = %x\n", k, y);
    exit(1);
  }
  y = DECODEPIX(y);
  if (y >= N) 
  {
    printf("RbtCheck: fatal error\n");
    exit(1);
  }
  if (R[y] != x) printf("RbtCheck: R[(%d,%d)]=%x ; x=%x\n", y%rs, y/rs, x);
  RbtCheck(T, x->left, R, rs, N);
  RbtCheck(T, x->right, R, rs, N);
} /* RbtCheck() */
#endif

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/*
           AMINCISSEMENT AVEC REDUCTION D'ANISOTROPIE
*/
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/*
   y est un voisin de x.
   principe de l'encodage du couple (x,y): 
   le numero de voisin de y par rapport a x est stocke sur 3 bits, avec le codage suivant: 
		3	2	1			
		4	x	0
		5	6	7
   soit k.
   
   ce code est place dans les bits 29 a 31 de l'int32_t contenant y
                                                      29
   il reste donc 29 bits pour coder les pixels, soit 2   = 1/2 gigapixels.
*/

/* ==================================== */
double dist(double x1, double y1, double x2, double y2)
/* ==================================== */
{
  double dx = x2 - x1;
  double dy = y2 - y1;
  return sqrt(dx * dx + dy * dy);
}

/* ==================================== */
void UpdateRbt4alpha(uint32_t x, uint8_t *F, uint32_t *T, 
                int32_t rs, int32_t N, RbtElt **R, Rbt *RBT, double dmax)
/* ==================================== */
/*
   i) si besoin, retire x de RBT
   ii) teste si x est 4-destructible
   iii) si oui, calcule d=d(x,T(y)) (avec y = N(x)) et stocke le triple (x,y,d) dans RBT
*/
{
  uint32_t y, z, k, kk, alpha;
  double dz, dtmp;
  RbtElt *r;

  if (R[x]) 
  { 
#ifdef DEBUGISO
      printf("delete x=(%d,%d)\n", x%rs, x/rs);
#endif
    RbtDelete(RBT, R[x], R); 
    R[x] = NULL; 
  }

  if (pdestr4(F, x, rs, N))
  {
    alpha = alpha8m(F, x, rs, N);
    z = -1;                         /* position du voisin retenu */
    dz = (double)N;                 /* distance au bord du voisin retenu */
    for (k = 0; k < 8; k += 1)
    {
      y = voisin(x, k, rs, N);
      if ((y != -1) && (F[y] == alpha))
      {
        dtmp = dist(x%rs, x/rs, T[y]%rs, T[y]/rs);
        if ((dtmp <= dmax) && ((z == -1) || (dtmp < dz)))
        {
          z = y;
          kk = k;
          dz = dtmp;
	}
      } /* if ((y != -1) && (F[y] == alpha)) */
    } /* for (k = 0; k < 8; k += 1) */

    if (z != -1)
    {
#ifdef DEBUGISO
      printf("insere x=(%d,%d), z=(%d,%d), kk=%d, d=%g\n", x%rs, x/rs, z%rs, z/rs, kk, dz, r);
#endif
      z = ENCODEDIR(x,kk);
      r = RbtInsert(RBT, dz, F[x], z);
      R[x] = r;
    }
  } /* if (pdestr4(F, x, rs, N)) */
} /* UpdateRbt4alpha() */

/* ==================================== */
void UpdateRbt4(uint32_t x, uint8_t *F, uint32_t *T, 
                int32_t rs, int32_t N, RbtElt **R, Rbt *RBT, double dmax)
/* ==================================== */
/*
   i) si besoin, retire x de RBT
   ii) teste si x est 4-destructible
   iii) si oui, calcule d=d(x,T(y)) (avec y = N(x)) et stocke le triple (x,y,d) dans RBT
*/
{
  uint32_t y, z, k, kk, delta;
  double dz, dtmp;
  uint8_t f;
  RbtElt *r;

  if (R[x]) 
  { 
#ifdef DEBUGISO
      printf("delete x=(%d,%d)\n", x%rs, x/rs);
#endif
    RbtDelete(RBT, R[x], R); 
    R[x] = NULL; 
  }

  if (pdestr4(F, x, rs, N))
  {
#ifdef DEBUGISO
    printf("x=(%d,%d) destructible ; F[x]=%d\n", x%rs, x/rs, F[x]);
#endif
    delta = delta4m(F, x, rs, N);
    z = -1;                         /* position du voisin retenu */
    dz = (double)N;                 /* distance au bord du voisin retenu */
    f = NDG_MAX;                    /* niveau de gris du voisin retenu */
    for (k = 0; k < 8; k += 1)
    {
      y = voisin(x, k, rs, N);
      if ((y != -1) && (F[y] < F[x]) && (F[y] >= delta))
      {
        dtmp = dist(x%rs, x/rs, T[y]%rs, T[y]/rs);

        if ((dtmp <= dmax) && ((z == -1) || (F[y] < f) || ((F[y] == f) && (dtmp < dz))))
	/*
           REMARQUE: ce n'est pas tout a fait le critere du papier. 
           Ici, on accepte un point qui n'est pas a la valeur delta- si
           aucun point a cette valeur ne satisfait le critere de distance
           (priorite est donnee au niveau de gris).
	 */
        {
          f = F[y];
          z = y;
          kk = k;
          dz = dtmp;
	}
      } /* if ((y != -1) && (F[y] == delta)) */
    } /* for (k = 0; k < 8; k += 1) */

    if (z != -1)
    {
#ifdef DEBUGISO
      printf("insere x=(%d,%d), z=(%d,%d), kk=%d, d=%g\n", x%rs, x/rs, z%rs, z/rs, kk, dz, r);
#endif
      z = ENCODEDIR(x,kk);
      r = RbtInsert(RBT, dz, F[x], z);
      R[x] = r;
    }
  } /* if (pdestr4(F, x, rs, N)) */
} /* UpdateRbt4delta() */

/* ==================================== */
int32_t lhthiniso(struct xvimage *image, double dmax, int32_t connex, double pixwhratio)
/* ==================================== */
{ 
  int32_t i;                       /* index muet */
  uint32_t x;                       /* index muet de pixel */
  uint32_t y;                       /* index muet (generalement un voisin de x) */
  uint32_t z;                       /* index muet (generalement un voisin de y) */
  uint32_t k;                       /* index muet */
  int32_t rs = rowsize(image);     /* taille ligne */
  int32_t cs = colsize(image);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint32_t *T;            /* les pointeurs "origine de la valeur" */
  int32_t incr_vois;
  Rbt * RBT;                   /* structure d'arbre equilibre (rouge et noir) */
  RbtElt ** R;                 /* pointeurs sur les noeuds de l'arbre rouge et noir */
  RbtElt * r;                  /* pointeur sur un noeud de l'arbre rouge et noir */

  if (connex == 4) incr_vois = 2; else incr_vois = 1;

  if (depth(image) != 1) 
  {
    fprintf(stderr, "lhthiniso: cette version ne traite pas les images volumiques\n");
    return(0);
  }

  T = (uint32_t *)calloc(1,N * sizeof(int32_t));
  if (T == NULL) 
  {
    fprintf(stderr, "lhthiniso: malloc failed\n");
    return(0);
  }

  R = (RbtElt **)calloc(N, sizeof(RbtElt *));
  if (R == NULL) 
  {
    fprintf(stderr, "lhthiniso: calloc failed\n");
    return(0);
  }

  RBT = CreeRbtVide(N);
  if (RBT == NULL)
  {   fprintf(stderr, "lhthiniso() : CreeRbtVide failed\n");
      return(0);
  }

  /* ================================================ */
  /*                  DEBUT ALGO                      */
  /* ================================================ */

  for (x = 0; x < N; x++) T[x] = x; 

  /* empile tous les points destructibles */
  if (connex == 4)
  {
      for (x = 0; x < N; x++) UpdateRbt4(x, F, T, rs, N, R, RBT, dmax);
  }

  /* ================================================ */
  /*                  DEBUT ITERATIONS                */
  /* ================================================ */

  if (connex == 4)
  {
    while (!RbtVide(RBT))
    {
      r = RbtMinimum(RBT, RBT->root);
      x = r->auxdata;
      k = DECODEDIR(x);
      x = DECODEPIX(x);
      y = voisin(x, k, rs, N);
#ifdef DEBUGISO
      printf("extrait x=(%d,%d), y=(%d,%d), k=%d, key=%g\n", 
              x%rs, x/rs, y%rs, y/rs, k, r->key);
#endif

      /* y est le point qui doit donner sa valeur a x */
#ifdef PARANO
      if (R[x] == NULL)
      {
        fprintf(stderr, "lhthiniso: erreur inattendue\n");
        exit(1);
      } 
      if (!pdestr4(F, x, rs, N))
      {
        fprintf(stderr, "lhthiniso: erreur: x n'est plus destructible !\n");
      }
      if (F[y] >= F[x])
      {
        fprintf(stderr, "lhthiniso: erreur: F[y]=%d >= F[x]=%d\n", F[y], F[x]);
      }
#endif
#ifdef DEBUGISO
      printf("delete x=(%d,%d)\n", x%rs, x/rs);
      printf("abaisse x=(%d,%d), %d -> %d\n", 
              x%rs, x/rs, F[x], F[y]);
#endif
      RbtDelete(RBT, R[x], R);
      R[x] = NULL;
      F[x] = F[y];
      T[x] = T[y];
      UpdateRbt4(x, F, T, rs, N, R, RBT, dmax);
      for (k = 0; k < 8; k += 1)
      {
        y = voisin(x, k, rs, N);
        if (y != -1) UpdateRbt4(y, F, T, rs, N, R, RBT, dmax);
      } /* for k */
    } /* while (!RbtVide(RBT)) */
  } /* if (connex == 4) */

  /* ================================================ */
  /* UN PEU DE MENAGE                                 */
  /* ================================================ */

  RbtTermine(RBT);
  free(T);
  free(R);
  return(1);
} /* lhthiniso() */
