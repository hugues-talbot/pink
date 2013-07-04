/* $Id: mcfahdouble.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

typedef struct FAHDELT {
  int32_t Point1;
  int32_t Point2;
  struct FAHDELT * Next;
} FahdElt;

#define NPRIO 256

typedef struct {
  int32_t Max;             /* taille max de la fahd (en nombre de points) */
  int32_t Niv;             /* niveau a partir duquel des listes existent */
  int32_t Util;            /* nombre de points courant dans la fahd */
  int32_t Maxutil;         /* nombre de points utilises max */
  FahdElt *Tete[NPRIO]; /* tableau des tetes de liste (la ou l'on insere) */
  FahdElt *Queue;       /* queue de liste (la ou l'on preleve) */
  FahdElt *Libre;       /* pile des cellules libres */
  FahdElt Elts[1];      /* tableau des elements physiques */
} Fahd;

/* ============== */
/* prototypes     */
/* ============== */

extern Fahd * CreeFahdVide(
  int32_t taillemax
);

extern void FahdFlush(
  Fahd * L
);

extern int32_t FahdVide(
  Fahd * L
);

extern int32_t FahdVideUrg(
  Fahd * L
);

extern int32_t FahdVideNiveau(
  Fahd * L,
  int32_t niv
);

extern int32_t FahdNiveau(
  Fahd * L
);

extern void FahdPop(
  Fahd * L,
  int32_t *P1,
  int32_t *P2
);

extern void FahdFirst(
  Fahd * L,
  int32_t *P1,
  int32_t *P2
);

extern void FahdPush(
  Fahd * L,
  int32_t P1,
  int32_t P2,
  int32_t Ni
);

extern void FahdTermine(
  Fahd * L
);

extern void FahdPrint(
  Fahd * L
);
