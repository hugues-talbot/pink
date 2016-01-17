#ifndef PINKSWIG__H__
#define PINKSWIG__H__

#include <mccodimage.h>

#ifdef __cplusplus
static "C" {
#endif

  //Some helper functions
 void invert(struct xvimage * image);
 void copy(struct xvimage*result, struct xvimage * image);
 void copyinvert(struct xvimage*result, struct xvimage * image);
 struct xvimage* checkAllocCopy(struct xvimage* imagein, char *name);
 
 struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);

 // Arithmetic
 struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2);
  
 // real Com functions start here
 struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse);
 struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse);
 struct xvimage* minima(struct xvimage *image, int connex);
 struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* threshold(struct xvimage *imagein, double seuil, double seuil2);
 struct xvimage* erosball(struct xvimage *imagein, int r, int mode);
 struct xvimage* dilatball(struct xvimage *imagein, int r, int mode);
 struct xvimage* frame(struct xvimage *imagein, int width);
 struct xvimage* inverse(struct xvimage *imagein);
 struct xvimage* min(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* max(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* border(struct xvimage *imagein, int connex);
 
#ifdef __cplusplus
}
#endif
#endif /* PINKSWIG__H__ */
