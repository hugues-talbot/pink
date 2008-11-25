/* $Id: lsegmentnum.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define SURFACE    0
#define PROFONDEUR 1
#define VOLUME     2

/* ============== */
/* prototype for lsegmentnum.c */
/* ============== */

extern int32_t lsegmentnum(
        struct xvimage *image,
        int32_t mesure,
        int32_t seuil,
        int32_t trace,
        struct xvimage *result
);
