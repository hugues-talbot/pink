/* $Id: llpetopo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for llpetopo.c */
/* ============== */

extern int32_t llpetopo(
        struct xvimage *image,
        struct xvimage *marqueurs,
        int32_t trace
);

#ifdef __cplusplus
}
#endif
