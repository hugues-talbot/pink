/* $Id: llpemeyer4d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
int32_t llpemeyer4d(struct xvimage4D *image, struct xvimage4D *marqueurs, struct xvimage4D *marqueursfond, struct xvimage4D *masque, int32_t connex);
#ifdef __cplusplus
}
#endif
