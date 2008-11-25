/* $Id: lgettree.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

#define MAX(a,b)  ((a)>(b)?(a):(b))
#define MIN(a,b)  ((a)<(b)?((a!=-1)?(a):(b)):((b!=-1)?(b):(a)))  /* -1 correspond � l'infini */

#define HAUTEUR_LIEN      2

#define NON_RACINE        0
#define RACINE            1

#define ELAGUE            0
#define NON_ELAGUE        1

#define POUMONS           0
#define LOBES             1

#define INVISIBLE         0
#define VISIBLE           1
#define INVISIBLE_BAS     2

#define LIEN_HAUT         0
#define LIEN_BAS          1
#define CADRE_HAUT        2
#define CADRE_BAS         3
#define LIGNE             4
#define NUM_NOEUD         5
#define COORD             6
#define LONGUEUR_BRANCHE  7
#define LEVEL             8
#define MAX_MIN           9
#define LIAISON          10
#define BRANCHES         11
#define NB_FEUILLES      12

#define CENTRE           21
#define GAUCHE           22

typedef struct noeudArbre {
  int32_t numNoeud;
  int32_t visible;
  int32_t coord_X;
  int32_t coord_Y;
  int32_t coord_Z;
  int32_t num_pixel;
  int32_t longBranche;
  int32_t level;
  int32_t levelMax;
  int32_t levelMin;
  int32_t nbFils;
  struct noeudArbre *fils[1];  /* expandable */
} noeudArbre;

typedef struct liste {
  int32_t valeur;
  struct liste * suivant;
} liste;

typedef struct listeNoeud {
  noeudArbre * noeud;
  int32_t valeur;
  struct listeNoeud * suivant;
} listeNoeud;

typedef struct caseArbre {
  noeudArbre * noeud;
  int32_t level;
  int32_t nbFeuilles;
  int32_t numFils;
  int32_t nbFreres;
  int32_t visiblePere;
  int32_t debutPere;
  int32_t colonnePere;
  int32_t colonne;
  struct caseArbre * suivant;
} caseArbre;



extern noeudArbre * lgettree(
  struct xvimage *imgx, 
  int32_t connex,
  int32_t extension,
  struct xvimage *res
);

extern noeudArbre * lgettree2d(
  struct xvimage *imgx, 
  int32_t connex,
  int32_t extension,
  struct xvimage *res
);

extern noeudArbre * lgettree3d(
  struct xvimage *imgx, 
  int32_t connex,
  int32_t extension,
  struct xvimage *res
);

extern noeudArbre * getbranch2d4(
  uint8_t *X,
  int32_t rs,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t racine
);

extern noeudArbre * getbranch2d8(
  uint8_t *X,
  int32_t rs,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t racine
);

extern noeudArbre * getbranch3d6(
  uint8_t *X,
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t racine
);

extern noeudArbre * getbranch3d18(
  uint8_t *X,
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t racine
);

extern noeudArbre * getbranch3d26(
  uint8_t *X,
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t racine
);

extern liste * fils3d26(
  uint8_t *X,
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t current_pixel,
  int32_t extension,
  int32_t *D,
  int32_t compteur
);

int32_t BaisserBranche3d26(
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t current_pixel,
  int32_t *D,
  int32_t level
);

extern int32_t MenuEnregistrement (
  char * chaine
);

extern int32_t Menu1 ();

extern int32_t Menu2 ();

extern int32_t Choix1 (
  noeudArbre * arbre,
  struct xvimage * etiquette
);

extern int32_t Choix2 (
  noeudArbre * arbre,
  int32_t choixSegment,
  struct xvimage * etiquette
);

extern int32_t ChoixEnregistrement (
  noeudArbre * arbre,
  int32_t chois
);

extern int32_t printArbreComplet (
  noeudArbre * arbre,
  FILE * fichier
);

extern int32_t printArbreReduit (
  noeudArbre * arbre,
  FILE * fichier
);

extern listeNoeud * SegmentPoumons (
  noeudArbre * arbre,
  int32_t choixSegment
);

extern listeNoeud * SupprimerNoeud (
  listeNoeud * ptrListe,
  int32_t nbComponents
);

extern int32_t TronquerImage (
  listeNoeud * ptrListe,
  struct xvimage * etiquette,
  char * nomFichier
);

int32_t ConstruireMarqueurs (
  int32_t *D,
  int32_t * M,
  int32_t current_pixel,
  int32_t rs,
  int32_t ps,
  int32_t N,
  int32_t brancheMax
);

extern int32_t printDataComplet (
  caseArbre * ligne,
  int32_t data,
  FILE * fichier
);

extern int32_t printDataReduit (
  caseArbre * ligne,
  int32_t data,
  FILE * fichier
);

extern caseArbre * NextLevel (
  caseArbre * ligne
);

extern int32_t NbFils (
  liste * liste
);

extern int32_t NbFeuilles (
  noeudArbre * arbre,
  int32_t raz
);

extern int32_t ArbreVisible (
  noeudArbre * arbre
);

extern int32_t ElaguerArbre (
  noeudArbre * arbre,
  int32_t levelMax
);

extern int32_t freeArbre (
  noeudArbre * arbre
);

extern int32_t freeLigne (
  caseArbre * ligne
);

extern int32_t freeListeNoeud (
  listeNoeud * liste
);
