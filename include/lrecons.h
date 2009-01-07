/* $Id: lrecons.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lrecons(
        struct xvimage *img,    /* image de depart en ndg */
        struct xvimage *seg,    /* image binaire (segmentation max) et resultat */
        int32_t connex,             /* 4, 8  */
        int32_t memoire);

#ifdef __cplusplus
}
#endif
