/* $Id: lwarp.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lwarp.c */
/* ============== */

extern int32_t lwarp(
  struct xvimage *in, 
  triangle *ts, 
  triangle *td, 
  int32_t noclip);
#ifdef __cplusplus
}
#endif
