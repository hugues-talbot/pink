/* 
   Segmentation d'images par hierarchie de LPE (saillance et cascade)

   Jean Cousty - 2004-2006 
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <mccodimage.h>
#include <jccodimage.h>
#include <mcimage.h>
#include <jcimage.h>
#include <jcgraphes.h>
#include <llpeGA.h>
#include <jccomptree.h>
#include <lhierarchie.h>
#include <mcutil.h>

// Calcul les attributs surface et profondeur du RAG
void attributNoeud(RAG *rag, struct xvimage *label, struct xvimage *ga)
{
  int32_t i;
  int32_t rs = rowsize(label);                   /* taille ligne      */
  int32_t cs = colsize(label);                   /* taille colonne    */
  int32_t N = rs * cs;                           /* taille image      */
  int32_t *LABEL = SLONGDATA(label);           /* les labels        */
  int32_t alt;
  int32_t l;
  
  for(i = 0; i < rag->g->nsom; i++) {
    /* tout ca peut se calculer au vol lors de la construction de la LPE */
    rag->surface[i] = 0;
    rag->profondeur[i] = 255;
  }
  
  for(i = 0; i < N; i++) {
    alt = altitudePoint(ga, i);
    l = LABEL[i];
    rag->profondeur[l] = min(alt,rag->profondeur[l]);
    rag->surface[l] ++;
  }
}

// Construit un RAG a partir d'une partition (label) et d'un ga
RAG *construitRAG(struct xvimage *ga, struct xvimage *label)
#undef F_NAME
#define F_NAME "construitRAGOpening"
{
  RAG *rag;
  int32_t i,x,y,u;
  int32_t rs = rowsize(label);     /* taille ligne */
  int32_t cs = colsize(label);     /* taille colonne */
  int32_t N = rs * cs;             /* taille image */
  int32_t N_t = 2*N;
  int32_t *LABEL = SLONGDATA(label);              /* l'image de depart */
  int32_t nblabels;

  nblabels = 0;
  for(i = 0; i < N; i++)
    if(LABEL[i] > nblabels) nblabels = LABEL[i];
  nblabels++;
  rag = initRAG(nblabels, 3*N);
  /* Parcourt de toutes les aretes du graphe d'arete F */
  for(u = 0; u < N_t; u ++){
    // si l'arete est bien ds le GA
    if( ( (u < N) && (u%rs < rs-1)) || ((u >= N) && (u < N_t - rs))){
      x = Sommetx(u, N, rs);
      y = Sommety(u, N, rs);
      if(LABEL[x] != LABEL[y]) updateRAGArc(rag, LABEL[x], LABEL[y], UCHARDATA(ga)[u]);
    }
  }
  // Puis calcul les attributs de noeuds du rag
  attributNoeud(rag,label,ga);  
  return rag;
}

/********************************************/
/* Calcule d'attribut d'un arbre des coupes */

/* calcul de la surface des composantes de l'arbre */
int32_t surfaceRec(JCctree *CT, int32_t *SurfaceCompo, int32_t root)
{
  JCsoncell *s; 
  if(CT->tabnodes[root].nbsons != 0){
    SurfaceCompo[root] = 0;
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
    {
      SurfaceCompo[root] += surfaceRec(CT, SurfaceCompo, s->son);
    }
  }
  return SurfaceCompo[root];
}

/* calcule la dynamique des composantes de l'arbre de fusion */
/* fonction inspirée par l'article de F. Meyer, ISMM 96 :    */
/* "The dynamics of minima and contours"                     */
void dynaRecCompTree(JCctree *CT,  /* arbre des coupes    */
	      int32_t root,       /* racine de l'arbre   */
	      int32_t *minSon,    /* cf ordonneMergeTree */
	      int32_t *dynamic    /* dynamique des feuilles de CT */
	     )
     /* dynamic est supposé alloué */
{
  JCsoncell *s;
  int32_t father = CT->tabnodes[root].father;
  
  if(father == -1)
    /* la racine de l'arbre de fusion a une dynamique infinie */
    dynamic[root] = 256;
  else
  {
    if(minSon[root] == minSon[father]) 
      if(dynamic[father] == 256){ 
	/* l'infini est un cas à part */
	dynamic[root] = 256; 
      }
      else dynamic[root] = (CT->tabnodes[father].data - CT->tabnodes[root].data) + dynamic[father];
    else
      dynamic[root] = CT->tabnodes[father].data - CT->tabnodes[root].data;
  }
  // on peut maintenant calculer la dynamique des enfants 
  for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next)
    dynaRecCompTree(CT, s->son, minSon, dynamic);
}

void volumeRec(JCctree *CT, int32_t *SurfaceCompo, int32_t *fuzzyAreaCompo, int32_t *volumeCompo, int32_t root)
{
  JCsoncell *s; 
  if(CT->tabnodes[root].nbsons == 0){
    // feuille
    fuzzyAreaCompo[root] =  SurfaceCompo[root]*(int32_t)CT->tabnodes[root].data;
  }
  else{
    SurfaceCompo[root] = 0;
    fuzzyAreaCompo[root] = 0;
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
    {
      volumeRec(CT, SurfaceCompo, fuzzyAreaCompo, volumeCompo, s->son);
      SurfaceCompo[root] += SurfaceCompo[s->son]; 
      fuzzyAreaCompo[root] += fuzzyAreaCompo[s->son];       
    }    
  }
  if(root != CT->root){
    volumeCompo[root] = ( (int32_t)SurfaceCompo[root] * 
			  (int32_t)CT->tabnodes[CT->tabnodes[root].father].data) - 
      (int32_t)fuzzyAreaCompo[root];

  }else volumeCompo[root] = (int32_t)SurfaceCompo[root] * 
	  (int32_t)CT->tabnodes[root].data - 
	  (int32_t)fuzzyAreaCompo[root];
}

// NB: cette fonction devrait etre utilisé de maniere générique quel
//que soit le critère (pas seulement pour le volume). Je prefère
//cependant attendre l'écriture définitive du papier sur la saillence
//avant de faire des modifications
int32_t attributOpenningRec(JCctree *CT, int32_t *attributCompo, int32_t *attributMerge, int32_t root)
{
  int32_t max,v;
  JCsoncell *s; 
  if(CT->tabnodes[root].father == -1){
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
      attributOpenningRec(CT, attributCompo, attributMerge, s->son);
    attributMerge[root] = attributCompo[root];
    return attributMerge[root]; 
  }  
  if(CT->tabnodes[CT->tabnodes[root].father].data != CT->tabnodes[root].data){
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
      attributOpenningRec(CT, attributCompo, attributMerge, s->son);
    attributMerge[root] = attributCompo[root]; 
    return attributMerge[root]; 
  }
  max = 0;
  for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next)
    if( (v=attributOpenningRec(CT, attributCompo, attributMerge, s->son)) > max) 
      max = v;
  attributMerge[root] = max; 
  return attributMerge[root];
}

/* allows to compute the value of the criterion at which a given
   component disappears. Remark that these are not equal to the
   surface of the components*/
int32_t surfaceOpenningRec(JCctree *CT, int32_t *SurfaceCompo, int32_t *SurfaceMerge, int32_t root)
{
  int32_t max,v;
  JCsoncell *s; 
  if(CT->tabnodes[root].father == -1){
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
      surfaceOpenningRec(CT, SurfaceCompo, SurfaceMerge, s->son);
    SurfaceMerge[root] = SurfaceCompo[root];
    return SurfaceMerge[root]; 
  }  
  if(CT->tabnodes[CT->tabnodes[root].father].data != CT->tabnodes[root].data){
    for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
      surfaceOpenningRec(CT, SurfaceCompo, SurfaceMerge, s->son);
    SurfaceMerge[root] = SurfaceCompo[root]; 
    return SurfaceMerge[root]; 
  }
  max = 0;
  for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next) 
  {
    if( (v=surfaceOpenningRec(CT, SurfaceCompo, SurfaceMerge, s->son)) > max) max = v;
  }
  SurfaceMerge[root] = max; 
  return SurfaceMerge[root];
} 

int32_t* altitudeOrdering(uint8_t* F, int32_t N)
/* Trie par denombrement */
/* T[i] indique l'ordre du sommet i*/
#undef F_NAME
#define F_NAME "jcLinSortIncreasing"
{
  int32_t i, j, k, H[256];
  int32_t *T = (int32_t *)malloc(N * sizeof(int32_t));
  if (T == NULL){   
    fprintf(stderr, "%s() : malloc failed for T\n", F_NAME);
    return NULL;
  }
  for (i = 0; i < 256; i++) H[i] = 0;   // initialise l'histogramme
  for (i = 0; i < N; i++) if(i < N)  H[F[i]] += 1; // calcule l'histogramme
  j = H[0]; H[0] = 0;                   // calcule l'histogramme cumule
  for (i = 1; i < 256; i++) { k = H[i]; H[i] = j; j += k; } 
  for (i = 0; i < N; i++)   
    // tri lineaire
  {
    k = F[i]; j = H[k]; T[i] = j; H[k] += 1;
  }
  return T;
}

/* Calcul minSon (cf description ci-dessous) et renvoie la valeur de
   minSon[root] dans l'ordre clefs */
int32_t ordonneCompTree(int32_t *clefs,  /* tableau ordonnant les
					     feuilles de l'arbre des
					     coupes clefs[i] donne la
					     position de i pour
					     l'ordre  */
		JCctree *CT,                /* L'arbre de fusion */
		int32_t root,             /* racine de l'arbre */
		int32_t *minSon           /* application de V(MT) vers
					     l'ensemble des feuilles
					     de CT telle que le plus
					     petit (pour l'ordre
					     clefs) des descendants de
					     x est minSon(x)  */
		) 
     /* minSon est supposé alloué */
{
  int32_t r,x;
  int32_t m = LONG_MAX;
  JCsoncell *s;
  if(CT->tabnodes[root].nbsons == 0){
    // root est une feuille
    minSon[root] = root;  
    return clefs[root];
  }  
  for(s = CT->tabnodes[root].sonlist; s != NULL; s = s->next){
    r = ordonneCompTree(clefs, CT, s->son, minSon); 
    if(r < m){
      x = minSon[s->son];
      m = r;
    }  
  }   
  minSon[root] = x;  
  return m;
}

int32_t * surfaceMergeTree(JCctree *CT, RAG *rag)
#undef F_NAME
#define F_NAME "surfaceMergeTree"
{
  int32_t *SurfaceCompo;
  int32_t *SurfaceMerge;
  int32_t i;

  if( (SurfaceCompo = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }
  if( (SurfaceMerge = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }  
  /* Initialisation des feuilles de CT */
  for(i = 0; i <rag->g->nsom; i++)
    SurfaceCompo[i] = rag->surface[i];
  surfaceRec(CT, SurfaceCompo, CT->root); 
  surfaceOpenningRec(CT, SurfaceCompo, SurfaceMerge, CT->root); 
  free(SurfaceCompo);
  return SurfaceMerge;
}

int32_t *dynaMergeTree(JCctree *CT, RAG *rag)
#undef F_NAME
#define F_NAME "dynaMergeTree"
{
  int32_t *minSon;
  int32_t *dynaMerge;
  int32_t *clefs;
  int32_t i;
  uint8_t* F;

  if( (minSon = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }
  if( (dynaMerge = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }  
  if( (F = (uint8_t*)malloc(sizeof(char) * rag->g->nsom)) == NULL){
    fprintf(stderr,"%s: erreur de malloc \n",F_NAME);
    exit(0);
  }
  for(i = 0; i < rag->g->nsom; i++)
    F[i] = CT->tabnodes[i].data;
  clefs =  altitudeOrdering(F, rag->g->nsom);  
  ordonneCompTree(clefs, CT, CT->root, minSon);
  dynaRecCompTree(CT, CT->root, minSon, dynaMerge);
  for(i = 0; i < CT->nbnodes; i++)
    // permet simplement de reporter les valeurs de merge sur les
    // aretes du MST
    dynaMerge[i] = dynaMerge[minSon[i]];
  fflush(stdout);
  free(minSon);
  free(clefs);
  free(F);
  return dynaMerge;
}

int32_t *volumeMergeTree(JCctree *CT, RAG *rag)
#undef F_NAME
#define F_NAME "volumeMergeTree"
{
  int32_t *SurfaceCompo;
  int32_t *fuzzyAreaCompo;
  int32_t *VolumeCompo;
  int32_t *VolumeMerge;
  int32_t i;
  int32_t nbsoms = rag->g->nsom;

  if( (SurfaceCompo = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes) ) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }
  if( (fuzzyAreaCompo = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes) ) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }

  if( (VolumeCompo = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }

  /* Initialisation des feuilles de CT */
  for(i = 0; i < nbsoms; i++)
    SurfaceCompo[i] = rag->surface[i];
  
  volumeRec(CT, SurfaceCompo, fuzzyAreaCompo, VolumeCompo, CT->root);  
  if( (VolumeMerge = (int32_t*)malloc(sizeof(int32_t) * CT->nbnodes)) == NULL){
    fprintf(stderr,"%s: erreur de malloc\n", F_NAME);
    exit(0);
  }
  attributOpenningRec(CT, VolumeCompo, VolumeMerge, CT->root); 
  free(fuzzyAreaCompo);
  free(VolumeCompo);
  free(SurfaceCompo);
  return VolumeMerge;
} 

// extraction du MST contenu dans un merge tree, vers un tableau d'arete
// et revaluation des arets en fonction de Attribut
static void mstCompute(mtree *MT, int32_t *MST, int32_t *Valeur, int32_t *Attribut) 
     /* MST et Valeur sont supposés alloués */
{

  int32_t i;
  int32_t k;
  k = 0;
  JCctree *CT = MT->CT;
  // les aretes du mst sont stokées ds MT. A chaque noeud de merge
  // (i.e., les noeuds qui ne sont pas des feuilles) correspond une
  // arete du MST
  for(i = CT->nbnodes - (CT->nbnodes/2); i < CT->nbnodes; i++){
    MST[k] = MT->mergeEdge[i];
    Valeur[k] = min(Attribut[CT->tabnodes[i].sonlist->son], Attribut[CT->tabnodes[i].lastson->son]);
    k++;
  }
}

#define LCAFAST
/* Calcul la carte de saillance à partir du CT de saillance du ga
   d'origine et du flow mapping (label) du ga */
int32_t computeSaliencyMap(JCctree *CT, struct xvimage *ga, int32_t *label, int32_t *attribut)
{
  int32_t rs = rowsize(ga);      /* taille ligne */
  int32_t cs = colsize(ga);      /* taille colonne */
  int32_t N = rs * cs;           /* taille image */
  uint8_t *F = UCHARDATA(ga);   /* l'image de depart */
  int32_t u,x,y,i,j,c1;
  /* la valeur maximum de l'attribut est à la racine */
  double facteur = max(255/(double)attribut[CT->root], 0.05); 
  printf("Attribut[racine] = %d et facteur %lf \n", attribut[CT->root],facteur);
#ifdef LCAFAST 
  /* Structure de donnée pour lca fast */
  int32_t logn, nbRepresent;
  int32_t *Euler, *Depth, *Represent, *Number, **Minim;
  Euler = (int32_t *)calloc(2*CT->nbnodes-1, sizeof(int32_t));
  Represent = (int32_t *)calloc(2*CT->nbnodes-1, sizeof(int32_t));
  Depth = (int32_t *)calloc(CT->nbnodes, sizeof(int32_t));
  Number = (int32_t *)calloc(CT->nbnodes, sizeof(int32_t));
  if ((Euler == NULL) || (Represent == NULL) 
      || (Depth == NULL) || (Number == NULL)) {
    fprintf(stderr, "%s : malloc failed\n", F_NAME);
    return(0);
  }
  
  Minim = LCApreprocess(CT, Euler, Depth, Represent, Number, &nbRepresent, &logn);
  printf("LCApreprocess done\n");
#endif
  for(j = 0; j < cs; j++)
    for(i = 0; i < rs -1; i++){
      u = j * rs + i; 
      x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
      if((x < 0) || (y < 0) || (x >= N) || (y >= N) ){
	printf("erreur index !!!!!\n");
	exit(0);
      }   
      if(label[x] != label[y]){		
#ifdef LCAFAST
	c1 = Represent[LowComAncFast((int32_t)label[x], (int32_t)label[y], Euler, Number, Depth, Minim)];
#endif
#ifndef LCAFAST
	c1 = LowComAncSlow(CT, (int32_t)label[x], (int32_t)label[y]);
#endif
	F[u] = (uint8_t)(min((int32_t)(facteur * (double)attribut[c1]), 255));   
      } 
       else F[u] =0; 
    }
  /* puis les aretes verticales */
   for(j = 0; j < cs -1; j++)
    for(i = 0; i < rs; i++)
    {
      u = N + j * rs + i; x = Sommetx(u,N,rs); y = Sommety(u,N,rs);
      if((x < 0) || (y < 0) || (x >= N) || (y >= N)){
	printf("erreur index !!!!!\n");
	exit(0);
      }  
      if(label[x] != label[y]){
#ifdef LCAFAST
	c1 = Represent[LowComAncFast((int32_t)(label[x]), (int32_t)(label[y]), Euler, Number, Depth, Minim)];
#endif
#ifndef LCAFAST
	c1 = LowComAncSlow(CT, (int32_t)label[x], (int32_t)label[y]);
#endif
	if(c1 < 0){
	  printf("Erreur de lca pour %d %d retourne %d\n", (int32_t)(label[x]),  (int32_t)(label[y]), c1);
	  exit(0);
	} 
	F[u] = (uint8_t)(min(255, (int32_t)(facteur * (double)attribut[c1])));  
      }
      else F[u] = 0;
    }
#ifdef LCAFAST
   free(Euler);
   free(Represent);
   free(Depth);
   free(Number);
   free(Minim[0]);
   free(Minim);
#endif
   return(1);
}

/* waterfall vu comme une succession de LPE sur les aretes*/
int32_t main_cascade(struct xvimage *image, struct xvimage *ga, int32_t param)
     /* 4 connexite uniquement Construit le graphe d'arete de saillance pour les cascades*/
     /* ==================================== */
#undef F_NAME
#define F_NAME "main_cascade"
{
  int32_t rs = rowsize(image);         /* taille ligne */
  int32_t cs = colsize(image);         /* taille colonne */
  int32_t N = rs * cs;                 /* taille image */
  uint8_t *F = UCHARDATA(image);      /* l'image de depart */
  uint8_t *G = UCHARDATA(ga);         /* le GA de sortie */
  int32_t *Label1, *Label2;
  struct GrapheValue *g1, *g2;
  int32_t i,k,y;
  int32_t nbregions;

  if ( (Label1 = malloc(sizeof(int32_t) * N)) == NULL){     
    fprintf(stderr, "%s: Erreur de malloc\n", F_NAME);
    exit(0);
  } 
  if ( (Label2 = malloc(sizeof(int32_t) * N)) == NULL){     
    fprintf(stderr, "%s: Erreur de malloc\n", F_NAME);
    exit(0);
  }
  
  for(i = 0; i < 2*N; i++)
    G[i] = 0;
  printf("Initialisation du Ga out done \n");
  
  g1 = initGrapheValue(N, 4*N - 2*rs - 2*cs); /* nbre d'arete du graphe 4-connexe symetrique correspondant */
  printf("Initialisation du graphe utile \n");
 
  for(i = 0; i < N; i++){
    Label1[i] = i;
    for (k = 0; k < 4; k += 2){
      if((y = voisin(i, k, rs, N)) != -1)
	switch(param){
	case 0: updateArcValue(g1,i, y, (uint8_t)max(F[i],F[y])); break;
	case 1: updateArcValue(g1,i, y, (uint8_t)abs((int32_t)F[i] - (int32_t)F[y])); break;
	default: fprintf(stderr,"%s: Mauvais parametre \n", F_NAME); exit(0);
	}
    }
  }
  printf("Graphe utile calcule \n");
  nbregions = (int32_t)lpeGrapheAreteValuee(g1, Label2);
  printf("LPE du graphe utile done et nb regions %d\n",nbregions);
  fflush(stdout);
  while(nbregions > 1){
    printf("Un etage de la hierarchie \n"); 
    
    g2 = initGrapheValue((int32_t)nbregions, (int32_t)min(nbregions*(nbregions-1), (int32_t)(4*N - 2*rs - 2*cs)) );   
    printf("2eme graphe initialise \n");
    for(i = 0; i < N; i++){
      for(k=0; k < 2; k++)
	if((y = voisin(i, 2*k, rs, N)) != -1){
	  if(Label2[Label1[i]] != Label2[Label1[y]]){
	    // Mise a jour de la carte de saillance du waterfall
	    G[incidente(i,k,rs,N)] ++;
	    switch(param){
	    case 0: updateArcValue(g2,Label2[Label1[i]], Label2[Label1[y]], max(F[i],F[y])); break;
	    case 1: updateArcValue(g2,Label2[Label1[i]], Label2[Label1[y]], 
	    			   (uint8_t)abs((int32_t)F[i] - (int32_t)F[y])); break;
	    default: fprintf(stderr,"%s: Mauvais parametre \n", F_NAME); exit(0);
	    }
	  }
	}
   }
   for(i = 0; i < N; i++){
     Label1[i] = Label2[Label1[i]];
   }
   termineGrapheValue(g1);
   g1 = g2;
   nbregions = (int32_t)lpeGrapheAreteValuee(g1, Label2);
  }
  termineGrapheValue(g1);
  free(Label1);
  free(Label2);
  return 1;
}

/* Calcule la carte de saillance du ga d'origine pour le critere param */
/* Le resulat est stocke ds ga */
int32_t saliencyGa(struct xvimage *ga, int32_t param) 
#undef F_NAME
#define F_NAME "saliencyGa"
{
  struct xvimage *label;
  int32_t rs = rowsize(ga);               /* taille ligne */
  int32_t cs = colsize(ga);               /* taille colonne */
  int32_t *LABEL;
  int32_t *Attribut;                      /* Attribut de surface du merge tree */
  RAG *rag;                               /* Graph d'adjacence de
					     régions */
  mtree *MT;                              /* arbre de fusion pour la
					     saillence */
  int32_t *MST, *Valeur,*STaltitude;      /* Arete du MST et Valuation
					     par attribut */
  JCctree * ST;                             /* Arbre des coupes pour le calcul de la carte de saillance */
  if((label = allocimage(NULL,rs,cs,1,VFF_TYP_4_BYTE)) == NULL){
    fprintf(stderr,"%s : ne peut allouer label \n",F_NAME);
    exit(1);
  }
  LABEL = SLONGDATA(label);    
  flowMappingRecursif(ga,LABEL);
  rag = construitRAG(ga, label);
  
  printf("nbre de regions: %d \n",rag->g->nsom);
  
  // Calcul de l'arbre de fusion et précalcul (stoké directement dans
  // MT) de l'arbre de poids min du graphe d'ajacence des régions
  mergeTree(rag, &MT);
  // calcul des valeurs de fusion des aretes du MST du graphe
  //d'adjacence
  switch(param){
  case SURFACE:
    Attribut = surfaceMergeTree(MT->CT,rag);
    break;
  case DYNAMIC:
    Attribut = dynaMergeTree(MT->CT,rag); 
    break;
  case VOLUME:
    Attribut = volumeMergeTree(MT->CT,rag);
    break;
  default: fprintf(stderr, "%s #1: Parametre incorecte \n",F_NAME); exit(0);
  }
  if((MST = (int32_t *)malloc(sizeof(int32_t) * rag->g->nsom -1)) == NULL) {
    fprintf(stderr,"erreur de malloc \n");
    exit(0);
  }
  if((Valeur = (int32_t *)malloc(sizeof(int32_t) * rag->g->nsom -1)) == NULL) {
    fprintf(stderr,"erreur de malloc \n");
    exit(0);
  } 
  mstCompute(MT, MST, Valeur, Attribut);
  if( (STaltitude = (int32_t *)malloc(sizeof(int32_t) * 2 * rag->g->nsom)) == NULL) {
    fprintf(stderr,"erreur de malloc \n");
    exit(0);
  }
  jcSaliencyTree_b(&ST, MST, Valeur, rag, STaltitude);
  computeSaliencyMap(ST, ga, LABEL, STaltitude);  
  mergeTreeFree(MT);
  componentTreeFree(ST);
  termineRAG(rag);
  free(STaltitude);
  freeimage(label);
  free(Attribut);
  free(MST);
  free(Valeur);
  return 1;
}
