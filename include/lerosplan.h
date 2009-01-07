/* $Id: lerosplan.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lerosplan.c */
/* ============== */

extern int32_t lerosplan(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y
);

extern int32_t lerosplan2(struct xvimage *f, int32_t nptb, int32_t *tab_es_x, int32_t *tab_es_y, int32_t xc, int32_t yc);
#ifdef __cplusplus
}
#endif
