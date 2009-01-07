/* $Id: lellipsefit.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lellipsefit(
  double * x, double * y, int32_t n, 
  double *x0, double *y0, double *xa, double *ya, double *xb, double *yb
  );
#ifdef __cplusplus
}
#endif
