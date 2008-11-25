/* $Id: lhtkern3d.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototypes for lhtkern3d.c */
/* ============== */

extern int32_t lhtkern3dbin(struct xvimage *image, int32_t connex, int32_t nimax);

extern int32_t lhtkern3d(struct xvimage *image, struct xvimage *imagecond, int32_t connexmin);

extern int32_t lhtkernu3d(struct xvimage *image, struct xvimage *imagecond, int32_t connexmin);

extern int32_t llvkern3d(struct xvimage *image, struct xvimage *imagecond, int32_t connexmin);

extern int32_t llvkernu3d(struct xvimage *image, struct xvimage *imagecond, int32_t connexmin);

extern int32_t lhthindelta3d(struct xvimage *image, struct xvimage *imagecond, int32_t nitermax, int32_t connexmin);

extern int32_t lhthickdelta3d(struct xvimage *image, struct xvimage *imagecond, int32_t nitermax, int32_t connexmin);

extern int32_t llthin3d(struct xvimage *image, struct xvimage *imagecond, int32_t nitermax, int32_t connexmin);

extern int32_t llthick3d(struct xvimage *image, struct xvimage *imagecond, int32_t nitermax, int32_t connexmin);

extern int32_t lsurfacerestoration(struct xvimage *image, int32_t nitermax, int32_t connexmin);

