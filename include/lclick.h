/* $Id: lclick.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
