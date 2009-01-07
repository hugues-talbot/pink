/* $Id: llpetoporeg.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for llpetoporeg.c */
/* ============== */

extern int32_t llpetoporeg(
        struct xvimage *image,
        int32_t seuil,
        int32_t trace
);

#ifdef __cplusplus
}
#endif
