/* $Id: lnbtopo.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

