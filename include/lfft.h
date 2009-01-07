/* $Id: lfft.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lfft(struct xvimage *image1, struct xvimage *image2, int32_t dir);
extern int32_t fft842 (int32_t in, int32_t n, float *x, float *y);
#ifdef __cplusplus
}
#endif
