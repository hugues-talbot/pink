/* $Id: ldist.h,v 1.4 2009-09-02 14:23:35 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldist(struct xvimage *img, int32_t connex, struct xvimage *res);
extern int32_t ldistbyte(struct xvimage *img, int32_t connex, struct xvimage *res);
extern int32_t ldistquad(struct xvimage *img, struct xvimage *res);
extern int32_t ldistvect(uint8_t *F, vect2Dint *L, int32_t rs, int32_t cs);
extern int32_t ldisteuc(struct xvimage *img, struct xvimage *res);
extern int32_t ldilatdisc(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lerosdisc(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lopendisc(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lclosedisc(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lchamfrein(struct xvimage *img, struct xvimage *res);
extern int32_t ldistquad3d(struct xvimage* ob, struct xvimage* res);
extern int32_t ldisteuc3d(struct xvimage* ob, struct xvimage* res);
extern int32_t ldilatball(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lerosball(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lopenball(struct xvimage* ob, int32_t r, int32_t mode);
extern int32_t lcloseball(struct xvimage* ob, int32_t r, int32_t mode);
extern struct xvimage* ldilatdiscloc(struct xvimage* f, int32_t mode);
extern struct xvimage* ldilatballloc(struct xvimage* f, int32_t mode);
extern int32_t ldistquadSaito(struct xvimage *img, struct xvimage *res);
extern int32_t ldistSaito(struct xvimage *img, struct xvimage *res);
extern int32_t ldistMeijster(struct xvimage *img, struct xvimage *res);
extern struct xvimage* lredt2d(struct xvimage* f);
extern struct xvimage* lredt3d(struct xvimage* f);
extern int32_t lsedt_meijster(struct xvimage *img, struct xvimage *res);
extern void SEDT_line(uint8_t *f, uint32_t *g, uint32_t rs, uint32_t cs);
extern void SEDT_column(uint32_t *f, uint32_t *g, uint32_t rs, uint32_t cs);
extern void SEDT3d_line(uint8_t *f, uint32_t *g, uint32_t rs, uint32_t cs, uint32_t ds);
extern void SEDT3d_column(uint32_t *f, uint32_t *g, uint32_t rs, uint32_t cs, uint32_t ds);
extern void SEDT3d_planes(uint32_t *f, uint32_t *g, uint32_t rs, uint32_t cs, uint32_t ds);
extern int32_t lskeleton_ST(struct xvimage* f, struct xvimage* res);
extern struct xvimage * lopeningfunction(struct xvimage *img, int32_t mode);
#ifdef __cplusplus
}
#endif
