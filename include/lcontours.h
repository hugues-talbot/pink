/* $Id: lcontours.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lcontours.c */
/* ============== */

extern int32_t lcontours(
        struct xvimage *image,
        int32_t seuilhaut, 
        int32_t seuilbas
);
#ifdef __cplusplus
}
#endif
