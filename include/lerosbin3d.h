/* $Id: lerosbin3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lerosbin3d.c */
/* ============== */

extern int32_t lerosbin3d(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y,
        int32_t z
);

#ifdef __cplusplus
}
#endif
