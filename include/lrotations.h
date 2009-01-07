/* $Id: lrotations.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lquasishear(struct xvimage * image, double theta, double xc, double yc);
extern struct xvimage * lquasishear2(struct xvimage * image, double theta);
extern struct xvimage * lrotationRT(struct xvimage * image, double theta, double xc, double yc, uint8_t resize);
extern struct xvimage * lrotationInter(struct xvimage * image, double theta, double xc, double yc, uint8_t resize);
extern int32_t laffinetransformation(struct xvimage * image, double theta, double hx, double hy, double tx, double ty, struct xvimage * image2);
#ifdef __cplusplus
}
#endif
