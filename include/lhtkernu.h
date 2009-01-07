/* $Id: lhtkernu.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lhtkernu.c */
/* ============== */

extern int32_t lhtkernu(
        struct xvimage *image,
        int32_t nitermax,
        int32_t connex
);
#ifdef __cplusplus
}
#endif
