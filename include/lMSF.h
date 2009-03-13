/* $Id: lMSF.h,v 1.5 2009-03-13 14:46:14 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern int32_t MSF(struct xvimage *ga, struct xvimage *marqueurs);
extern int32_t MSF3d(struct xvimage *ga, struct xvimage *marqueurs);
extern int32_t MSF4d(struct GA4d *ga, struct xvimage4D *marqueurs);
#ifdef __cplusplus
}
#endif
