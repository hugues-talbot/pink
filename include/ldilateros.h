/* $Id: ldilateros.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

/* les elements structurants 2-D */
static uint8_t vois8[8] = {1, 1, 1, 1, 1, 1, 1, 1};
static uint8_t vois4[8] = {1, 0, 1, 0, 1, 0, 1, 0};
static uint8_t croix[8] = {0, 1, 0, 1, 0, 1, 0, 1};
/* les elements structurants 1-D */
static uint8_t horiz[8] = {1, 0, 0, 0, 1, 0, 0, 0};
static uint8_t vert[8]  = {0, 0, 1, 0, 0, 0, 1, 0};
static uint8_t mont[8]  = {0, 1, 0, 0, 0, 1, 0, 0};
static uint8_t desc[8]  = {0, 0, 0, 1, 0, 0, 0, 1};
