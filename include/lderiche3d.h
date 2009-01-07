/* $Id: lderiche3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lderiche3d.c */
/* ============== */

extern int32_t lderiche3d(
        struct xvimage *image1,
        double alpha,
        int32_t function,
        double l
);

extern int32_t llisseurrec3d(
        struct xvimage *image1,
        double alpha
);

#ifdef __cplusplus
}
#endif
