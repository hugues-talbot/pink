/* $Id: ldilatbin.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for ldilatbin.c */
/* ============== */

extern int32_t ldilatbin(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y
);

#ifdef __cplusplus
}
#endif
