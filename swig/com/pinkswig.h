#ifndef PINKSWIG__H__
#define PINKSWIG__H__

#include <mccodimage.h>

#ifdef __cplusplus
static "C" {
#endif

 struct xvimage *  createimage(index_t x, index_t y, index_t z, int32_t type, long int data);
 void invert(struct xvimage * image);
 void copy(struct xvimage*result, struct xvimage * image);

 void copyinvert(struct xvimage*result, struct xvimage * image);
 struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse);
 struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse);
 struct xvimage* minima(struct xvimage *image, int connex);
 struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 
#ifdef __cplusplus
}
#endif
#endif /* PINKSWIG__H__ */
