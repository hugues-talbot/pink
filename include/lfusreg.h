/* $Id: lfusreg.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lfusreg.c */
/* ============== */

extern int32_t lfusreg(
  struct xvimage *img,
  struct xvimage *mrq,
  struct xvimage *result,
  int32_t memoire
);

#ifdef __cplusplus
}
#endif
