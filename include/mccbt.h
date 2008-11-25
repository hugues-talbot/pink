/* $Id: mccbt.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define NIL -1

typedef struct
{
  uint8_t data;    /* pour le niveau de la coupe */
  uint8_t label;   /* pour l'algo lca */
  int32_t father;
  int32_t cbtaux;          
} cbtcell;

#define ancestor cbtaux     /* utile pour la construction seulement */
#define firstson cbtaux     /* utile pour l'arbre inverse seulement */

#define Data(tab,c) (tab[c].data)
#define Father(tab,c) (tab[c].father)
#define Label(tab,c) (tab[c].label)
#define FirstSon(tab,c) (tab[c].cbtaux)
#define Aux(tab,c) (tab[c].cbtaux)

/* ============== */
/* prototypes     */
/* ============== */

extern void initcbt(
  cbtcell * arbre,
  int32_t nbcell
);

extern int32_t CreateCell(
  cbtcell * arbre,
  int32_t * nbcell,
  int32_t nbmaxcell
);

extern void SetFather(
  cbtcell * arbre,
  int32_t cell,
  int32_t f
);

extern void SetData(
  cbtcell * arbre,
  int32_t cell,
  int32_t d
);

extern void PrintCbt(
  cbtcell * arbre,
  int32_t nbcell
);

extern int32_t Anc(
  cbtcell * arbre,
  int32_t cell,
  int32_t d
);

extern int32_t DiffAnc(
  cbtcell * arbre,
  int32_t argc,
  int32_t *argv
);

extern int32_t LowComAnc(
  cbtcell * arbre,
  int32_t argc,
  int32_t *argv,
  uint8_t d
);

extern int32_t IsAnc(
  cbtcell * arbre,
  int32_t a,
  int32_t x
);

extern int32_t Euthanasie(
  cbtcell * arbre,
  int32_t argc,
  int32_t *argv
);

extern int32_t * Regularise(
  cbtcell * arbre,
  int32_t nbleaf,
  int32_t nbcell
);

extern int32_t * InverseCBT(
  cbtcell * arbre,
  int32_t nbleaf,
  int32_t nbcell
);
