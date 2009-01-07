/* $Id: lzoomint.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lzoomint.c */
/* ============== */

extern int32_t lzoomint(
  struct xvimage * in,
  struct xvimage ** out,
  int32_t z,
  int32_t fill);

extern struct xvimage* lzoomintxyz(
  struct xvimage * in,
  int32_t zoomx,
  int32_t zoomy,
  int32_t zoomz,
  int32_t fill);
#ifdef __cplusplus
}
#endif
