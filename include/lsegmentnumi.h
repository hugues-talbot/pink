/* $Id: lsegmentnumi.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
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
/* prototype for lsegmentnumi.c */
/* ============== */

extern int32_t lsegmentnumi(
        struct xvimage *image,
        struct xvimage *result
);

#ifdef __cplusplus
}
#endif
