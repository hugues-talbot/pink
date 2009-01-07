/* $Id: llabelplateaux.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#define LABPLA      2

/* ============== */
/* prototype for llabelplateaux.c */
/* ============== */

extern int32_t llabelplateaux(
        struct xvimage *img,
        int32_t connex,
        struct xvimage *lab,
        int32_t *nlabels
);
#ifdef __cplusplus
}
#endif
