/* $Id: lbarycentre.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for lbarycentre.c */
/* ============== */

extern int32_t lbarycentre(struct xvimage * image1, int32_t connex);
extern int32_t lbarycentrelab(struct xvimage * image1);

#ifdef __cplusplus
}
#endif
