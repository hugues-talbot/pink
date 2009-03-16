/* $Id: lsegmenti.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#undef SURFACE
#undef PROFONDEUR
#undef VOLUME
#define SURFACE      1
#define PROFONDEUR   2
#define VOLUME       4
#define PERIMETRE    8
#define CIRCULARITE  16
#define ROOTDIST     32
#define NBTROUS      64
#define DENSTROUS    128
#define EXCENTRICITE 256

/* ============== */
/* prototype for lsegmenti.c */
/* ============== */

extern int32_t lsegmenti(
        struct xvimage *image,
        int32_t connex
);
#ifdef __cplusplus
}
#endif
