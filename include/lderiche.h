/* $Id: lderiche.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lderiche.c */
/* ============== */

extern void derichegen(double *x,
		       int32_t M,
		       int32_t N,
		       double *y1,
		       double *y2,
		       double *y,
		       double a1, double a2, double a3, double a4, 
		       double a5, double a6, double a7, double a8, 
		       double b1, double b2, double b3, double b4);

extern int32_t lderiche(
        struct xvimage *image1,
        double alpha,
        int32_t function,
        double l
);

extern int32_t lshencastan(struct xvimage *image, double beta);

#ifdef __cplusplus
}
#endif
