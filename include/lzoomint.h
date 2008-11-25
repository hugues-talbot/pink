/* $Id: lzoomint.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
