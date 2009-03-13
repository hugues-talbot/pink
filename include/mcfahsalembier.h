/* $Id: mcfahsalembier.h,v 1.3 2009-03-13 14:46:14 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FAHSELT {
  int32_t Point;
  struct FAHSELT * Next;
  struct FAHSELT * Prev;
} FahsElt;

//#define FAHS_NPRIO 256
#define FAHS_NPRIO 65536

typedef struct {
  int32_t Max;             /* taille max de la fah (en nombre de points) */
  int32_t Niv;             /* niveau a partir duquel des listes existent */
  int32_t Util;            /* nombre de points courant dans la fah */
  int32_t Maxutil;         /* nombre de points utilises max (au cours du temps) */
  FahsElt *Tete[FAHS_NPRIO]; /* tableau des tetes de liste (la ou l'on insere) */
  FahsElt *Queue[FAHS_NPRIO];/* tableau des queues de liste (la ou l'on preleve) */
  FahsElt *Libre;       /* pile des cellules libres */
  FahsElt Elts[1];      /* tableau des elements physiques */
} Fahs;

/* ============== */
/* prototypes     */
/* ============== */

extern Fahs * CreeFahsVide(
  int32_t taillemax
);

extern void FahsFlush(
  Fahs * L
);

extern int32_t FahsVide(
  Fahs * L
);

extern int32_t FahsVideNiveau(
  Fahs * L,
  int32_t niv
);

extern int32_t FahsPop(
  Fahs * L
);

extern int32_t FahsPopNiveau(
  Fahs * L,
  int32_t niv
);

extern void FahsPush(
  Fahs * L,
  int32_t Po,
  int32_t Ni
);

extern void FahsTermine(
  Fahs * L
);

extern void FahsPrint(
  Fahs * L
);
#ifdef __cplusplus
}
#endif
