/* $Id: mclin.h,v 1.2 2009-01-06 13:18:06 mcouprie Exp $ */
#define MCLIN_EPSILON 1E-10

extern double * lin_zeros(int32_t n, int32_t m);
extern double * lin_ones(int32_t n, int32_t m);
extern void lin_printmat(double * mat, int32_t n, int32_t m);
extern double lin_min(double * mat, int32_t n, int32_t m);
extern double lin_max(double * mat, int32_t n, int32_t m);
extern double lin_sum(double * mat, int32_t n, int32_t m);
extern double lin_norme2(double * mat, int32_t n, int32_t m);
extern double lin_normevec2(double * vec, int32_t n);
extern void lin_transpose(double * mat, int32_t n);
extern void lin_copy(double * dest, double * source, int32_t n, int32_t m);
extern double * lin_duplicate(double *A, int32_t n, int32_t m);
extern void lin_add(double * A, double * B, int32_t n, int32_t m);
extern void lin_sub(double * A, double * B, int32_t n, int32_t m);
extern void lin_multscal(double * A, double r, int32_t n, int32_t m);
extern void lin_multpoint(double * A, double * B, int32_t n, int32_t m);
extern void lin_mult(double * A, double * B , double * R, int32_t p, int32_t q, int32_t r);
extern void lin_multAtB(double * A, double * B , double * R, int32_t nA, int32_t mA, int32_t nB, int32_t mB);
extern int32_t lin_decomposition_LUP(double * A, int32_t * pi, int32_t n);
extern int32_t lin_decomposition_cholesky(double * a, double * l, int32_t n);
extern int32_t lin_inverse_gauss(double *TB, double *InvB, int32_t N);
extern void ROTATE(double * a, int32_t i, int32_t j, int32_t k, int32_t l, double tau, double s, int32_t N);
extern int32_t lin_jacobi(double * A, int32_t n, double * D, double * V, int32_t nrot);
extern void lin_permutmat(int32_t * pi, double * P, int32_t n);
extern void lin_LU(double * A, double * L, double * U, int32_t n);
extern void lin_solveLUP(double * LU, int32_t *P, double * b, double * x, int32_t n);
extern void lin_solve(double * A, double * b, double * x, int32_t n);
extern void lin_solvebidiag(double * A, double * b, double * x, int32_t n);
extern void lin_solvetridiag(double * A, double * b, double * x, int32_t n);
extern double lin_det2(double * m);
extern int32_t lin_invmat2(double * ma, double * mr);
extern double lin_det3(double * m);
extern int32_t lin_invmat3(double * ma, double * mr);
extern int32_t lin_inverseLUP(double * A, double * R, int32_t n);
extern int32_t lellipsefit(double * x, double * y, int32_t n, 
                       double *x0, double *y0, double *xa, double *ya,
					   double *xb, double *yb);
extern int32_t lin_trouvemin(double * x, double * d, double (*F)(double *, int32_t), int32_t n, double *precision);


