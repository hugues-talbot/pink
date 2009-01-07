/* $Id: lmedialaxis.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern struct xvimage * lmedialaxis(struct xvimage *f, int32_t connex);
extern int32_t lmedialaxisbin(struct xvimage *f, int32_t connex);
extern int32_t lbisector(struct xvimage *id, struct xvimage *im, struct xvimage *ia);
extern int32_t lbisector_talbot(struct xvimage * image, struct xvimage *angles);
extern int32_t lmedax_Remy_Thiel(struct xvimage *ImageDist, struct xvimage *ImageMedial);
extern int32_t llambdamedialaxis(struct xvimage *dist, struct xvimage *lambda);
#ifdef __cplusplus
}
#endif
