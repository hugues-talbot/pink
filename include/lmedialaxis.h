/* $Id: lmedialaxis.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern struct xvimage * lmedialaxis(struct xvimage *f, int32_t connex);
extern int32_t lmedialaxisbin(struct xvimage *f, int32_t connex);
extern int32_t lbisector(struct xvimage *id, struct xvimage *im, struct xvimage *ia);
extern int32_t lmedax_Remy_Thiel(struct xvimage *ImageDist, struct xvimage *ImageMedial);
extern int32_t llambdamedialaxis(struct xvimage *dist, struct xvimage *lambda);
