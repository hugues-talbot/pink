/* $Id: llinapprox.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern double EuclidianDistance(double px0, double py0, double pxi, double pyi);
extern double LineDistance(double px0, double py0, double pxi, double pyi, double pxk, double pyk);
extern int32_t DistanceAnalysis (double* t_min,double* t_max,double* vector_x, double* vector_y, int32_t p0, int32_t pi, double epsilon, double * distance);
extern double Phi (double p0_x, double p0_y, double pi_x, double pi_y);
extern double Psi (double p0_x, double p0_y, double pi_x, double pi_y, double error);
extern void TESAO(double* t_min,double* t_max,double* vector_x, double* vector_y, int32_t p0, int32_t pi, double epsilon);
extern void SJCurveAproximate(double* vector_x, 
		       double* vector_y, 
		       int32_t cont, // nb de points
		       double epsilon);
#ifdef __cplusplus
}
#endif
