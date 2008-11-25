/* $Id: lattribute.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define LABMAX   0
#define LABMIN   1
#define LABPLATEAU   2

#define AREA  0
#define PERIM 1
#define CIRC  2
#define TROUS 3
#define EXCEN 4
#define ORIEN 5
#define VDIAM 6
#define HDIAM 7

/* ============== */
/* prototype for lattribute.c */
/* ============== */

extern int32_t lattribute(
        struct xvimage *img, /* image de depart */
        int32_t connex,          /* 4, 8  */
        int32_t minimum,         /* booleen */
        int32_t attrib,          /* 0: surface, 1: perimetre, 2: circularite ... */
        int32_t seuil,           /* en dessous de seuil, l'attribut est mis a 0 */
        struct xvimage *lab, /* resultat: image d'attributs */
        int32_t *nlabels         /* resultat: nombre d'extrema traites */
);

