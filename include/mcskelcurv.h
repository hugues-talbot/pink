/* $Id: mcskelcurv.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

/* ================================================ */
/* types publics */
/* ================================================ */

/* structure "classique" de liste */

typedef struct SKC_cell {
  int32_t val;
  struct SKC_cell * next;
} SKC_cell;

typedef SKC_cell * SKC_pcell;

/*
Un squelette S est représenté par un graphe comportant 4 types de sommets : 
- les isolés
- les extrémités
- les courbes
- les jonctions
liés par des arcs qui représentent les relations d'adjacence.

Une courbe de S est composée de points qui ont 1 ou 2 voisins dans S

Tous les sommets du graphe sont représentés par la structure suivante :
*/

typedef struct {
  float fval;
  struct SKC_cell * pts; // liste des points constituant l'élément de squelette
  struct SKC_cell * adj; // liste des éléments adjacents
} skelpart;

typedef skelpart * pskelpart;

/*
La structure pour représenter un squelette :
*/

typedef struct {
  int32_t connex;
  uint32_t rs, cs, ds;
  uint32_t e_isol;
  uint32_t e_end;
  uint32_t e_curv;
  uint32_t e_junc;
  uint32_t nbcell;
  uint32_t freecell;      // pour la gestion du tas de cellules
  skelpart * tskel;    // tableau des éléments de squelette (tas)
  SKC_cell * tcell;        // tableau des cellules (tas)
} skel;

#define IS_ISOL(x) (x<S->e_isol)
#define IS_END(x) ((x>=S->e_isol)&&(x<S->e_end))
#define IS_CURV(x) ((x>=S->e_end)&&(x<S->e_curv))
#define IS_JUNC(x) (x>=S->e_curv)

/* ================================================ */
/* prototypes */
/* ================================================ */

extern skel * initskel(uint32_t rs, uint32_t cs, uint32_t ds, 
  uint32_t nbvertex, uint32_t nbcell, int32_t connex);
extern void termineskel(skel * S);
extern SKC_pcell skeladdcell(skel *S, int32_t val, SKC_pcell next);
extern void addadjlist(skel * S, uint32_t k, uint32_t v);
extern void addptslist(skel * S, uint32_t k, uint32_t v);
extern skel * readskel(char *filename);
extern void writeskel(skel * S, char *filename);
extern void writevskel(skel * S, char *filename, struct xvimage *val);

#ifdef __cplusplus
}
#endif
