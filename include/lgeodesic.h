/* $Id: lgeodesic.h,v 1.3 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lgeodilat(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t lreconsdilat(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex) ;

extern int32_t lgeoeros(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t ldeletecomp(
        struct xvimage *f,
        int32_t connex,
        int32_t x, int32_t y, int32_t z) ;

extern int32_t lselectcomp(
        struct xvimage *f,
        int32_t connex,
        int32_t x, 
        int32_t y, 
        int32_t z) ;

extern int32_t lgeodilat3d(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t lreconsdilat3d(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex) ;

extern int32_t lgeoeros3d(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t lreconseros3d(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex) ;

extern int32_t lamont(
        struct xvimage *m,
        struct xvimage *f,
        int32_t connex,
        int32_t strict) ;
#ifdef __cplusplus
}
#endif
