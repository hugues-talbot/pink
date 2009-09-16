/* $Id: jcgraphes.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

// Structure de Graphe binaire

typedef struct BasicCell {
  // index du sommet extremite de l'arete
  uint32_t vertex;
  // index de l'arete
  uint32_t edge;
  struct BasicCell * next;
} BasicCell;

typedef BasicCell * PBasicCell;

typedef struct GrapheBasic {
  /* informations globales */
  //!  nombre de sommets 
  int32_t nsom;
  //!  nombre maximum d'arcs
  int32_t nmaxarc;
  //!  nombre d'arcs
  int32_t narc; // Ah les hauts fonctionnaires !!
  /* representation par listes chainees de successeurs (application gamma) */
  //!  tableau des cellules en réserve 
  PBasicCell reserve;
  //!  liste des cellules libres gérée en pile lifo 
  PBasicCell libre;
  //!  tableau des listes de successeurs indexé par les sommets 
  PBasicCell *gamma;     
} GrapheBasic; 

// Manipulation ces graphes binaires
GrapheBasic *initGrapheBasic(int32_t nsom, int32_t nmaxarc);
void termineGrapheBasic(GrapheBasic *g);
uint32_t ajouteGrapheBasicSymArc(GrapheBasic *g, int32_t i, int32_t s);
PBasicCell alloueBasicCell(PBasicCell *plibre);

// Structure pour des graphes a aretes valuees
typedef struct GrapheValue {
  GrapheBasic *g;
  uint8_t *F;
} GrapheValue;

// Manipulations des graphes values
GrapheValue *initGrapheValue(int32_t nsom, int32_t nmaxarc);
void termineGrapheValue(GrapheValue * gv);
// add a weighted edge if necessary, otherwise update the value of the
// edge from i to s
int32_t updateArcValue(GrapheValue *gv, int32_t i, int32_t s, uint8_t val);

// Graphe d'adjacence (graphes values contenant de l'information sur les sommets)
typedef struct RAG{
  GrapheBasic *g;           // la structure binaire
  uint8_t *F;              // valuation des aretes
  uint8_t *profondeur;     // profondeur des regions
  uint32_t *surface;       //
  uint32_t *altitude;       //
  uint32_t *tete;          // representation du graphe
  uint32_t *queue;         // par liste d'aretes
} RAG;

// Manipulation des RAGs
extern RAG *initRAG(int32_t nsom, int32_t nmaxarc);
extern void termineRAG(RAG * rag);
extern int32_t updateRAGArc(RAG *rag, int32_t i, int32_t s, uint8_t val);

// Les deux fonctions suivantes vont plutot dans la bibliothèque
// hierarchie
extern void attributNoeud(RAG *rag, struct xvimage *label, struct xvimage *ga, struct xvimage *annexe);
#ifdef __cplusplus
}
#endif
