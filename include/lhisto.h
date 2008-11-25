/* $Id: lhisto.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for lhisto.c */
/* ============== */

extern int32_t lhisto(
        struct xvimage *image,
        struct xvimage *mask,
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
#define MAX 1
#define MIN 2
#define NONEXTR 3

extern void labelextr1d(int32_t *F, int32_t n, uint8_t *E);

extern int32_t lhisto2image(uint32_t *histo, int32_t size, struct xvimage **image);
