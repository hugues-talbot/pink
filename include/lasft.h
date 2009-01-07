/* $Id: lasft.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lasft(struct xvimage * image, struct xvimage * imagec, struct xvimage * imagecc, 
		 int32_t connex, int32_t rayonmax);
extern int32_t lasft_ndg(struct xvimage * image, struct xvimage * imagec, struct xvimage * imagecc, 
		 int32_t connex, int32_t rayonmax);
extern int32_t lasft3d(struct xvimage * image, struct xvimage * imagec, struct xvimage * imagecc, 
		   int32_t connex, int32_t rayonmax);
extern int32_t lasft_ndg3d(struct xvimage * image, struct xvimage * imagec, struct xvimage * imagecc, 
		   int32_t connex, int32_t rayonmax);
extern int32_t lasftmed(struct xvimage * image, int32_t connex, int32_t rayonmax);
extern int32_t lasftmed3d(struct xvimage * image, int32_t connex, int32_t rayonmax);
extern int32_t lhpclosingdisc(struct xvimage * image, int32_t connex, int32_t rayon);
extern int32_t lhpopeningdisc(struct xvimage * image, int32_t connex, int32_t rayon);
extern int32_t lhpclosing3dball(struct xvimage * image, int32_t connex, int32_t rayon);
extern int32_t lhpopening3dball(struct xvimage * image, int32_t connex, int32_t rayon);
#ifdef __cplusplus
}
#endif
