/* $Id: ldynamique.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t ldynamique(struct xvimage *image, struct xvimage *order, int32_t connex);
extern int32_t lfiltredynamique(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t seuil);
extern int32_t lordermaxima(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nlabels);
extern int32_t lordermaximasurf(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nblabels);
extern int32_t lordermaximavol(struct xvimage *image, struct xvimage *order, int32_t connex, int32_t nblabels);
