/* $Id: mcfahsalembier.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FAHELT {
  int32_t Point;
  struct FAHELT * Next;
  struct FAHELT * Prev;
} FahElt;

//#define NPRIO 256
#define NPRIO 65536

typedef struct {
  int32_t Max;             /* taille max de la fah (en nombre de points) */
  int32_t Niv;             /* niveau a partir duquel des listes existent */
  int32_t Util;            /* nombre de points courant dans la fah */
  int32_t Maxutil;         /* nombre de points utilises max (au cours du temps) */
  FahElt *Tete[NPRIO]; /* tableau des tetes de liste (la ou l'on insere) */
  FahElt *Queue[NPRIO];/* tableau des queues de liste (la ou l'on preleve) */
  FahElt *Libre;       /* pile des cellules libres */
  FahElt Elts[1];      /* tableau des elements physiques */
} Fah;

/* ============== */
/* prototypes     */
/* ============== */

extern Fah * CreeFahVide(
  int32_t taillemax
);

extern void FahFlush(
  Fah * L
);

extern int32_t FahVide(
  Fah * L
);

extern int32_t FahVideNiveau(
  Fah * L,
  int32_t niv
);

extern int32_t FahPop(
  Fah * L
);

extern int32_t FahPopNiveau(
  Fah * L,
  int32_t niv
);

extern void FahPush(
  Fah * L,
  int32_t Po,
  int32_t Ni
);

extern void FahTermine(
  Fah * L
);

extern void FahPrint(
  Fah * L
);
#ifdef __cplusplus
}
#endif
