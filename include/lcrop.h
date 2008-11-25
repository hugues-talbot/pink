extern struct xvimage * lcrop(struct xvimage *in, int32_t x, int32_t y, int32_t w, int32_t h);
extern struct xvimage * lcrop3d(struct xvimage *in, int32_t x, int32_t y, int32_t z, int32_t w, int32_t h, int32_t d);
extern struct xvimage * lencadre(struct xvimage *image, int32_t grayval);
extern struct xvimage * lenframe(struct xvimage *image, int32_t grayval, int32_t width);
extern int32_t linsert(struct xvimage *a, struct xvimage *b, int32_t x, int32_t y, int32_t z);
extern struct xvimage * lexpandframe(struct xvimage *image1, int32_t n);

