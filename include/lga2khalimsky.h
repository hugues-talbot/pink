/* $Id: lga2khalimsky.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lga2khalimsky(struct xvimage *ga,struct xvimage *out, int32_t bar);
extern int32_t lga2khalimsky3d(struct xvimage *ga, struct xvimage *out, int32_t bar);
#ifdef __cplusplus
}
#endif
