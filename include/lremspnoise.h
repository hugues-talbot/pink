/* $Id: lremspnoise.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lremspnoise.c */
/* ============== */

extern int32_t lremspnoise(
        struct xvimage *image,
        int32_t g,
        int32_t k
);

#ifdef __cplusplus
}
#endif
