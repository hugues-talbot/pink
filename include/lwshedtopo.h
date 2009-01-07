/* $Id: lwshedtopo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lwshedtopo(struct xvimage *image, int32_t connex);
extern int32_t lreconsdilat(struct xvimage *g, struct xvimage *f, int32_t connex);
extern int32_t lreconseros(struct xvimage *g, struct xvimage *f, int32_t connex);
extern int32_t lwshedtopobin(struct xvimage *image, struct xvimage *marqueur, int32_t connex);
#ifdef __cplusplus
}
#endif
