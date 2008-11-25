/* $Id: ldiZenzo.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

extern int32_t ldiZenzoGradient(
	struct xvimage *imageR, struct xvimage *imageV, struct xvimage *imageB,
	double alpha);

int32_t ldiZenzoDirection(struct xvimage *imageR, struct xvimage *imageV, struct xvimage *imageB,
		      struct xvimage *result,
		      double alpha);
