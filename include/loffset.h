/* $Id: loffset.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for loffset.c */
/* ============== */

extern int32_t loffset(
  struct xvimage * image,
  int32_t ox,
  int32_t oy,
  int32_t oz,
  int32_t mode
);

#ifdef __cplusplus
}
#endif
