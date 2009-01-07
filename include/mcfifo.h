/* $Id: mcfifo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
  int32_t Max;          /* taille max de la fifo */
  int32_t In;           /* index ou ranger le prochain arrivant */
  int32_t Out;          /* index d'ou retirer le prochain sortant */
  int32_t Pts[1];
} Fifo;

/* ============== */
/* prototypes     */
/* ============== */

extern Fifo * CreeFifoVide(int32_t taillemax);
extern void FifoFlush(Fifo * L);
extern int32_t FifoVide(Fifo * L);
extern int32_t FifoPop(Fifo * L);
extern void FifoPush(Fifo * L, int32_t V);
extern void FifoPushFirst(Fifo * L, int32_t V);
extern int32_t FifoTaille(Fifo * L);
extern void FifoPrint(Fifo * L);
extern void FifoTermine(Fifo * L);



#ifdef __cplusplus
}
#endif
