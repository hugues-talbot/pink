/*
Copyright ESIEE (2009) 

m.couprie@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/
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
  int32_t connex;      // connexité pour l'objet 
  uint32_t rs, cs, ds; // dimensions image
  uint32_t e_isol;     // fin des index "points isolés" 
  uint32_t e_end;      // fin des index "points extrémités" 
  uint32_t e_curv;     // fin des index "points de courbe" 
  uint32_t e_junc;     // fin des index "points de jonction" 
  uint32_t nbcell;     // nombre total de cellules de liste
  uint32_t freecell;   // pour la gestion du tas de cellules
  skelpart * tskel;    // tableau des éléments de squelette (tas)
  SKC_cell * tcell;    // tableau des cellules (tas)
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
