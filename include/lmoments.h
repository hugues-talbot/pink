/* $Id: lmoments.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

typedef struct {
  int32_t area;
  double xcentre; /* coord. isobarycentre */
  double ycentre;
  double gaxe;  /* longueur demi-grand axe de l'ellipse */
  double excen; /* facteur excentricite */
  double orien; /* orientation / direction horizontale */
  double gdiam; /* longueur du plus grand demi-diametre */
} Indicateur;

extern int32_t limagemoments(
        struct xvimage *img,    /* image de depart */
        Indicateur * indic);    /* resultat */

extern int32_t lmoments(
        struct xvimage *img,    /* image de depart */
        int32_t connex,             /* 4, 8  */
        int32_t minimum,            /* booleen */
        Indicateur ** tabindic, /* tableau des resultats */
        int32_t *nlabels);          /* resultat: nombre d'extrema traites */

extern int32_t lmomentslab(
        struct xvimage *labels, /* image de depart (cc etiquetees) */
        int32_t connex,             /* 4, 8  */
        int32_t nlabels,            /* nombre de composantes */
        Indicateur ** tabindic);/* tableau des resultats */

extern void PrintIndicateur(Indicateur I);
