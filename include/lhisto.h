/* $Id: lhisto.h,v 1.4 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lhisto.c */
/* ============== */

extern int32_t lhisto(
        struct xvimage *image,
        struct xvimage *mask,
        uint32_t *histo
);

extern int32_t lhisto1(
        struct xvimage *image,
        uint32_t *histo
);

extern int32_t lhisto2(
        struct xvimage *image1, 
        struct xvimage *image2, 
        struct xvimage *mask, 
        struct xvimage *histo
);

extern int32_t lhistolong(
        struct xvimage *image, 
        struct xvimage *mask,
        uint32_t **histo, 
        int32_t *size
);

extern int32_t lhistofloat(struct xvimage *image, struct xvimage *mask, uint32_t **histo, int32_t *size, 
		       float *Sincr, float *Smin, float *Smax);

extern void lhistcompact(uint32_t *histo, int32_t n);

extern int32_t lhistsum(uint32_t *histo);

#define NONMIN 1
#define NONMAX 2
#undef MAX
#define MAX 1
#undef MIN
#define MIN 2
#define NONEXTR 3

extern void labelextr1d(int32_t *F, int32_t n, uint8_t *E);

extern int32_t lhisto2image(uint32_t *histo, int32_t size, struct xvimage **image);
#ifdef __cplusplus
}
#endif
