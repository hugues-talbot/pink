/* $Id: lhoughcercles.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t lhoughcercles(
                  struct xvimage *f, /* image de depart - en niveaux de gris */
                  int32_t rayonmin,      /* premier rayon */
                  int32_t pasrayon,      /* increment du rayon */
                  int32_t nbpas,         /* nombre de rayons a considerer */
                  struct xvimage *h  /* accumulateur - doit etre alloue aux bonnes dimensions */
		  );
#ifdef __cplusplus
}
#endif
