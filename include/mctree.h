/* $Id: mctree.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* definitions pour le module "mctree.c" */

#define TREEPAGECELLS 1024

typedef int32_t TypKey;

typedef struct TREECELL {
  TypKey key;
  struct TREECELL * father;
  uint8_t prof;
} TreeCell;

typedef struct TREEPAGE {
  TreeCell tab[TREEPAGECELLS];
  int32_t lastfreecell;
  struct TREEPAGE *nextpage;
} TreePage;

extern void TreeInit();
extern void TreeTermine();
extern TreeCell * AllocTreeCell();
#ifdef __cplusplus
}
#endif
