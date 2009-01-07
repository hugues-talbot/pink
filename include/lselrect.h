/* $Id: lselrect.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lselrect.c */
/* ============== */

extern int32_t lselrect(
  struct xvimage * image,
  int32_t x,
  int32_t y,
  int32_t w,
  int32_t h
);

#ifdef __cplusplus
}
#endif
