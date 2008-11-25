/* $Id: ldraw.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */

extern void ldrawline(struct xvimage * image1, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
extern void ldrawline3d(struct xvimage * image1, int32_t x1, int32_t y1, int32_t z1, int32_t x2, int32_t y2, int32_t z2);
extern void ldrawline2(struct xvimage * image1);
extern void ldrawfilledellipse(struct xvimage * image, double R, double S, double T, double U, double V, double Z);
extern void ldrawcubic1(struct xvimage * image1, double *x, double *y, int32_t nseg, double sx, double sy);
extern void ldrawcubic2(struct xvimage * image1, double *x, double *y, int32_t nseg, double tmin, double tmax);
extern void ldrawcubic3d(struct xvimage * image1, double *x, double *y, double *z, int32_t nseg, double tmin, double tmax);
extern void ldrawball(struct xvimage * image1, double r, double xc, double yc, double zc);
extern void ldrawtorus(struct xvimage * image1, double c, double a, double xc, double yc, double zc);


