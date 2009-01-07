/* $Id: lskelcurv.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern skel * limage2skel(struct xvimage *image, int32_t connex);
extern struct xvimage * lskel2image(skel *S);
#ifdef __cplusplus
}
#endif
