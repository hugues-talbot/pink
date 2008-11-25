/* $Id: ltopotypes.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
/* ============== */
/* prototype for ltopotypes.c */
/* ============== */

extern int32_t ltopotypes(
  struct xvimage * image,
  struct xvimage * types, 
  int32_t connex,
  int32_t bin);

extern int32_t lt4pp(struct xvimage * image);
extern int32_t lt8pp(struct xvimage * image);
extern int32_t lt6pp(struct xvimage * image);
extern int32_t lt26pp(struct xvimage * image);
extern int32_t lalpha(struct xvimage * image, int32_t connex, char sign);
