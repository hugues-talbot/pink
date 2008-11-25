/* $Id: lhistscal.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

