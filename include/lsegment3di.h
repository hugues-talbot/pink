/* $Id: lsegment3di.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#define SURFACE    1
#define PROFONDEUR 2
#define VOLUME     4

/* ============== */
/* prototype for lsegment3di.c */
/* ============== */

extern int32_t lsegment3di(
        struct xvimage *image,
        int32_t connex
);
#ifdef __cplusplus
}
#endif
