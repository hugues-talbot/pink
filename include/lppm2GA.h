/* $Id: lppm2GA.h,v 1.4 2009-01-08 14:05:19 lnajman Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lpgm2ga(struct xvimage *im, struct xvimage *ga, int32_t param, double alpha);
extern int32_t lpgm2gafloat(struct xvimage *im, struct xvimage *ga, int32_t param, double alpha);
extern int32_t lpgm2gaDouble(struct xvimage *im, struct xvimage *ga, int32_t param, double alpha);
extern int32_t lpgm2gaDouble(struct xvimage *im, struct xvimage *ga, int32_t param, double alpha);
extern int32_t lppm2ga(struct xvimage *r, struct xvimage *v, struct xvimage *b, struct xvimage *ga, int32_t param);
extern int32_t lpgm2ga4d(struct xvimage4D *im, struct GA4d * ga, int32_t param);
extern int32_t compute_scale(uint8_t **image, uint8_t **scale_image, float *scale_map, int32_t *sphere_no_points, /*int16_t ***sphere_points,*/ int32_t N, int32_t rs, int32_t cs, double * feature_mean, int32_t *feature_thr, int32_t * pow_value);
extern int32_t compute_homogeneitysb(uint8_t ** image, double *feature_mean, uint8_t *x_affinity, uint8_t *y_affinity, uint8_t* scale_image, int32_t *sphere_no_points, /*int16_t ***sphere_points,*/ int32_t *feature_thr, float **homogeneity_map, int32_t N, int32_t rs, int32_t cs, int32_t * pow_value);
#ifdef __cplusplus
}
#endif
