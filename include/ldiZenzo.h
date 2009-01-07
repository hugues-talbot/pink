/* $Id: ldiZenzo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern int32_t ldiZenzoGradient(
	struct xvimage *imageR, struct xvimage *imageV, struct xvimage *imageB,
	double alpha);

int32_t ldiZenzoDirection(struct xvimage *imageR, struct xvimage *imageV, struct xvimage *imageB,
		      struct xvimage *result,
		      double alpha);
#ifdef __cplusplus
}
#endif
