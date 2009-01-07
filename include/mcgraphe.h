/* $Id: mcgraphe.h,v 1.4 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/*! \file graphes.h
    \brief structures de base pour la manipulation de graphes
*/

typedef char boolean;
//typedef int32_t TYP_VARC;
//typedef int32_t TYP_VSOM;
typedef double TYP_VARC;
typedef double TYP_VSOM;

#define TRUE 1
#define FALSE 0

#ifndef HUGE
#define HUGE HUGE_VAL
#endif
#define M_PI	 3.14159265358979323846

#define SP_AVG 0
#define SP_MIN 1
#define SP_MAX 2

/* ================================================ */
/* types publics */
/* ================================================ */

/*! \struct cell
    \brief structure de cellule pour les listes chaînees de successeurs.
*/
typedef struct cell {
//! index du sommet
  int32_t som;
//! poids de l'arc pour les graphes pondérés 
  TYP_VARC v_arc; 
//! suite de la liste ou pointeur NULL
  struct cell * next; 
} cell;

/*! \var pcell
    \brief pointeur sur une cellule
*/
typedef cell * pcell; 


/*! \struct graphe
    \brief structure pour la representation des graphes
*/
typedef struct graphe {

  /* informations globales */

//!  nombre de sommets 
  int32_t nsom;         
//!  nombre maximum d'arcs
  int32_t nmaxarc;      
//!  nombre d'arcs
  int32_t narc;         

  /* representation par listes chainees de successeurs (application gamma) */

//!  tableau des cellules en réserve 
  pcell reserve;    
//!  liste des cellules libres gérée en pile lifo 
  pcell libre;      
//!  tableau des listes de successeurs indexé par les sommets 
  pcell * gamma;    

  /* representation par liste d'arcs 
     (vecteurs tete (sommet initial), queue (sommet final)) */

//!  tableau des extremités initiales d'arcs 
  int32_t *tete;        
//!  tableau des extremités finales d'arcs 
  int32_t *queue;       

  /* informations additionelles ajoutees aux arcs */

//!  tableau des valeurs associées aux arcs
  TYP_VARC *v_arcs;

  /* informations additionelles ajoutees aux sommets */

//!  tableau des valeurs associées aux sommets
  TYP_VSOM *v_sommets;

//!  abcisses des sommets 
  double *x;        
//!  ordonnees des sommets 
  double *y;        
//!  cotes des sommets 
  double *z;        
//!  noms des sommets 
  char **nomsommet; 
} graphe;

/* ================================================ */
/* prototypes */
/* ================================================ */

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * InitGraphe(int32_t nsom, int32_t nmaxarc);
extern void TermineGraphe(graphe * g);
extern void CopyXY(graphe * g1, graphe * g2);
extern void CopyVsom(graphe * g1, graphe * g2);
extern graphe * CopyGraphe(graphe * g1);
extern void UnitLength(graphe * g);
extern void UnionGraphes(graphe * g1, graphe * g2);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE lECTURE ET DE SAUVEGARDE EN FICHIER  */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * ReadGraphe(char * filename);
extern void SaveGraphe(graphe * g, char *filename);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'AFFICHAGE ET DE SAUVEGARDE */
/* ====================================================================== */
/* ====================================================================== */

extern void AfficheEnsemble(boolean *s, int32_t n);
extern void AfficheListe(pcell p);
extern void AfficheSuccesseurs(graphe * g) ;
extern void AfficheArcs(graphe * g);
extern void AfficheValeursSommets(graphe * g);
extern void PSGraphe(graphe * g, char *filename, double r, double t, double marge);
extern void EPSGraphe(graphe * g, char *filename, double r, double t, double marge, int32_t noms_sommets, int32_t v_sommets, int32_t col_sommets, int32_t v_arcs);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE PLONGEMENT DE GRAPHES DANS LE PLAN */
/* ====================================================================== */
/* ====================================================================== */

extern void AutoNomsSommets(graphe * g, int32_t mode);
extern void PlongementCirculaire(graphe * g, double r);
extern void PlongementRadial(graphe * g, int32_t c);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS SUR LES LISTES CHAINEES DE SOMMETS */
/* ====================================================================== */
/* ====================================================================== */

extern pcell AlloueCell(pcell * plibre);
extern void LibereCell(pcell * plibre, pcell p);
extern void RetireTete(pcell * plibre, pcell * pliste);
extern void AjouteTete(pcell * plibre, pcell * pliste, int32_t a, TYP_VARC v);
extern int32_t EstDansListe(pcell p, int32_t a);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS */
/* ====================================================================== */
/* ====================================================================== */

extern void AjouteArc(graphe * g, int32_t i, int32_t s);
extern void AjouteArcValue(graphe * g, int32_t i, int32_t s, TYP_VARC v);
extern void RetireArc(graphe * g, int32_t i, int32_t s);
extern int32_t PopSuccesseur(graphe *g, int32_t i);
extern int32_t EstSuccesseur(graphe *g, int32_t i, int32_t s);
extern void Gamma2ListArcs(graphe *g);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE GENERATION DE GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * GrapheAleatoire(int32_t nsom, int32_t narc);

/* ====================================================================== */
/* ====================================================================== */
/* OPERATEURS DE BASE SUR LES GRAPHES */
/* ====================================================================== */
/* ====================================================================== */

extern int32_t Degre(graphe * g, int32_t s);
extern graphe * Symetrique(graphe * g);
extern graphe * FermetureSymetrique(graphe * g);
extern void CompFortConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca);
extern boolean ExisteCircuit(graphe * g, int32_t a);
extern void CompConnexe(graphe * g, graphe *g_1, int32_t a, boolean * Ca);
extern boolean Connexe(graphe * g, graphe *g_1);

/* ====================================================================== */
/* ====================================================================== */
/* ARBRES */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * Kruskal1(graphe * g, graphe *g_1);
extern graphe * Kruskal2(graphe * g, graphe *g_1);
extern void DepthTree(graphe * g, int32_t a, TYP_VARC *depth, int32_t *farthest);
extern graphe * MaxDiameterTree(graphe * g);
extern int32_t LCA(graphe * g, int32_t i, int32_t j);
extern graphe * RootTree(graphe * g, graphe * g_1, int32_t i);
extern void DistRoot(graphe * g, int32_t r);

/* ====================================================================== */
/* ====================================================================== */
/* PLUS COURTS CHEMINS */
/* ====================================================================== */
/* ====================================================================== */

extern void Dijkstra(graphe * g, int32_t i);
extern void Lee(graphe * g, int32_t i);
extern void LeeNO(graphe * g, graphe * g_1, int32_t i);
extern graphe * PCC(graphe * g, int32_t d, int32_t a);
extern graphe * PCCna(graphe * g, int32_t d, int32_t a);

extern graphe * Image2Graphe(struct xvimage *image, int32_t mode, int32_t connex);
extern struct xvimage *Graphe2Image(graphe * g, int32_t rs);
#ifdef __cplusplus
}
#endif
