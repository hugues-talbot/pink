/* $Id: lerosbin.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lerosbin.c */
/* ============== */

extern int32_t lerosbin(
        struct xvimage *f,
        struct xvimage *m,
        int32_t x,
        int32_t y
);

#ifdef __cplusplus
}
#endif
