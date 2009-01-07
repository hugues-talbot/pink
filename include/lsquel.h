/* $Id: lsquel.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lsquel.c */
/* ============== */

extern int32_t lsquel(
        struct xvimage *image,
        int32_t seuil, 
        int32_t niseuil
);
#ifdef __cplusplus
}
#endif
