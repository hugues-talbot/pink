/* $Id: lzoom.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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
