/* $Id: lhierarchie.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
// Bibliotheque pour la segmentation hierarchique a base de LPE sur
// les aretes

#undef SURFACE
#undef DYNAMIC
#undef VOLUME
#undef OMEGA
#define SURFACE 0
#define DYNAMIC 1
#define VOLUME 2
#define OMEGA 3

extern int32_t main_cascade(struct xvimage *image, struct xvimage *ga, int32_t param);
extern int32_t saliencyGa(struct xvimage *ga, int32_t param, struct xvimage *annexe);
#ifdef __cplusplus
}
#endif
