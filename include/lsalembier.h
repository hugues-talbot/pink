/* $Id: lsalembier.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lsegreconsheight(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lsegmentheight(struct xvimage *image, int32_t connex, int32_t param, int32_t max);
extern int32_t lsegmentarea(struct xvimage *image, int32_t connex, int32_t param, int32_t max);
extern int32_t lsegmentvol(struct xvimage *image, int32_t connex, int32_t param, int32_t max);
extern int32_t lareaopening(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lareaclosing(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lheightmaxima(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lheightselnb(struct xvimage *image, int32_t connex, int32_t param, int32_t mode);
extern int32_t lareaselnb(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lvolselnb(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t lvolmaxima(struct xvimage *image, int32_t connex, int32_t param);
extern int32_t ldynamique(struct xvimage *image, int32_t connex);
extern int32_t lwshedval(struct xvimage *image, int32_t connex);
extern int32_t lwshedtopo(struct xvimage *image, int32_t connex);
