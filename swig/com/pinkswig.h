#ifndef PINKSWIG__H__
#define PINKSWIG__H__

#include <mccodimage.h>

#ifdef __cplusplus
static "C" {
#endif

  //Some helper functions
 void invertbyte(struct xvimage * image);
 void copybyte(struct xvimage*result, struct xvimage * image);
 void copyinvertbyte(struct xvimage*result, struct xvimage * image);
 int EqualSize(struct xvimage * image1, struct xvimage *image2);
 struct xvimage* checkAllocCopy(struct xvimage* imagein, char *name);
 
 struct xvimage* createimage(index_t x, index_t y, index_t z, int32_t type, long int data);

 // Arithmetic
 struct xvimage* add(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* addconst(struct xvimage *imagein1, double constante);
 struct xvimage* sub(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* subconst(struct xvimage *imagein1, double constante);
 struct xvimage* mult(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* multconst(struct xvimage *imagein1, double constante);
 struct xvimage* divide(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* divideconst(struct xvimage *imagein1, double constante);
 struct xvimage* sup(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* inf(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* xor(struct xvimage *imagein1, struct xvimage *imagein2);
 struct xvimage* absimg(struct xvimage *imagefloat);
 struct xvimage* normalize(struct xvimage *imagein, double nmin, double nmax);
  
 // real Com functions start here
 struct xvimage* ComputeEdgeGraphGrey(struct xvimage* im, int32_t param, double alpha);
 struct xvimage* ComputeEdgeGraphColor(struct xvimage* r, struct xvimage* g, struct xvimage* b, int32_t param);
 struct xvimage* zerocrossing(struct xvimage *imagefloat, int32_t bar);

 struct xvimage* interpolateX2(struct xvimage *imagefloat);

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
 struct xvimage* asf(struct xvimage *imagein, int32_t radiusmax, int32_t radiusmin);
 struct xvimage* geodilat(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* geoeros(struct xvimage *image1, struct xvimage *image2, int32_t connex, int32_t niter);
 struct xvimage* erosball(struct xvimage *imagein, int r, int mode);
 struct xvimage* dilatball(struct xvimage *imagein, int r, int mode);
 struct xvimage* rankfilter(struct xvimage *image, float r, struct xvimage *elem, int32_t x, int32_t y, int32_t z);
 
 struct xvimage* threshold(struct xvimage *imagein, double seuil, double seuil2);
 struct xvimage* frame(struct xvimage *imagein, int width);
 struct xvimage* invert(struct xvimage *imagein);
 struct xvimage* border(struct xvimage *imagein, int connex);
 struct xvimage* genball(double radius, int32_t dim);

 struct xvimage* long2byte(struct xvimage *imagelong, int32_t mode, int32_t nbnewval);
 struct xvimage* long2float(struct xvimage *imagelong);
 struct xvimage* byte2float(struct xvimage *imagebyte);
 struct xvimage* byte2long(struct xvimage*imagebyte);
 struct xvimage* float2byte(struct xvimage *imagefloat, int32_t mode);
 struct xvimage* float2long(struct xvimage*imagefloat, double offset, double factor);
 struct xvimage* double2byte(struct xvimage *imagedouble, int32_t mode);

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
 struct xvimage* selectcomp(struct xvimage *imagein, int32_t x, int32_t y, int32_t z, int32_t connex);

 struct xvimage* htkern(struct xvimage *imagebyte, int32_t connex, struct xvimage* imagecond);
 struct xvimage* lambdaskel(struct xvimage *imagebyte, int32_t lambda, struct xvimage* imagecond);
 struct xvimage* tuf(struct xvimage *imagebyte, int32_t connexmin, int32_t radius);
 struct xvimage* tlf(struct xvimage *imagebyte, int32_t connexmin, int32_t radius);
 struct xvimage* crestrestoration(struct xvimage *imagebyte, int32_t niter, int32_t connex, struct xvimage* imcond, struct xvimage** condout);
 
#ifdef __cplusplus
}
#endif
#endif /* PINKSWIG__H__ */
