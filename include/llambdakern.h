/* $Id: llambdakern.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
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

