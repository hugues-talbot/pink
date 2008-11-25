/* $Id: mctree.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
