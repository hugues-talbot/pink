/* $Id: lhistscal.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lhistscal.c */
/* ============== */

extern int32_t lhistscal(
        struct xvimage *image,
        int32_t a,
        int32_t A,
        int32_t b,
        int32_t B
);

extern int32_t lhistscal3(
        struct xvimage *image,
        int32_t a,
        int32_t A,
        int32_t b,
        int32_t B,
        int32_t c,
        int32_t C
);

#ifdef __cplusplus
}
#endif
