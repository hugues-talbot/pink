/* $Id: lsegmentnumi.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define SURFACE    1
#define PROFONDEUR 2
#define VOLUME     4

/* ============== */
/* prototype for lsegmentnumi.c */
/* ============== */

extern int32_t lsegmentnumi(
        struct xvimage *image,
        struct xvimage *result
);

