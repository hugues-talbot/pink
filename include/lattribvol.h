/* $Id: lattribvol.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lsegmentvol(struct xvimage *image, int32_t connex, int32_t param, int32_t max);
extern int32_t lvolselnb(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lvolmaxima(struct xvimage *image, int32_t connex, int32_t param);
#ifdef __cplusplus
}
#endif
