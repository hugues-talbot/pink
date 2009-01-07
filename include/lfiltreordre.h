/* $Id: lfiltreordre.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lfiltreordre.c */
/* ============== */

extern int32_t lfiltreordre(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y,
        double r
);

extern int32_t lfiltreordre3d(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y,
        int32_t z,
        double r
);

#ifdef __cplusplus
}
#endif
