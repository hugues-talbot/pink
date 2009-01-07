/* $Id: jccomptree.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============================================================================== */
/* 
  Structure de donnees pour la construction de l'arbre des composantes.

  Les sommets de cet arbre representent les composantes des coupes de F,  
  a l'exception de celles qui sont egales a une composante d'un niveau inferieur.
  Il y a donc moins de N sommets (N = nombre de pixels) et de N-1 arcs.

  Une composante (sommet) est representee par une structure ctreenode.
  +
  arbre des composantes d'un graphe d'adjacence valué
  +
  arbre de fusion d'un graphe d'adjacence valué 
  +
  un arbre de saillance
*/
/* ============================================================================== */

typedef struct soncell
{
  int32_t son;
  struct soncell *next;
} soncell;

typedef struct
{
  uint8_t data;             // node's level
  int32_t father;            // value -1 indicates the root
  int32_t nbsons;            // value -1 indicates a deleted node
  soncell *sonlist;
  soncell *lastson;
}ctreenode;

typedef struct
{
  int32_t nbnodes;
  int32_t nbsoncells;
  int32_t root;
  ctreenode * tabnodes; 
  soncell * tabsoncells;
  uint8_t *flags;
} ctree;

typedef struct
{
  ctree *CT;
  int32_t *mergeEdge;
} mtree;

/* ==================================== */
/* PROTOTYPES */
/* ==================================== */
extern ctree * componentTreeAlloc(int32_t N);
extern void componentTreeFree(ctree * CT);
extern int32_t ComponentTreeGA( uint8_t *F, int32_t rs, int32_t N, ctree **CompTree, int32_t **CompMap);
extern int32_t ** LCApreprocess(ctree *CT, int32_t *Euler, int32_t *Depth, int32_t *Represent, int32_t *Number, int32_t *nbR, int32_t *lognR);
extern int32_t LowComAncFast(int32_t n1, int32_t n2, int32_t *Euler, int32_t *Number, int32_t *Depth, int32_t **Minim);
extern int32_t LowComAncSlow(ctree * CT, int32_t c1, int32_t c2);
extern void mergeTreePrint(mtree * MT);
extern mtree * mergeTreeAlloc(int32_t N);
extern void mergeTreeFree(mtree * MT);
extern int32_t mergeTree(RAG *rag, mtree **MergeTree);
// Ces 3 fontions vont plutot dans la biblio hierarchie
int32_t jcSaliencyTree_b (ctree ** SaliencyTree, int32_t *MST, int32_t *Valeur, RAG *rag, int32_t *STaltitude);


#ifdef __cplusplus
}
#endif
