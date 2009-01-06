/* $Id: lmedialaxis.h,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
extern struct xvimage * lmedialaxis(struct xvimage *f, int32_t connex);
extern int32_t lmedialaxisbin(struct xvimage *f, int32_t connex);
extern int32_t lbisector(struct xvimage *id, struct xvimage *im, struct xvimage *ia);
extern int32_t lbisector_talbot(struct xvimage * image, struct xvimage *angles);
extern int32_t lmedax_Remy_Thiel(struct xvimage *ImageDist, struct xvimage *ImageMedial);
extern int32_t llambdamedialaxis(struct xvimage *dist, struct xvimage *lambda);
