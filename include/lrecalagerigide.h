/* $Id: lrecalagerigide.h,v 1.2 2008-12-11 13:46:16 mcouprie Exp $ */
extern double *lrecalagerigide2d(double *X, int32_t n, double *Y, int32_t m);
extern double *lrecalagerigide3d(double *X, int32_t n, double *Y, int32_t m);
extern int32_t lrecalagerigide2d_num(struct xvimage * image1, struct xvimage * image2, double *G, double seuil);
