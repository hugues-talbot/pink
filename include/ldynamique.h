/* $Id: ldynamique.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldynamique(struct xvimage *image, struct xvimage *order, int32_t connex);
extern int32_t lfiltredynamique(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t seuil);
extern int32_t lordermaxima(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nlabels);
extern int32_t lordermaximasurf(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nblabels);
extern int32_t lordermaximavol(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nblabels);
#ifdef __cplusplus
}
#endif
