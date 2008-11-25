/* $Id: lsquelbin.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for lsquelbin.c */
/* ============== */

extern int32_t lsquelbin(struct xvimage *image, int32_t connex, int32_t niseuil);
extern int32_t lhthinbin(struct xvimage *image, int32_t connex, int32_t niseuil, int32_t niter);
extern int32_t lhthinbincontr(struct xvimage *image, struct xvimage *contr, int32_t connex, int32_t niseuil, int32_t niter);
extern int32_t lhthickbin(struct xvimage *image, int32_t connex, int32_t niseuil, int32_t niter);
extern int32_t lhthickbincontr(struct xvimage *image, struct xvimage *contr, int32_t connex, int32_t niseuil, int32_t niter);
