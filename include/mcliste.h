/* $Id: mcliste.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#ifndef _mcliste_h
typedef struct {
  int32_t Max;          /* taille max de la Liste */
  int32_t Sp;           /* index de pile (pointe la 1ere case libre) */
  int32_t Pts[1];
} Liste;

/* prototypes     */
extern Liste * CreeListeVide(int32_t taillemax);
extern void ListeFlush(Liste * L);
extern int32_t ListeVide(Liste * L);
extern int32_t ListePop(Liste * L);
extern void ListePush(Liste * L, int32_t V);
extern int32_t ListeIn(Liste * L, int32_t e);
extern void ListePrint(Liste * L);
extern void ListePrintLine(Liste * L);
extern void ListeTermine(Liste * L);
#endif
#define _mcliste_h
