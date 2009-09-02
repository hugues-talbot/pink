/* $Id: l2dkhalimsky.h,v 1.4 2009-09-02 14:23:35 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t l2dmakecomplex(struct xvimage * i);
extern int32_t l2dclosebeta(struct xvimage * i);
extern int32_t l2dkhalimskize(struct xvimage * i, struct xvimage **k, int32_t mode);
extern int32_t l2dkhalimskize_noalloc(struct xvimage * i, struct xvimage *k, int32_t mode);
extern int32_t l2dcolor(struct xvimage * k);
extern int32_t l2dthin(struct xvimage * k, int32_t nsteps);
extern int32_t l2dlabel(struct xvimage * f, struct xvimage * lab, int32_t *nlabels);
extern int32_t l2dlabelextrema(
      struct xvimage * f,   /* ordre value' original */
      int32_t minimum,          /* booleen */
      struct xvimage * lab, /* resultat: image de labels */
      int32_t *nlabels);        /* resultat: nombre d'extrema traites + 1 (0 = non extremum) */
extern int32_t l2dtopotess(struct xvimage * lab, struct xvimage * mask);
extern int32_t l2dvoronoi(struct xvimage * lab, struct xvimage * mask);
extern int32_t l2dtopotessndg(struct xvimage * f);
extern int32_t l2dinvariants(struct xvimage *f, int32_t *nbcc, int32_t *nbtrous, int32_t *euler);
extern int32_t l2dseltype(struct xvimage * k, uint8_t d1, uint8_t d2, uint8_t a1, uint8_t a2, uint8_t b1, uint8_t b2);
extern int32_t l2dboundary(struct xvimage * f);
extern int32_t l2dborder(struct xvimage * f);

#ifdef __cplusplus
}
#endif
