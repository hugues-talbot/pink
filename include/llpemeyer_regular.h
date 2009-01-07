/* $Id: llpemeyer_regular.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
/* ============== */
/* prototype for llpemeyer.c */
/* ============== */

extern int32_t llpemeyer(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque,
        int32_t connex
);

extern int32_t llpemeyersansligne(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque,
        int32_t connex,
        struct xvimage *result	
);

extern int32_t llpemeyer2(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *masque,
        int32_t connex
);

extern int32_t llpemeyer3d(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque,
        int32_t connex
);

extern int32_t llpemeyer3dsansligne(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque,
        int32_t connex,
        struct xvimage *result	
);

extern int32_t llpemeyer3d2(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *masque,
        int32_t connex
);

extern int32_t llpemeyerkhalimsky(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque
);

extern int32_t llpemeyerbiconnecte(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque,
        int32_t parite);

int32_t llpemeyerbiconnecte3d(
        struct xvimage *image,
        struct xvimage *marqueurs,
        struct xvimage *marqueursfond,
        struct xvimage *masque);
#ifdef __cplusplus
}
#endif
