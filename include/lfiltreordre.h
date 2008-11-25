/* $Id: lfiltreordre.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

