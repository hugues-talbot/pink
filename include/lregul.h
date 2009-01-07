/* $Id: lregul.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lregul.c */
/* ============== */

extern int32_t lregul(
        struct xvimage * lvkern, 
        int32_t mode,
        double alpha,
        int32_t * nbreg
);

#ifdef __cplusplus
}
#endif
