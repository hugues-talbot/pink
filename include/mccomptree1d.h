/* $Id: mccomptree1d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============================================================================== */
/* 
  Structure de donnees pour la construction de l'arbre des composantes 1d.

  Les sommets de cet arbre representent les composantes des coupes de F,  
  a l'exception de celles qui sont egales a une composante d'un niveau inferieur.
  Il y a donc moins de N sommets (N = nombre de pixels) et de N-1 arcs.

  Une composante (sommet) est representee par une structure ctree1dnode.
*/
/* ============================================================================== */

typedef struct
{
  int32_t level;   // node's level
  int32_t father;  // value -1 indicates the root
  int32_t begin;
  int32_t end;
  int32_t area;
  int32_t height;
  int32_t volume;
} ctree1dnode;

typedef struct
{
  int32_t nbnodes;
  int32_t nbmaxnodes;
  int32_t root;
  ctree1dnode * tabnodes;
} ctree1d;

/* ==================================== */
/* PROTOTYPES */
/* ==================================== */
extern ctree1d * ComponentTree1dAlloc(int32_t N);
extern void ComponentTree1dFree(ctree1d * CT);
extern void ComponentTree1d( uint8_t *F, int32_t N, // inputs
                           ctree1d **CompTree, // output
                           int32_t **CompMap       // output
			 );
#ifdef __cplusplus
}
#endif
