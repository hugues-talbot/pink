/* $Id: lsegment3d.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define SURFACE    0
#define PROFONDEUR 1
#define VOLUME     2

/* ============== */
/* prototype for lsegment3d.c */
/* ============== */

extern int32_t lsegment3d(
        struct xvimage *image,
        int32_t connex,
        int32_t mesure,
        int32_t seuil,
        int32_t maximise,
        int32_t trace
);

