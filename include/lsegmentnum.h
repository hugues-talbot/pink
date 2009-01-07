/* $Id: lsegmentnum.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
