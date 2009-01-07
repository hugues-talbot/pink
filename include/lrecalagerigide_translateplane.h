/* $Id: lrecalagerigide_translateplane.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern double *lrecalagerigide2d(double *X, int n, double *Y, int m);
extern double *lrecalagerigide3d_translateplane(double *X, int n, double *Y, int m);
double *lrecalagerigide3d_translate(double *X, int n, double *Y, int m);
#ifdef __cplusplus
}
#endif
