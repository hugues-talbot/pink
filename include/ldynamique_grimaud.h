/* $Id: ldynamique_grimaud.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldynamique_grimaud(struct xvimage *image, int32_t connex);
extern int32_t lwshedval(struct xvimage *image, int32_t connex);
extern int32_t lwshedtopo(struct xvimage *image, int32_t connex);
#ifdef __cplusplus
}
#endif
