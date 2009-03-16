/* $Id: lsegment3di.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#undef SURFACE
#undef PROFONDEUR
#undef VOLUME
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
