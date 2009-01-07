/* $Id: lrecalagerigide.h,v 1.4 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern double *lrecalagerigide2d(double *X, int32_t n, double *Y, int32_t m);
extern double *lrecalagerigide3d(double *X, int32_t n, double *Y, int32_t m);
extern int32_t lrecalagerigide2d_num(struct xvimage * image1, struct xvimage * image2, double *G, double seuil, double precision);

#ifdef __cplusplus
}
#endif
