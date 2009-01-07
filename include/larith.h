/* $Id: larith.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ladd(
        struct xvimage *image1,
        struct xvimage *image2
);

extern int32_t laddconst(
        struct xvimage *image1,
        int32_t constante
);

extern int32_t larea(
  struct xvimage * image, 
  int32_t *area
);

extern int32_t laverage(
  struct xvimage * image1,
  struct xvimage * image2,
  double alpha
);

extern int32_t ldiff(
	struct xvimage * image1,
        struct xvimage * image2
);

extern int32_t lequal(
	struct xvimage * image1,
        struct xvimage * image2
);

extern int32_t linf(
        struct xvimage *image1,
        struct xvimage *image2
);

extern int32_t linverse(
        struct xvimage *image1
);

extern int32_t lmax(
        struct xvimage *image1,
        struct xvimage *image2
);

extern int32_t lmin(
        struct xvimage *image1,
        struct xvimage *image2
);

extern double lmax1(struct xvimage * image1);
extern double lmin1(struct xvimage * image1);
extern int32_t lneg(struct xvimage * image);

extern int32_t lmult(
        struct xvimage *image1,
        struct xvimage *image2
);

extern int32_t lnormalize(
	struct xvimage * image, 
	float nmin, 
	float nmax
);

extern int32_t lnull(
	struct xvimage * image1
);

extern int32_t lscale(
        struct xvimage * image,
        double scale
);

extern int32_t lsub(
        struct xvimage * image1,
        struct xvimage * image2
);

extern int32_t lsup(
        struct xvimage * image1,
        struct xvimage * image2
);

extern int32_t lvolume(
  struct xvimage * image, 
  double *vol
);

extern int32_t lxor(
        struct xvimage *image1,
        struct xvimage *image2
);
#ifdef __cplusplus
}
#endif
