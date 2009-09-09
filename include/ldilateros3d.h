/* $Id: ldilateros3d.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t ldilatbin3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc);
extern int32_t lerosbin3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc);
extern int32_t ldilatfast3d(struct xvimage *f, uint8_t *mask);
extern int32_t lerosfast3d(struct xvimage *f, uint8_t *mask);
extern int32_t ldilatbin3d2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, 
			int32_t xc, int32_t yc, int32_t zc);
extern int32_t lerosbin3d2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t *tab_es_z, 
			int32_t xc, int32_t yc, int32_t zc);
extern int32_t ldilat3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc);
extern int32_t leros3d(struct xvimage *f, struct xvimage *m, int32_t xc, int32_t yc, int32_t zc);
#ifdef __cplusplus
}
#endif
