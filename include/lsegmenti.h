/* $Id: lsegmenti.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
