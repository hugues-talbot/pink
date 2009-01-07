/* $Id: llambdakern.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for llambdakern.c */
/* ============== */

extern int32_t llambdakern(
  struct xvimage *image, 
  struct xvimage *imcon, 
  int32_t connex,
  int32_t lambda);

extern int32_t llambdathin(
  struct xvimage *image,
  struct xvimage *imagecond,
  int32_t nitermax,
  int32_t connex,
  int32_t lambda);

extern int32_t llambdathick(
  struct xvimage *image,
  struct xvimage *imagecond,
  int32_t nitermax,
  int32_t connex,
  int32_t lambda);

extern int32_t lgrayskel(
  struct xvimage *image, 
  struct xvimage *imcon, 
  int32_t connex,
  int32_t lambda);

#ifdef __cplusplus
}
#endif
