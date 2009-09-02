/* $Id: mcrlifo.h,v 1.1 2009-09-02 14:23:35 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int32_t Max;          /* taille max de la Rlifo */
  int32_t Sp;           /* index de pile (pointe la 1ere case libre) */
  int32_t Pts[1];
} Rlifo;

/* ============== */
/* prototypes     */
/* ============== */
extern Rlifo * CreeRlifoVide(int32_t taillemax);
extern void RlifoFlush(Rlifo * L);
extern int32_t RlifoVide(Rlifo * L);
extern int32_t RlifoPop(Rlifo * L);
extern int32_t RlifoHead(Rlifo * L);
extern void RlifoPush(Rlifo ** L, int32_t V);
extern void RlifoPrint(Rlifo * L);
extern void RlifoPrintLine(Rlifo * L);
extern void RlifoTermine(Rlifo * L);
#ifdef __cplusplus
}
#endif
