/* $Id: lconvol.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lconvol.c */
/* ============== */

extern int32_t lconvol(
  struct xvimage * image,
  struct xvimage * mask,
  int32_t mode
);

extern int32_t ldirectionalfilter(
  struct xvimage *f, 
  double width, 
  double length, 
  int32_t ndir
);

#ifdef __cplusplus
}
#endif
