/* $Id: lseltopo.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lptisolated(struct xvimage * image, int32_t connex);
extern int32_t lptinterior(struct xvimage * image, int32_t connex);
extern int32_t lptmultiple(struct xvimage * image, int32_t connex);
extern int32_t lptend(struct xvimage * image, int32_t connex);
extern int32_t lptcurve(struct xvimage * image, int32_t connex);
extern int32_t lptseparating(struct xvimage * image, int32_t connex);
extern int32_t lptseparatinggray(struct xvimage * image, int32_t connex);
extern int32_t lptsimple(struct xvimage * image, int32_t connex);
extern int32_t lseltopo(struct xvimage * image, int32_t connex, int32_t tm, int32_t tp, int32_t tbm, int32_t tbp);
extern int32_t lsimplepair(struct xvimage * image, uint32_t onepair);

