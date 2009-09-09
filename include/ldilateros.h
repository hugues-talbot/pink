/* $Id: ldilateros.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldilatnum( struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t lerosnum( struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t ldilatbin(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t lerosbin(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t ldilatfast(struct xvimage *f, uint8_t *mask) ;
extern int32_t lerosfast(struct xvimage *f, uint8_t *mask);
extern int32_t ldilat(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t ldilat2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t xc, int32_t yc);
extern int32_t ldilat3(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, int32_t xc, int32_t yc, int32_t zc);
extern int32_t leros(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc);
extern int32_t leros2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t xc, int32_t yc);
extern int32_t leros3(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, int32_t xc, int32_t yc, int32_t zc);
#ifdef __cplusplus
}
#endif
