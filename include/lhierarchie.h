// Bibliotheque pour la segmentation hierarchique a base de LPE sur
// les aretes

#define SURFACE 0
#define DYNAMIC 1
#define VOLUME 2

extern int32_t main_cascade(struct xvimage *image, struct xvimage *ga, int32_t param);
extern int32_t saliencyGa(struct xvimage *ga, int32_t param);
