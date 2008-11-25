/* $Id: llocalextrema.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

/* ============== */
/* prototype for llocalextrema.c */
/* ============== */

int32_t llocalextrema(
        struct xvimage *img,  /* image de depart */
        int32_t connex,           /* 4, 8 (2d) ou 6, 18, 26 (3d) */
        int32_t minimum,          /* booleen */
        struct xvimage *max); /* resultat: image des extrema mis a 255 */

