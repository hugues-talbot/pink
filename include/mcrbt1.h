/* $Id: mcrbt1.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define RBT_Black 0
#define RBT_Red   1

typedef struct {
  double x;
  double y;
  double z;
} TypRbtKey;
typedef int32_t TypRbtAuxData;

#define EQUALKEY(j,k) ((j.x==k.x)&&(j.y==k.y)&&(j.z==k.z))
#define LESSKEY(j,k) ((j.x<k.x) || ((j.x==k.x)&&(j.y<k.y)) || ((j.x==k.x)&&(j.y==k.y)&&(j.z<k.z)))
#define COPYKEY(j,k) {j.x=k.x;j.y=k.y;j.z=k.z;}
#define PRINTKEY(j) printf("%g %g %g",j.x,j.y,j.z)

typedef struct RBTELT {
  TypRbtAuxData auxdata;
  TypRbtKey key;
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

/* ============== */
/* prototypes     */
/* ============== */

extern Rbt * CreeRbtVide(
  int32_t taillemax);

extern void RbtFlush(
  Rbt * T);

extern int32_t RbtVide(
  Rbt * T);

extern void RbtTermine(
  Rbt * T);

extern void RbtPrint(
  Rbt * T);

extern RbtElt * RbtSearch(
  Rbt * T, TypRbtKey k);

extern RbtElt * RbtMinimum(
  Rbt * T, RbtElt * x);

extern RbtElt * RbtMaximum(
  Rbt * T, RbtElt * x);

extern RbtElt * RbtSuccessor(
  Rbt * T, RbtElt * x);

extern RbtElt * RbtInsert(
  Rbt ** T, TypRbtKey k, TypRbtAuxData d);

extern void RbtDelete(
  Rbt * T, RbtElt * z);

extern TypRbtAuxData RbtPopMin(
  Rbt * T);

extern TypRbtAuxData RbtPopMax(
  Rbt * T);

extern TypRbtKey RbtMinLevel(
  Rbt * T);
