/* $Id: lhtkern.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lhtkern.c */
/* ============== */

extern int32_t lhtkern(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t connex
);

extern int32_t lhtkernu(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t connex
);

extern int32_t llvkern(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t connex
);

extern int32_t llvkernu(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t connex
);

extern int32_t lhthinalpha(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t lhthickalpha(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t lhthindelta(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t lhthickdelta(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t lhthiniso(
        struct xvimage *image, 
        double dmax, 
        int32_t connex, 
        double pixwhratio
);

extern int32_t llthindelta(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t llthin(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t llthickdelta(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t llthick(
        struct xvimage *image,
        struct xvimage *imagecond,
        int32_t nitermax,
        int32_t connex
);

extern int32_t lcrestrestoration(
        struct xvimage *image, 
        struct xvimage *imcond, 
        int32_t nitermax, 
        int32_t connex
);

extern int32_t lptextensible(
        struct xvimage *image, 
        struct xvimage *imcond, 
        int32_t connex
);

#ifdef __cplusplus
}
#endif
