/* $Id: lderiche3d.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

