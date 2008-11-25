/* $Id: lgeodesic.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lgeodilat(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t lgeoeros(
        struct xvimage *g,
        struct xvimage *f,
        int32_t connex,
        int32_t niter) ;

extern int32_t ldeletecomp(
        struct xvimage *f,
        int32_t connex,
        int32_t x, int32_t y, int32_t z) ;

extern int32_t lselcomp(
        struct xvimage *f, 
        int32_t connex, 
        int32_t x, int32_t y, int32_t z) ;

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
