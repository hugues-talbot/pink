/* $Id: mctree.c,v 1.1.1.1 2008-11-25 08:01:43 mcouprie Exp $ */
/* structure d'arbre */

/* #define TESTTREE */
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <mctree.h>

/* var. globales (privees) pour les pages de cellules */
static TreePage * FirstTreePage;
static TreePage * CurTreePage;

/* ==================================== */
void TreeInit()
/* ==================================== */
{
  FirstTreePage = (TreePage *)calloc(1,sizeof(TreePage));
  if (FirstTreePage == NULL)
  {
     fprintf(stderr, "InitTree : malloc failed\n");
     exit(0);
  }
  FirstTreePage->lastfreecell = 0;
  FirstTreePage->nextpage = NULL;
  CurTreePage = FirstTreePage;
} /* TreeInit() */

/* ==================================== */
void TreeTermine()
/* ==================================== */
{
  TreePage *t = FirstTreePage;
  TreePage *tp;

  while (t)
  {
    tp = t->nextpage;
    free(t);
    t = tp;
  }
} /* TreeTermine() */

/* ==================================== */
TreeCell * AllocTreeCell()
/* ==================================== */
{
  TreeCell *c;
  if (CurTreePage->lastfreecell >= TREEPAGECELLS)
  {
    TreePage *t = (TreePage *)calloc(1,sizeof(TreePage));
    if (t == NULL)
    {
      fprintf(stderr, "AllocTreeCell : malloc failed\n");
      exit(0);
    }
    t->lastfreecell = 0;
    t->nextpage = NULL;
    CurTreePage->nextpage = t;
    CurTreePage = t;
  }
  c = &(CurTreePage->tab[CurTreePage->lastfreecell]);
  (CurTreePage->lastfreecell)++;
  return c;
} /* AllocTreeCell() */
