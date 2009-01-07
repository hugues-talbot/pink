/* $Id: lattribheight.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lsegmentheight(struct xvimage *image, int32_t connex, int32_t param, int32_t max);
extern int32_t lheightmaxima(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lheightselnb(struct xvimage *image, int32_t connex, int32_t param, int32_t mode);
#ifdef __cplusplus
}
#endif
