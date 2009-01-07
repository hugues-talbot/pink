/* $Id: mcsplines.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern void scn_solvespline(double *x, double *y, int32_t n, 
			    double *Z0, double *Z1, double *Z2, double *Z3);
extern void scn_solvespline1(double *y, int32_t n, 
			     double *Z0, double *Z1, double *Z2, double *Z3);
extern void scn_solveclosedspline(double *x, double *y, int32_t n, 
			          double *Z0, double *Z1, double *Z2, double *Z3);
extern void scn_samplespline(double *x, double *y, int32_t n, int32_t m, double *X, double *Y);
extern void scn_samplespline3d(double *x, double *y, double *z, int32_t n, int32_t m, double *X, double *Y, double *Z);
extern void scn_curvatures(double *x, double *y, int32_t n, int32_t m, double *sk, double *rhok);
extern void scn_approxcurve(int32_t *X, int32_t *Y, int32_t N, double deltamax, int32_t *Z, int32_t *n, 
                            double *C0, double *C1, double *C2, double *C3,
                            double *D0, double *D1, double *D2, double *D3);
extern void scn_approxcurve3d(int32_t *X, int32_t *Y, int32_t *Z, int32_t N, double deltamax, 
                              int32_t *C, int32_t *n, 
                              double *C0, double *C1, double *C2, double *C3,
                              double *D0, double *D1, double *D2, double *D3,
			      double *E0, double *E1, double *E2, double *E3);
#ifdef __cplusplus
}
#endif
