/* $Id: lconvol3.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lconvol3.c */
/* ============== */

extern int32_t lconvol3(
  struct xvimage * image,
  struct xvimage * mask,
  int32_t normalise
);
#ifdef __cplusplus
}
#endif
