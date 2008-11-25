/* $Id: lfiltrestopo.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t ldespics(struct xvimage * image, struct xvimage * mask, int32_t connexmin);
extern int32_t ldespuits(struct xvimage * image, struct xvimage * mask, int32_t connexmin);
extern int32_t ldespics3d(struct xvimage * image, struct xvimage * mask, int32_t connexmin);
extern int32_t ldespuits3d(struct xvimage * image, struct xvimage * mask, int32_t connexmin);
extern int32_t ltaf(struct xvimage * image, int32_t connexmin, int32_t rayon);
extern int32_t ltuf(struct xvimage * image, int32_t connexmin, int32_t rayon);
extern int32_t ltlf(struct xvimage * image, int32_t connexmin, int32_t rayon);
extern int32_t ltaflambda(struct xvimage * image, int32_t connexmin, int32_t rayon, int32_t lambdapics, int32_t lambdapuits);

