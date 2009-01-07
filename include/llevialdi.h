/* $Id: llevialdi.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t llevialdi(
        struct xvimage *img, /* image de depart */
        int32_t connex,          /* 4, 8 */
        int32_t n);              /* nombre d'iterations */

extern int32_t lrao(
        struct xvimage *img, /* image de depart */
        int32_t connex,          /* 4, 8 */
        int32_t n);              /* nombre d'iterations */
#ifdef __cplusplus
}
#endif
