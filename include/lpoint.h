/* $Id: lpoint.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

extern int32_t lpoint(struct xvimage * image1, int32_t x, int32_t y, int32_t z, float v);
extern void lshowpoint(struct xvimage * image1, int32_t x, int32_t y, int32_t z);

#ifdef __cplusplus
}
#endif
