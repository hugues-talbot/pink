/* $Id: lsegmentnum.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#undef SURFACE
#undef PROFONDEUR
#undef VOLUME
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
