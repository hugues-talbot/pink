/* $Id: llocalextrema.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

/* ============== */
/* prototype for llocalextrema.c */
/* ============== */

int32_t llocalextrema(
        struct xvimage *img,  /* image de depart */
        int32_t connex,           /* 4, 8 (2d) ou 6, 18, 26 (3d) */
        int32_t minimum,          /* booleen */
        struct xvimage *max); /* resultat: image des extrema mis a 255 */

#ifdef __cplusplus
}
#endif
