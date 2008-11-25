/* $Id: lclick.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for lclick.h */
/* ============== */

extern int32_t lclick2d(
        struct xvimage *image,
        struct xvimage *lab,
        int32_t zoom
);

extern int32_t lclicklist(
        struct xvimage *image, 
        uint32_t *LAB, 
        int32_t *VLAB, 
        int32_t *nlabs,
        int32_t zoom
);

extern int32_t lclick3d(
        struct xvimage *image, 
        uint32_t *LAB, 
        int32_t *VLAB, 
        int32_t *nlabs,
        int32_t zoom
);
