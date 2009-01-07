/* $Id: lsegmentlignes.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lsegmentlignes.c */
/* ============== */

extern int32_t lsegmentlignes(
        struct xvimage *image,
        int32_t connex,
        int32_t seuilbas,
        int32_t seuilhaut
);

#ifdef __cplusplus
}
#endif
