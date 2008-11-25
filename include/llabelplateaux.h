/* $Id: llabelplateaux.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define LABPLA      2

/* ============== */
/* prototype for llabelplateaux.c */
/* ============== */

extern int32_t llabelplateaux(
        struct xvimage *img,
        int32_t connex,
        struct xvimage *lab,
        int32_t *nlabels
);
