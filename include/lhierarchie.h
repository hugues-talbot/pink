/* $Id: lhierarchie.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
// Bibliotheque pour la segmentation hierarchique a base de LPE sur
// les aretes

#define SURFACE 0
#define DYNAMIC 1
#define VOLUME 2

extern int32_t main_cascade(struct xvimage *image, struct xvimage *ga, int32_t param);
extern int32_t saliencyGa(struct xvimage *ga, int32_t param);
#ifdef __cplusplus
}
#endif
