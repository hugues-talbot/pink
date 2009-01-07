/* $Id: mcliste.h,v 1.3 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _mcliste_h
typedef struct {
  int32_t Max;          /* taille max de la Liste */
  int32_t Sp;           /* index de pile (pointe la 1ere case libre) */
  int32_t Pts[1];
} Liste;

/* prototypes     */
extern Liste * CreeListeVide(int32_t taillemax);
extern void ListeFlush(Liste * L);
extern int32_t ListeTaille(Liste * L);
extern int32_t ListeVide(Liste * L);
extern int32_t ListeElt(Liste * L, uint32_t n);
extern int32_t ListePop(Liste * L);
extern int32_t ListePush(Liste * L, int32_t V);
extern int32_t ListeIn(Liste * L, int32_t e);
extern void ListePrint(Liste * L);
extern void ListePrintLine(Liste * L);
extern void ListeTermine(Liste * L);
#endif
#define _mcliste_h
#ifdef __cplusplus
}
#endif
