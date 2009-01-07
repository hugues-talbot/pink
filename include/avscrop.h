/* $Id: avscrop.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/*
  Functions to be added to lcrop.h

  Author: Andre Vital Saude, 2005
*/
extern struct xvimage *lautocropEnframe3d(struct xvimage *in, int32_t *xi, int32_t *yi, int32_t *zi, int32_t nboxes);
#ifdef __cplusplus
}
#endif
