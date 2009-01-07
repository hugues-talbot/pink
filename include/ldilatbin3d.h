/* $Id: ldilatbin3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for ldilatbin3d.c */
/* ============== */

extern int32_t ldilatbin3d(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y,
        int32_t z
);

#ifdef __cplusplus
}
#endif
