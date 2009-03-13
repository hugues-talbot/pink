/* $Id: mcfahpure.h,v 1.3 2009-03-13 14:46:14 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct FAHPELT {
  int32_t Point;
  struct FAHPELT * Next;
} FahpElt;

#define FAHP_NPRIO 256*2

typedef struct {
  int32_t Max;             /* taille max de la fah (en nombre de points) */
  int32_t Niv;             /* niveau a partir duquel des listes existent */
  int32_t Util;            /* nombre de points courant dans la fah */
  int32_t Maxutil;         /* nombre de points utilises max (au cours du temps) */
  FahpElt *Tete[FAHP_NPRIO]; /* tableau des tetes de liste (la ou l'on insere) */
  FahpElt *Queue;       /* queue de liste (la ou l'on preleve) */
  FahpElt *Libre;       /* pile des cellules libres */
  FahpElt Elts[1];      /* tableau des elements physiques */
} Fahp;

/* ============== */
/* prototypes     */
/* ============== */

extern Fahp * CreeFahpVide(
  int32_t taillemax
);

extern void FahpFlush(
  Fahp * L
);

extern int32_t FahpVide(
  Fahp * L
);

extern int32_t FahpVideUrg(
  Fahp * L
);

extern int32_t FahpVideNiveau(
  Fahp * L,
  int32_t niv
);

extern int32_t FahpNiveau(
  Fahp * L
);

extern int32_t FahpPop(
  Fahp * L
);

extern int32_t FahpFirst(
  Fahp * L
);

extern void FahpPush(
  Fahp * L,
  int32_t Po,
  int32_t Ni
);

extern void FahpTermine(
  Fahp * L
);

extern void FahpPrint(
  Fahp * L
);
#ifdef __cplusplus
}
#endif
