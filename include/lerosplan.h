/* $Id: lerosplan.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
