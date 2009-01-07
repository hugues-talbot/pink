/* $Id: lfermetrous3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lfermetrous3dbin(struct xvimage *image, int32_t connex, int32_t tailletrous);
extern int32_t lfermetrous3dbin2(struct xvimage *in,struct xvimage *g, int32_t connex, int32_t tailletrous);

#ifdef __cplusplus
}
#endif
