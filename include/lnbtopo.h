/* $Id: lnbtopo.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#define PP 0
#define P  1
#define M  2
#define MM 3

/* ============== */
/* prototype for lnbtopo.c */
/* ============== */

extern int32_t lnbtopo(
        struct xvimage *image1,
        int32_t connex,
        int32_t whichnumber
);

#ifdef __cplusplus
}
#endif
