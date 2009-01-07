/* $Id: mclifo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int32_t Max;          /* taille max de la Lifo */
  int32_t Sp;           /* index de pile (pointe la 1ere case libre) */
  int32_t Pts[1];
} Lifo;

/* ============== */
/* prototypes     */
/* ============== */

extern Lifo * CreeLifoVide(
  int32_t taillemax
);

extern void LifoFlush(
  Lifo * L
);

extern int32_t LifoVide(
  Lifo * L
);

extern int32_t LifoPop(
  Lifo * L
);

extern int32_t LifoHead(
  Lifo * L
);

extern void LifoPush(
  Lifo * L,
  int32_t V
);

extern void LifoPrint(
  Lifo * L
);

extern void LifoPrintLine(
  Lifo * L
);

extern void LifoTermine(
  Lifo * L
);
#ifdef __cplusplus
}
#endif
