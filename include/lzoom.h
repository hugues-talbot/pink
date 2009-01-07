/* $Id: lzoom.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lzoom.c */
/* ============== */

extern int32_t lzoom(
  struct xvimage * in,
  struct xvimage ** out,
  double zoomx, double zoomy, double zoomz
);

extern int32_t lzoom2(
  struct xvimage * in,
  struct xvimage ** out,
  int32_t newdim,
  char dim
);
#ifdef __cplusplus
}
#endif
