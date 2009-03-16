/* $Id: lsegment.h,v 1.3 2009-03-16 15:52:22 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
#undef SURFACE
#undef PROFONDEUR
#undef VOLUME
#define SURFACE      1
#define PROFONDEUR   2
#define VOLUME       4
#ifdef VERSION_EXPERIMENTALE
#define PERIMETRE    8
#define CIRCULARITE  16
#define ROOTDIST     32
#define NBTROUS      64
#define DENSTROUS    128
#define EXCENTRICITE 256
#endif
/* ============== */
/* prototype for lsegment.c */
/* ============== */

extern int32_t lsegment(
             struct xvimage *image,
             int32_t connex,
             int32_t mesure, 
             int32_t seuilsurf,
             int32_t seuilprof,
             int32_t seuilvol,
#ifdef VERSION_EXPERIMENTALE
             int32_t seuilperim,
             int32_t seuilcirc,
             int32_t seuilrdist,
             int32_t seuiltrou,
             int32_t seuildtrou,
             int32_t seuilexcen,
#endif
             int32_t maximise
);

#ifdef __cplusplus
}
#endif
