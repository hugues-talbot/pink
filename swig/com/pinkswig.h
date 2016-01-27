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
 int EqualSize(struct xvimage * image1, struct xvimage *image2);
 struct xvimage* checkAllocCopy(struct xvimage* imagein, char *name);
 
 struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);

 // Arithmetic
 struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2);
  
 // real Com functions start here
 struct xvimage* ComputeEdgeGraphGrey(struct xvimage* im, int32_t param, double alpha);
 struct xvimage* ComputeEdgeGraphColor(struct xvimage* r, struct xvimage* g, struct xvimage* b, int32_t param);

 struct xvimage* EWG2Khalimsky(struct xvimage *imagein, int32_t bar);
 struct xvimage* labelfgd(struct xvimage *imagein, int32_t connex);

 struct xvimage* wshedtopo(struct xvimage *image, int connex, int inverse);
 struct xvimage* watershed(struct xvimage *image, struct xvimage *mark, int connex, int inverse);
 void watershedcut(struct xvimage* image, struct xvimage* markers,  struct xvimage** wc, struct xvimage** labels);
 
 struct xvimage* minima(struct xvimage *image, int connex);
 struct xvimage* maxima(struct xvimage *image, int connex);

 struct xvimage* dilation(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* erosion(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* opening(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* closing(struct xvimage *image, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 struct xvimage* asfbin(struct xvimage *imagein, int32_t radiusmax, int32_t radiusmin);
 struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* erosball(struct xvimage *imagein, int r, int mode);
 struct xvimage* dilatball(struct xvimage *imagein, int r, int mode);

 struct xvimage* threshold(struct xvimage *imagein, double seuil, double seuil2);
 struct xvimage* frame(struct xvimage *imagein, int width);
 struct xvimage* inverse(struct xvimage *imagein);
 struct xvimage* border(struct xvimage *imagein, int connex);
 struct xvimage* genball(double radius, int32_t dim);

 struct xvimage* min(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* max(struct xvimage *imagein1, struct xvimage *imagein2);

 struct xvimage* heightmaxima(struct xvimage *imagein, int32_t param, int32_t connex);
 struct xvimage* heightminima(struct xvimage *imagein, int32_t param, int32_t connex);
 struct xvimage* areaopening(struct xvimage *imagein, int32_t param, int32_t connex);
 struct xvimage* areaclosing(struct xvimage *imagein, int32_t param, int32_t connex);
 struct xvimage* volmaxima(struct xvimage *imagein, int32_t param, int32_t connex);
 struct xvimage* volminima(struct xvimage *imagein, int32_t param, int32_t connex);

 struct xvimage* dist(struct xvimage *image, int32_t mode);

 struct xvimage* skeletonprio1(struct xvimage *imagein, struct xvimage *prio, int32_t connex, int32_t inhibvalue);
 struct xvimage* skeletondist1(struct xvimage *imagein, int32_t mode, int32_t connex, int32_t inhibivalue);
 struct xvimage* skeletonprio2(struct xvimage *imagein, struct xvimage *prio, int32_t connex, struct xvimage *inhibimage);
 struct xvimage* skeletondist2(struct xvimage *imagein, int32_t mode, int32_t connex, struct xvimage *inhibimage);
 struct xvimage* ptisolated(struct xvimage *imagein, int32_t connex);
 struct xvimage* ptjunction(struct xvimage *imagein, int32_t connex);
 struct xvimage* ptcurve(struct xvimage *imagein, int32_t connex);
 struct xvimage* ptend(struct xvimage *imagein, int32_t connex);
 
#ifdef __cplusplus
}
#endif
#endif /* PINKSWIG__H__ */
