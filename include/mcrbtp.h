/* $Id: mcrbtp.h,v 1.1 2009-01-06 13:18:06 mcouprie Exp $ */
#define RBTP_Black 0
#define RBTP_Red   1

typedef struct {
  double x;
  double y;
  double z;
} TypRbtpKey;
typedef int32_t TypRbtpAuxData;

#define EQUALKEY(j,k) ((j.x==k.x)&&(j.y==k.y)&&(j.z==k.z))
#define LESSKEY(j,k) ((j.x<k.x) || ((j.x==k.x)&&(j.y<k.y)) || ((j.x==k.x)&&(j.y==k.y)&&(j.z<k.z)))
#define COPYKEY(j,k) {j.x=k.x;j.y=k.y;j.z=k.z;}
#define PRINTKEY(j) printf("%g %g %g",j.x,j.y,j.z)

typedef struct RBTPELT {
  TypRbtpAuxData auxdata;
  TypRbtpKey key;
  char color;
  struct RBTPELT * left;
  struct RBTPELT * right;
  struct RBTPELT * parent;
} RbtpElt;

typedef struct {
  int32_t max;             /* taille max du rbtp (en nombre de points) */
  int32_t util;            /* nombre de points courant dans le rbtp */
  int32_t maxutil;         /* nombre de points utilises max (au cours du temps) */
  RbtpElt *root;        /* racine de l'arbre */
  RbtpElt *nil;         /* sentinelle et element dont l'adresse joue le role de NIL */
  RbtpElt *libre;       /* pile des cellules libres */
  RbtpElt elts[1];      /* tableau des elements physiques */
} Rbtp;

/* ============== */
/* prototypes     */
/* ============== */

extern Rbtp * CreeRbtpVide(
  int32_t taillemax);

extern void RbtpFlush(
  Rbtp * T);

extern int32_t RbtpVide(
  Rbtp * T);

extern void RbtpTermine(
  Rbtp * T);

extern void RbtpPrint(
  Rbtp * T);

extern RbtpElt * RbtpSearch(
  Rbtp * T, TypRbtpKey k);

extern RbtpElt * RbtpMinimum(
  Rbtp * T, RbtpElt * x);

extern RbtpElt * RbtpMaximum(
  Rbtp * T, RbtpElt * x);

extern RbtpElt * RbtpSuccessor(
  Rbtp * T, RbtpElt * x);

extern RbtpElt * RbtpInsert(
  Rbtp ** T, TypRbtpKey k, TypRbtpAuxData d);

extern void RbtpDelete(
  Rbtp * T, RbtpElt * z);

extern TypRbtpAuxData RbtpPopMin(
  Rbtp * T);

extern TypRbtpAuxData RbtpPopMax(
  Rbtp * T);

extern TypRbtpKey RbtpMinLevel(
  Rbtp * T);
