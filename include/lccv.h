/* $Id: lccv.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lccv.c */
/* ============== */

extern int32_t lccv(
        struct xvimage *f,
        struct xvimage *m
);

extern int32_t lecarttype(struct xvimage *f, struct xvimage *m);
#ifdef __cplusplus
}
#endif
