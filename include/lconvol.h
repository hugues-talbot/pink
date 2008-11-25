/* $Id: lconvol.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

