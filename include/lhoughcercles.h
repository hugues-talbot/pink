/* $Id: lhoughcercles.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern int32_t lhoughcercles(
                  struct xvimage *f, /* image de depart - en niveaux de gris */
                  int32_t rayonmin,      /* premier rayon */
                  int32_t pasrayon,      /* increment du rayon */
                  int32_t nbpas,         /* nombre de rayons a considerer */
                  struct xvimage *h  /* accumulateur - doit etre alloue aux bonnes dimensions */
		  );
