/* 
   Manipulation de graphes binaires à aretes valués ou "graphe
   d'adjacence" (arete value + attributs sur les sommets)

                Jean Cousty - 2004-2006
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <mccodimage.h>
#include <mcutil.h>
#include <jccodimage.h>
//#include <mcimage.h>
#include <jcimage.h>
#include <jcgraphes.h>


/* Manipulation des graphes binaires (BasicGraph) */
GrapheBasic *initGrapheBasic(int32_t nsom, int32_t nmaxarc)
{ 
  int i;
  struct GrapheBasic * g = (GrapheBasic *)malloc(sizeof(GrapheBasic));
  if (g == NULL){   
    fprintf(stderr, "initGrapheBasic : malloc failed\n");
    exit(0);
  }
  g->nsom = nsom;
  g->nmaxarc = nmaxarc;
  g->narc = 0;
  
  g->reserve = (BasicCell *)malloc(nmaxarc * sizeof(BasicCell));
  if (g->reserve == NULL){ 
    fprintf(stderr, "initGrapheBasic : malloc reserve failed\n");
    exit(0);
  }

  g->gamma = (PBasicCell *)calloc(nsom, sizeof(PBasicCell)); /* calloc met a 0 la memoire allouee */
  if (g->gamma == NULL){
    (stderr, "initGrapheBasic : calloc gamma failed\n");
    exit(0);
  }
  for(i = 0; i < nsom; i++)g->gamma[i] = NULL;
  /* construit la liste initiale de cellules libres */
  for (i = 0; i < nmaxarc - 1; i++)
    (g->reserve+i)->next = g->reserve+i+1;
  (g->reserve+i)->next = NULL;
  g->libre = g->reserve;    
  return g;
}

void termineGrapheBasic(GrapheBasic *g)
/* ====================================================================== */
{
  int32_t i, n = g->nsom;
  free(g->reserve);
  if (g->gamma) free(g->gamma);
  free(g);
} /* TermineGraphe() */   

uint32_t ajouteGrapheBasicSymArc(GrapheBasic *g, int32_t i, int32_t s)
/* ====================================================================== */
{
  PBasicCell p;
  p = alloueBasicCell( &(g->libre) ); 
  p->next = g->gamma[i];
  p->vertex = s;
  p->edge = g->narc / 2;
  g->gamma[i] = p;
  p = alloueBasicCell( &(g->libre));
  p->next = g->gamma[s];
  p->vertex = i;
  p->edge = g->narc / 2;
  g->gamma[s] = p;
  g->narc += 2;
  return p->edge;
} /* ajouteGrapheBasicSymArc */

PBasicCell alloueBasicCell(PBasicCell *plibre)
/* ====================================================================== */
{
  PBasicCell p;
  if (*plibre == NULL) 
  {
    fprintf(stderr, "AlloueCell : plus de cellules libres\n");
    exit(0);
  }
  p = *plibre;
  *plibre = (*plibre)->next;
  return p;
} /* AlloueCell() */

GrapheValue *initGrapheValue(int32_t nsom, int32_t nmaxarc)
/* ====================================================================== */
{
  struct GrapheValue *gv = (GrapheValue *)malloc(sizeof(GrapheValue));
  gv->g = initGrapheBasic(nsom,nmaxarc);
  if( (gv->F = (uint8_t *)malloc(sizeof(uint8_t)*nmaxarc)) == NULL){
    fprintf(stderr, "initGrapheValue : malloc failed\n");
    exit(0);
  }
  return gv;
} /* InitGraphe() */

void termineGrapheValue(GrapheValue * gv)
/* ====================================================================== */
{
  termineGrapheBasic(gv->g);
  free(gv->F);
  free(gv);
} /* termineGrapheValue() */ 

int32_t updateArcValue(GrapheValue *gv, int32_t i, int32_t s, uint8_t val)
{
  PBasicCell p;
  PBasicCell p1;
  uint32_t index;

  GrapheBasic *g = gv->g;
  uint8_t *F = gv->F;

  /* Pour tout successeur de la region i */
  for (p = g->gamma[i]; p != NULL; p = p->next)
    /* Si la region s est deja adjacente a i */
    if (p->vertex == s){
      F[p->edge] = min(F[p->edge],val);
      return 1;
    }
  
  /* On a trouve un nouvel arc */
  if(g->narc < g->nmaxarc) { 
    if((index = ajouteGrapheBasicSymArc(g,i,s)) < 0){
      fprintf(stderr,"updateArcValue: ne peut plus ajouter d'arc \n");
      exit(0);
    }
    F[index] = val;
    return 1;
  } else {
    fprintf(stderr,"narc = %d et nmaxarc %d, UpdateArc Erreur !!!\n",g->narc,g->nmaxarc); 
    return 0;
  }
}

// manipulation d'un graphe d'adjacence RAG
RAG *initRAG(int32_t nsom, int32_t nmaxarc)
/* ====================================================================== */
{
  RAG *_rag = (RAG *)malloc(sizeof(RAG));
  _rag->g = initGrapheBasic(nsom,nmaxarc);
  if( (_rag->F = (uint8_t *)malloc(sizeof(uint8_t)*nmaxarc)) == NULL){
    fprintf(stderr, "initRAG : malloc failed\n");
    exit(0);
  }
  if( (_rag->profondeur = (uint8_t *)malloc(sizeof(uint8_t)*nsom)) == NULL){
    fprintf(stderr, "initRAG : malloc failed\n");
    exit(0);
  }
  if( (_rag->surface = (uint32_t *)malloc(sizeof(uint32_t)*nsom)) == NULL){
    fprintf(stderr, "initRAG : malloc failed\n");
    exit(0);
  }
  if( (_rag->tete = (uint32_t *)malloc(sizeof(uint32_t)*nmaxarc)) == NULL){
    fprintf(stderr, "initRAG : malloc failed\n");
    exit(0);
  }
  if( (_rag->queue = (uint32_t *)malloc(sizeof(uint32_t)*nmaxarc)) == NULL){
    fprintf(stderr, "initRAG : malloc failed\n");
    exit(0);
  }
  return _rag;
} /* InitGraphe() */

void termineRAG(RAG * rag)
/* ====================================================================== */
{
  termineGrapheBasic(rag->g);
  free(rag->F);
  free(rag->profondeur);
  free(rag->surface);
  free(rag->tete);
  free(rag->queue);
  free(rag);
} /* termineRAG() */ 

/* si les deux sommets i et s ne sont pas voisins ds */
/* g cree une arete de i vers s */
/* Idem updateArcValue*/
int32_t updateRAGArc(RAG *rag, int32_t i, int32_t s, uint8_t val)
#undef F_NAME
#define F_NAME "UpdateArc"
{ 
  PBasicCell p, p1;
  GrapheBasic *g = rag->g;
  uint32_t index;
  uint8_t *F = rag->F;
  uint32_t *tete = rag->tete;
  uint32_t *queue = rag->queue;
  
  /* Pour tout successeur de la region i */
  for (p = g->gamma[i]; p != NULL; p = p->next)
    /* Si la region s est deja adjacente a i */
    if (p->vertex == s){ 
      F[p->edge] = min(val,F[p->edge]);
      return 1; 
    }    
  /* On a trouve un nouvel arc */
  if(g->narc < g->nmaxarc)
  { 
    if((index = ajouteGrapheBasicSymArc(g,i,s)) < 0){
      fprintf(stderr,"updateRAGarc: ne peut plus ajouter d'arc \n");
      exit(0);
    }
    tete[index] = i;
    queue[index] = s;
    F[index] = val;
    return 1;
  } else {
    printf("updateRAGarc Erreur !!!\n"); 
    return 0;
  }
}
