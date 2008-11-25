/* $Id: l3dkhalimsky.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t l3dkhalimskize(struct xvimage * i, struct xvimage **k, int32_t mode);
extern int32_t l3dcolor(struct xvimage * k);
extern int32_t l3dthin(struct xvimage * k, int32_t nsteps);
extern int32_t l3dskelsurf(struct xvimage * k, int32_t nsteps);
extern int32_t l3disthmus(struct xvimage * f);
extern int32_t l3dlabel(struct xvimage * f, struct xvimage * lab);
extern int32_t l3drecons(struct xvimage * f, uint32_t *tab, int32_t n);
extern int32_t l3dsphere(struct xvimage * k, int32_t x0, int32_t y0, int32_t z0, double r);
extern int32_t l3dplane(struct xvimage * k, double a, double b, double c, double d);
extern int32_t l3dinvariants(struct xvimage *f, int32_t *nbcc, int32_t *nbcav, int32_t *nbtun, int32_t *euler);
extern int32_t l3dborder(struct xvimage * f);
extern int32_t l3dseltype(struct xvimage * k, uint8_t d1, uint8_t d2, uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2);
extern int32_t l3dmakecomplex(struct xvimage * i);
extern int32_t l3dalpha(struct xvimage * i);
extern int32_t l3dbeta(struct xvimage * i);
