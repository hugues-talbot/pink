/* $Id: mcunionfind.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* types publics  */
/* ============== */

typedef struct {
  int32_t Size;
  int32_t *Fth;
  int32_t *Rank;
} Tarjan;

/* ============== */
/* prototypes     */
/* ============== */

extern Tarjan * CreeTarjan(int32_t taille);
extern void TarjanTermine(Tarjan * T);
extern void TarjanInit(Tarjan * T);
extern void TarjanPrint(Tarjan * T);
extern void TarjanMakeSet(Tarjan * T, int32_t x);
extern int32_t TarjanFind(Tarjan * T, int32_t x);
extern int32_t TarjanLink(Tarjan * T, int32_t x, int32_t y);
extern int32_t TarjanLinkSafe(Tarjan * T, int32_t x, int32_t y);

#ifdef __cplusplus
}
#endif
