/* $Id: ldilateros3d.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

/* les elements structurants */
/*                               0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 */
static uint8_t vois26[26]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static uint8_t vois18[26]={0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0};
static uint8_t vois6 [26]={0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
