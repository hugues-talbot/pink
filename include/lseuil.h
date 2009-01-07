/* $Id: lseuil.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lseuil.c */
/* ============== */

extern int32_t lseuil(
        struct xvimage *f,
        double seuil);

extern int32_t lseuil2(
        struct xvimage *f,
        uint8_t seuilmin,
        uint8_t seuilmax,
        uint8_t valmin,
        uint8_t valmax);

extern int32_t lseuil3(
        struct xvimage *f,
        double seuil, double seuil2);

#ifdef __cplusplus
}
#endif
