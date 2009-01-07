/* $Id: lreconseros3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lreconseros3d.c */
/* ============== */


extern int32_t lreconseros3d(
        struct xvimage *g,
        struct xvimage *f,
        uint8_t *mask
);

#ifdef __cplusplus
}
#endif
