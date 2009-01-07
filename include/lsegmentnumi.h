/* $Id: lsegmentnumi.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
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
